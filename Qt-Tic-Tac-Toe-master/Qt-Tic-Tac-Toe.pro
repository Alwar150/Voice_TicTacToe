#-------------------------------------------------
#
# Project created by QtCreator 2020-06-13T19:45:14
#
#-------------------------------------------------

QT       += core gui widgets concurrent network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qt-Tic-Tac-Toe
TEMPLATE = app

#-------------------------------------------------
# CONFIGURACIÓN DE COMPILACIÓN
#-------------------------------------------------
CONFIG += c++14
DEFINES += QT_DEPRECATED_WARNINGS

#-------------------------------------------------
# INCLUDES
#-------------------------------------------------
INCLUDEPATH += \
    AI \
    Model \
    View \
    Controller \
    Config \
    Speech \
    Forms \
    Network \
    include/

#-------------------------------------------------
# FUENTES
#-------------------------------------------------
SOURCES += \
    AI/Player.cpp \
    Network/NetworkManager.cpp \
    main.cpp \
    AI/MiniMaxAgent.cpp \
    AI/HumanPlayer.cpp \
    Speech/SpeechManager.cpp \
    Model/Board.cpp \
    View/TicTacToeGame.cpp \
    View/TitleScreen.cpp \
    Controller/TTTController.cpp \
    Config/ConfigManager.cpp

HEADERS += \
    AI/AIAgent.h \
    AI/MiniMaxAgent.h \
    AI/Player.h \
    AI/HumanPlayer.h \
    Speech/SpeechManager.h \
    Model/Board.h \
    Network/NetworkManager.h \
    View/TicTacToeGame.h \
    View/TitleScreen.h \
    Controller/TTTController.h \
    Config/ConfigManager.h \
    TTTCommonTypes.h

FORMS += \
    Forms/TicTacToeGame.ui \
    Forms/titlescreen.ui

RESOURCES += resources.qrc
RC_ICONS = images/icon.ico

#-------------------------------------------------
# LIBRERÍAS EXTERNAS
#-------------------------------------------------
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lpocketsphinx
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lpocketsphinx
else:unix: LIBS += -L$$PWD/lib/ -lpocketsphinx \
                 -L/usr/lib/ -lportaudio \
                 -L/usr/local/lib/ -lsox

INCLUDEPATH += $$PWD/include /usr/local/include /usr/include
DEPENDPATH  += $$PWD/include /usr/local/include /usr/include

unix:!macx: PRE_TARGETDEPS += /usr/local/lib/libsox.a

#-------------------------------------------------
# DIRECTORIOS DE COMPILACIÓN Y DESTINO
#-------------------------------------------------
# OUT_PWD → directorio de build (ej. build/Desktop_Qt_6_10_0-Debug)
# DESTDIR → donde se genera el ejecutable
DESTDIR = $$OUT_PWD

#-------------------------------------------------
# COPIA RECURSIVA DE CARPETAS DE RECURSOS
#-------------------------------------------------
# Copiamos todo el contenido de /share dentro del directorio del ejecutable.
# Se usa '.' al final del origen para copiar el contenido, no la carpeta raíz.
# mkdir -p asegura que el directorio exista antes de copiar.
QMAKE_POST_LINK += \
    mkdir -p "$$DESTDIR/share" && \
    echo "Copiando carpetas de recursos a $$DESTDIR/share ..." && \
    cp "$$PWD/config.ini" "$$DESTDIR/" && \
    cp -a "$$PWD/share/." "$$DESTDIR/share/"


#-------------------------------------------------
# INSTALACIÓN (para Linux/UNIX)
#-------------------------------------------------
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
