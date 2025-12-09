cmake_minimum_required(VERSION 3.16) # Versión actualizada para características modernas
project(VoiceQtTicTacToe VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# -----------------------------------------------------
# Encontrar Qt (Funciona bien en Qt Creator)
# -----------------------------------------------------
# Qt Creator inyecta las rutas del kit. Usar 'find_package' es suficiente.
set(Qt6_DIR "$ENV{HOME}/Qt/6.10.0/gcc_64/lib/cmake/Qt6")
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Network
    Concurrent
)
qt_standard_project_setup()


# -----------------------------------------------------
# Encontrar o Construir PortAudio (Con FetchContent)
# -----------------------------------------------------
include(FetchContent)

# Declaración de PortAudio
FetchContent_Declare(
    portaudio
    GIT_REPOSITORY https://github.com/PortAudio/portaudio.git
    GIT_TAG        v19.7.0
    FIND_PACKAGE_ARGS NAMES PortAudio
)
FetchContent_MakeAvailable(portaudio)


# -----------------------------------------------------
# Encontrar PocketSphinx (Lógica Unificada)
# -----------------------------------------------------

if(WIN32)
    message(STATUS "Configurando rutas para Windows.")
    # Ruta de instalación para Windows
    set(POCKETSPHINX_BASE_DIR "C:/cmusphinx")
    set(IMPORTED_NO_SONAME_PROP 1) # Necesario para DLLs/librerías estáticas de Windows
else()
    message(STATUS "Configurando rutas para UNIX.")
    # Ruta de instalación para UNIX (ej. Linux/macOS)
    set(POCKETSPHINX_BASE_DIR "$ENV{HOME}/cmusphinx")
    set(IMPORTED_NO_SONAME_PROP 0)
endif()

# 1. Definir rutas de cabeceras
set(POCKETSPHINX_INCLUDE_DIR_1 "${POCKETSPHINX_BASE_DIR}/include")
set(POCKETSPHINX_INCLUDE_DIR_2 "${POCKETSPHINX_BASE_DIR}/include/pocketsphinx")

# 2. Buscar el archivo de librería
find_library(POCKETSPHINX_LIBRARY
    NAMES pocketsphinx
    PATHS "${POCKETSPHINX_BASE_DIR}/lib"
    REQUIRED
)

# 3. Crear el target IMPORTADO para el enlace.
if(POCKETSPHINX_LIBRARY)
    add_library(pocketsphinx UNKNOWN IMPORTED)

    set_target_properties(pocketsphinx PROPERTIES
        IMPORTED_LOCATION "${POCKETSPHINX_LIBRARY}"
        IMPORTED_LINK_INTERFACE_LANGUAGES "C"
        IMPORTED_NO_SONAME ${IMPORTED_NO_SONAME_PROP}
    )

    # IMPORTANTE: Esto añade las cabeceras al target, eliminando la necesidad
    # de include_directories separadas para PocketSphinx.
    target_include_directories(pocketsphinx INTERFACE
        "${POCKETSPHINX_INCLUDE_DIR_1}"
        "${POCKETSPHINX_INCLUDE_DIR_2}"
    )
    message(STATUS "PocketSphinx encontrado y target 'pocketsphinx' creado: ${POCKETSPHINX_LIBRARY}")
else()
    message(FATAL_ERROR "PocketSphinx no encontrado en ${POCKETSPHINX_BASE_DIR}/lib. Por favor, instale la librería manualmente.")
endif()


# -----------------------------------------------------
# Directorios de Inclusión del Proyecto (Limpieza)
# -----------------------------------------------------
# Solo incluimos nuestros propios directorios. Las librerías las manejan sus targets (pocketsphinx).
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/AI)
include_directories(${CMAKE_SOURCE_DIR}/include/Config)
include_directories(${CMAKE_SOURCE_DIR}/include/Network)
include_directories(${CMAKE_SOURCE_DIR}/include/Controller)
include_directories(${CMAKE_SOURCE_DIR}/include/View)
include_directories(${CMAKE_SOURCE_DIR}/include/Speech)
include_directories(${CMAKE_SOURCE_DIR}/include/Model)


# Recolectar .cpp, .ui, .qrc
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "src/*.cpp"
    "include/*.h"
    "src/*/*.cpp"
    "include/*/*.h"
)

file(GLOB UI_FILES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/ui/*.ui"
)

set(RESOURCE_FILES
    "${CMAKE_SOURCE_DIR}/resources.qrc"
)

# -----------------------------------------------------
# Ejecutable
# -----------------------------------------------------
qt_add_executable(tictactoe
    ${PROJECT_SOURCES}
    ${UI_FILES}
    ${RESOURCE_FILES}
)

# Copiar el archivo de configuración y la carpeta 'share' (antes de ejecutar)
add_custom_command(TARGET tictactoe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_SOURCE_DIR}/config.ini"
        "$<TARGET_FILE_DIR:tictactoe>/config.ini"

    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/share"
        "$<TARGET_FILE_DIR:tictactoe>/share"
)

# -----------------------------------------------------
# Enlazar Librerías
# -----------------------------------------------------
target_link_libraries(tictactoe PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Concurrent
    # PortAudio está disponible a través de FetchContent
    portaudio
    # PocketSphinx (Target IMPORTED)
    pocketsphinx
)

# -----------------------------------------------------
# Inclusión del Módulo de Despliegue
# -----------------------------------------------------
# Esto establece RPATH, y define el target 'deploy_linux'
include(deploy.cmake)
