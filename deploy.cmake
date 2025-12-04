# deploy.cmake para tictactoe
# Implementación de despliegue robusta usando el método qt_generate_deploy_app_script de Qt6.

# ==============================================================================
# 1. Configuración de Variables de Instalación
# ==============================================================================

# 1.1 Definir la ruta de instalación temporal donde se creará el paquete AppDir
set(DEPLOY_DIR "${CMAKE_BINARY_DIR}/appdir")

# 1.2 Establecer el prefijo de instalación a la ruta temporal
set(CMAKE_INSTALL_PREFIX ${DEPLOY_DIR})


# ==============================================================================
# 2. Configuración de RPATH
# ==============================================================================

# RPATH: Define dónde buscará el ejecutable las librerías en tiempo de ejecución.
# '$ORIGIN' apunta al directorio del ejecutable ('bin' en este caso).
# La aplicación buscará en 'bin/../lib', que es 'lib' dentro del AppDir.
set(CMAKE_INSTALL_RPATH "\$ORIGIN/../lib")

# Usar la nueva configuración de RPATH.
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)


# ==============================================================================
# 3. Instalación de Recursos Propios (Config y Share)
# ==============================================================================

# 3.1 Instalar el ejecutable en el subdirectorio 'bin'
install(TARGETS tictactoe
    DESTINATION bin
)

# 3.2 Copiar el archivo de configuración a la raíz del AppDir
install(FILES
    "${CMAKE_SOURCE_DIR}/config.ini"
    DESTINATION bin # Copiar a la raíz de DEPLOY_DIR
)

# 3.3 Copiar la carpeta 'share' (recursos) a la raíz del AppDir
install(DIRECTORY
    "${CMAKE_SOURCE_DIR}/share"
    DESTINATION "." # Copiar a la raíz de DEPLOY_DIR
)


# ==============================================================================
# 4. Despliegue de Qt (Usando la función recomendada)
# ==============================================================================

# 4.1 Generar el script de despliegue de Qt.
# Qt lo usará para encontrar todas las librerías, plugins, traducciones, etc.
qt_generate_deploy_app_script(
    TARGET tictactoe
    OUTPUT_SCRIPT DEPLOY_SCRIPT_PATH
    # Podemos excluir librerías que no necesitamos si quisiéramos reducir el tamaño
    # EXCLUDE_PLUGIN_TYPES qml
)

# 4.2 Ejecutar el script generado por Qt como un paso de instalación.
# El script se ejecuta justo antes de los pasos de instalación por defecto.
install(SCRIPT ${DEPLOY_SCRIPT_PATH})


# ==============================================================================
# 5. Objetivo Custom para Despliegue
# ==============================================================================

# Define un objetivo que el usuario puede ejecutar con 'cmake --build . --target deploy_linux'
add_custom_target(deploy_linux
    COMMAND ${CMAKE_COMMAND} --install ${CMAKE_BINARY_DIR} --config $<CONFIG>
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Building self-contained AppDir in: ${DEPLOY_DIR}"
)
