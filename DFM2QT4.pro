TEMPLATE = app
TARGET = DFM2QT4
QT += core \
    xml \
    gui
HEADERS += \
    cguitree2ui.h \
    cuidomdocument.h \
    cguitreedomdocument.h \
    cuidomelement.h \
    cguitreedomelement.h \
    clogger.h \
    formpreview.h \
    cfrontend2.h \
    cdfmparser.h
SOURCES += main.cpp \
    cguitree2ui.cpp \
    cuidomdocument.cpp \
    cguitreedomdocument.cpp \
    cuidomelement.cpp \
    cguitreedomelement.cpp \
    clogger.cpp \
    formpreview.cpp \
    cfrontend2.cpp \
    cdfmparser.cpp
RESOURCES += 
FORMS = formpreview.ui \
    cfrontend2.ui
mac:CONFIG = ppc \
    qt
QT += designer
QT += uitools
QMAKE_CXXFLAGS += -Wunused-parameter
# LIBS += -lQtDesignerComponents -lQtDesigner

DISTFILES += \
    LICENSE \
    README.md \
    COPYRIGHT
