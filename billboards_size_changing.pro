TEMPLATE = app
QT += 3dcore 3drender 3dinput 3dquick qml quick 3dquickextras 3dextras

SOURCES += \
        main.cpp \
    billboardgeometry.cpp

RESOURCES += \
    image.qrc \
    shaders.qrc


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    billboardgeometry.h
