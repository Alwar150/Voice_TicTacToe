# qt-mingw-toolchain.cmake
# Define el sistema operativo objetivo
SET(CMAKE_SYSTEM_NAME Windows)
SET(CMAKE_SYSTEM_PROCESSOR x86_64)

# Define la ruta a los compiladores MinGW/GCC
SET(QT_TOOLS_MINGW "C:/Qt/Tools/mingw1310_64/bin")

# Forzar los compiladores GCC/G++
SET(CMAKE_C_COMPILER "${QT_TOOLS_MINGW}/gcc.exe")
SET(CMAKE_CXX_COMPILER "${QT_TOOLS_MINGW}/g++.exe")

# Variables de inicialización de MinGW
SET(CMAKE_RC_COMPILER "${QT_TOOLS_MINGW}/windres.exe")
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)