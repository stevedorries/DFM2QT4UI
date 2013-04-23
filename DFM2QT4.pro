TEMPLATE = app
TARGET = DFM2QT4
QT += core \
    xml \
    gui
HEADERS += cdfm2guitree.h \
    cguitree2ui.h \
    cuidomdocument.h \
    cguitreedomdocument.h \
    cuidomelement.h \
    cguitreedomelement.h \
    clogger.h \
    formpreview.h \
    cfrontend2.h
SOURCES += main.cpp \
    cdfm2guitree.cpp \
    cguitree2ui.cpp \
    cuidomdocument.cpp \
    cguitreedomdocument.cpp \
    cuidomelement.cpp \
    cguitreedomelement.cpp \
    clogger.cpp \
    formpreview.cpp \
    cfrontend2.cpp
RESOURCES += 
FORMS = formpreview.ui \
    cfrontend2.ui
mac:CONFIG = ppc \
    qt
CONFIG += designer
CONFIG += uitools
QMAKE_CXXFLAGS += -Wunused-parameter
# LIBS += -lQtDesignerComponents -lQtDesigner
