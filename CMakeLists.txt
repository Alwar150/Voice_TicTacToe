cmake_minimum_required(VERSION 3.5)

project(VoiceQtTicTacToe VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# -----------------------------------------------------
# Encontrar Qt
# -----------------------------------------------------
if(NOT Qt6_DIR)
    message(STATUS "Qt6_DIR no definido. Buscando instalación de Qt en rutas comunes...")
    
    # Lista de rutas comunes donde el instalador de Qt instala los kits (ajuste estos kits si usa otros)
    set(Qt6_DIR
        # Ruta estándar de instalación que usted usa:
        "C:/Qt/6.10.0/mingw_64"
        # Kits de MSVC populares (útiles para el usuario final):
        "C:/Qt/6.10.0/msvc2022_64"
        "C:/Qt/6.10.0/msvc2019_64"
        # Ruta genérica de instalación para que CMake pruebe versiones anteriores/superiores
        "C:/Qt/6.10.0/mingw_64/lib/cmake/Qt6"
    )
    
    # 1. Búsqueda en la variable de entorno QT_INSTALL_PREFIX (establecida por algunos instaladores)
    if(DEFINED ENV{QT_INSTALL_PREFIX})
        # Agregar la carpeta raíz de la instalación base de Qt
        list(APPEND QT_COMMON_ROOTS "$ENV{QT_INSTALL_PREFIX}")
        message(STATUS "Añadiendo ruta de entorno QT_INSTALL_PREFIX: $ENV{QT_INSTALL_PREFIX}")
    endif()
    
    # 2. Agregar todas estas rutas de raíz a CMAKE_PREFIX_PATH.
    # find_package(Qt6) buscará automáticamente en <prefix>/lib/cmake/Qt6/.
    list(APPEND CMAKE_PREFIX_PATH ${QT_COMMON_ROOTS})
endif()

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Network
    Concurrent
)

qt_standard_project_setup()
# -----------------------------------------------------
# Encontrar PocketSphinx y PortAudio, 
# si no se encuentra se construye desde el Source
# -----------------------------------------------------
include(FetchContent)

if(WIN32)
    message(STATUS "Configurando para Windows: Enlace manual con PocketSphinx precompilado (MinGW).")

    # 1. Rutas Portátiles (sin codificar el nombre de usuario)
    if (DEFINED ENV{LOCALAPPDATA})
        set(LOCAL_APP_DATA "$ENV{LOCALAPPDATA}")
    else()
        set(LOCAL_APP_DATA "$ENV{USERPROFILE}/AppData/Local")
    endif()

    # Directorio de instalación de PocketSphinx
    set(POCKETSPHINX_BASE_DIR "C:/cmusphinx")
    
    # Rutas específicas
    set(POCKETSPHINX_INCLUDE_DIR "${POCKETSPHINX_BASE_DIR}/include")
    set(POCKETSPHINX_INCLUDE_DIR "${POCKETSPHINX_BASE_DIR}/include/pocketsphinx")
    
    # 2. Buscar el archivo de librería (lib/pocketsphinx.lib o lib/pocketsphinx.dll.a)
    find_library(POCKETSPHINX_LIBRARY
        NAMES pocketsphinx
        PATHS "${POCKETSPHINX_BASE_DIR}/lib"
        REQUIRED
    )

    # 3. Crear un target IMPORTADO para el enlace.
    # Esto es crucial para que 'target_link_libraries' funcione.
    if(POCKETSPHINX_LIBRARY)
        # Crear un target INTERFACE para las cabeceras
        add_library(pocketsphinx STATIC IMPORTED) 
    
    # Le decimos a CMake dónde está el archivo de librería
    set_target_properties(pocketsphinx PROPERTIES
        IMPORTED_LOCATION "${POCKETSPHINX_LIBRARY}"
    )

    # Le decimos a CMake dónde están las cabeceras para ese target
    target_include_directories(pocketsphinx INTERFACE
        "${POCKETSPHINX_INCLUDE_DIR}"
    )
        message(STATUS "PocketSphinx encontrado y target creado: ${POCKETSPHINX_LIBRARY}")
    else()
        message(FATAL_ERROR "PocketSphinx no encontrado en ${POCKETSPHINX_BASE_DIR}/lib. Por favor, instale la librería manualmente.")
    endif()
    FetchContent_Declare(
  portaudio
  GIT_REPOSITORY https://github.com/PortAudio/portaudio.git
  GIT_TAG        v19.7.0 
  FIND_PACKAGE_ARGS NAMES PortAudio
)

FetchContent_MakeAvailable(portaudio)
endif(WIN32)

if(UNIX)
    # Directorio de instalación de PocketSphinx
    set(POCKETSPHINX_BASE_DIR "$ENV{HOME}/cmusphinx")
    message(STATUS "usqueda establecida en ${POCKETSPHINX_BASE_DIR}")
    # Rutas específicas
    set(POCKETSPHINX_INCLUDE_DIR "${POCKETSPHINX_BASE_DIR}/include")
    set(POCKETSPHINX_INCLUDE_DIR "${POCKETSPHINX_BASE_DIR}/include/pocketsphinx")

    # 2. Buscar el archivo de librería (lib/pocketsphinx.lib o lib/pocketsphinx.dll.a)
    find_library(POCKETSPHINX_LIBRARY
        NAMES pocketsphinx
        PATHS "${POCKETSPHINX_BASE_DIR}/lib"
        REQUIRED
    )

    # 3. Crear un target IMPORTADO para el enlace.
    # Esto es crucial para que 'target_link_libraries' funcione.
    if(POCKETSPHINX_LIBRARY)
        # Crear un target INTERFACE para las cabeceras
        add_library(pocketsphinx STATIC IMPORTED)

    # Le decimos a CMake dónde está el archivo de librería
    set_target_properties(pocketsphinx PROPERTIES
        IMPORTED_LOCATION "${POCKETSPHINX_LIBRARY}"
    )

    # Le decimos a CMake dónde están las cabeceras para ese target
    target_include_directories(pocketsphinx INTERFACE
        "${POCKETSPHINX_INCLUDE_DIR}"
    )
        message(STATUS "PocketSphinx encontrado y target creado: ${POCKETSPHINX_LIBRARY}")
    else()
        message(FATAL_ERROR "PocketSphinx no encontrado en ${POCKETSPHINX_BASE_DIR}/lib. Por favor, instale la librería manualmente.")
    endif()

endif(UNIX)


# -----------------------------------------------------
# Directorios
# -----------------------------------------------------
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/AI)
include_directories(${CMAKE_SOURCE_DIR}/include/Config)
include_directories(${CMAKE_SOURCE_DIR}/include/Network)
include_directories(${CMAKE_SOURCE_DIR}/include/Controller)
include_directories(${CMAKE_SOURCE_DIR}/include/View)
include_directories(${CMAKE_SOURCE_DIR}/include/Speech)
include_directories(${CMAKE_SOURCE_DIR}/include/Model)
include_directories(${POCKETSPHINX_BASE_DIR}/include)
include_directories(${POCKETSPHINX_BASE_DIR}/include/pocketsphinx)

# Recolectar .cpp
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "src/*.cpp" # Solo cpp en src/
    "include/*.h"  # Solo h en include/
    "src/*/*.cpp" # cpp en subdirectorios de src/
    "include/*/*.h" # h en subdirectorios de include/
     "src/*/*.ui"
)

# Recolectar .ui
file(GLOB UI_FILES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/ui/*.ui"
)

# Recolectar recursos .qrc
set(RESOURCE_FILES
    "${CMAKE_SOURCE_DIR}/resources.qrc"
)


message(STATUS "--------------------------------------------------------")
message(STATUS "Variables de Recursos")
message(STATUS "PROJECT_SOURCES: ${PROJECT_SOURCES}")
message(STATUS "UI_FILES:        ${UI_FILES}")
message(STATUS "RESOURCE_FILES:  ${RESOURCE_FILES}")
message(STATUS "--------------------------------------------------------")

# -----------------------------------------------------
# Ejecutable
# -----------------------------------------------------
qt_add_executable(tictactoe
    ${PROJECT_SOURCES}
    ${UI_FILES}
    ${RESOURCE_FILES}
)

# -----------------------------------------------------
# Enlazar Qt
# -----------------------------------------------------
target_link_libraries(tictactoe PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Concurrent
    portaudio
    # Targets de Dependencias (Pocketsphinx y Portaudio)
    pocketsphinx
)

# -----------------------------------------------------
# Instalar recursos opcionales (si quieres)
# -----------------------------------------------------
install(TARGETS tictactoe
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_custom_command(TARGET tictactoe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_SOURCE_DIR}/config.ini"
        "$<TARGET_FILE_DIR:tictactoe>/config.ini"

    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/share"
        "$<TARGET_FILE_DIR:tictactoe>/share"
)
