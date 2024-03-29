TEMPLATE = lib
TARGET = Engine
QT += qml quick
CONFIG += plugin c++11

DESTDIR = ./Engine
TARGET = $$qtLibraryTarget($$TARGET)
uri = Engine
QML_XHR_ALLOW_FILE_READ=1
# Input
SOURCES += \
        bubblesmodel.cpp \
        engine_plugin.cpp \
        json_loader.cpp

HEADERS += \
        bubblesmodel.h \
        engine_plugin.h \
        json_loader.h

DISTFILES = qmldir

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) "$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)" "$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir
unix {
    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
}

# Copy the qmldir file to the same folder as the plugin binary
cpqmldir.files = qmldir
cpqmldir.path = $$DESTDIR
COPIES += cpqmldir
