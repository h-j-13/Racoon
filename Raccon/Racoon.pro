QT += widgets

HEADERS       = mainwindow.h \
    src/editorMode.h \
    src/codeEditor.h \
    src/highLight.h

SOURCES       = main.cpp \
                mainwindow.cpp \
    src/codeEditor.cpp \
    src/highLight.cpp

RESOURCES     = resources.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/mainwindows/Raccon
INSTALLS += target

CONFIG += console

DISTFILES += \
    Racoon.ico \
    icon.rc

RC_FILE=icon.rc
