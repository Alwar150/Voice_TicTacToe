#!/bin/bash

# Este script comprueba e instala las dependencias de audio/video y Qt xcb
# necesarias en sistemas basados en Debian/Ubuntu.

# Función para verificar e instalar un paquete
# Uso: check_and_install "nombre_del_paquete"
check_and_install() {
    PACKAGE_NAME=$1
    echo "🔍 Comprobando paquete: ${PACKAGE_NAME}..."

    # Usa dpkg -l o dpkg-query para verificar la instalación.
    # El código de salida 0 (éxito) significa que está instalado.
    if dpkg-query -W -f='${Status}' ${PACKAGE_NAME} 2>/dev/null | grep -q "install ok installed"; then
        echo "✅ ${PACKAGE_NAME} ya está instalado."
    else
        echo "❌ ${PACKAGE_NAME} no está instalado. Instalando..."
        if sudo apt install -y "${PACKAGE_NAME}"; then
            echo "👍 ${PACKAGE_NAME} instalado con éxito."
        else
            echo "⚠️ ERROR: No se pudo instalar ${PACKAGE_NAME}. Revisa los logs."
            # Opcional: Salir del script si la instalación de un paquete falla
            # exit 1
        fi
    fi
    echo "---"
}

# --- 1. Actualizar caché de APT ---
echo "Actualizando la caché de APT..."
sudo apt update
echo "-------------------"

# --- 2. Instalación de paquetes de audio/video y desarrollo ---
echo "Comprobando e instalando dependencias de audio/video y desarrollo..."
echo "--------------------------------------------------------"

# Lista de paquetes de audio/video/desarrollo
AUDIO_VIDEO_DEPS=(
    ffmpeg
    libasound2-dev
    libportaudio2
    libportaudiocpp0
    libpulse-dev
    libsox-fmt-all
    portaudio19-dev
    sox
)

for dep in "${AUDIO_VIDEO_DEPS[@]}"; do
    check_and_install "$dep"
done

# --- 3. Instalación de paquetes XCB de Qt ---
echo "Comprobando e instalando librerías XCB para Qt (libxcb-cursor0)..."
echo "------------------------------------------------------------------"

# Paquetes XCB de Qt
QT_XCB_DEPS=(
    libxcb-cursor0
)

for dep in "${QT_XCB_DEPS[@]}"; do
    check_and_install "$dep"
done

echo "✅ Proceso de comprobación e instalación finalizado."
