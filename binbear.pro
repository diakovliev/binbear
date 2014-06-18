# -------------------------------------------------
# Project created by QtCreator 2012-04-05T14:18:51
# -------------------------------------------------
TARGET = binbear
TEMPLATE = app
QT += core xml
SOURCES += main.cpp \
    mainwindow.cpp \
    qbinarydatasource.cpp \
    qbinarydataview.cpp \
    qbinarydataviewviewport.cpp \
    qbinarydatasourceproxy.cpp \
    qabstractbinarydatasource.cpp \
    qabstractbinarydataformatter.cpp \
    qbinarydataformatter_cxxarray.cpp \
    qbinarydataformatter_base64.cpp \
    qbinarydataformatter_raw.cpp \
    qbinarydataformatter_hex.cpp \
    qbinarydataformatsfactory.cpp \
    qbinarydatacolorscheme.cpp \
    qbinarydatacolorschemasfactory.cpp \
    qbinarydataview_propertieseditor.cpp
HEADERS += mainwindow.h \
    qbinarydatasource.h \
    qbinarydataview.h \
    qbinarydataviewviewport.h \
    binbear.h \
    qbinarydatasourceproxy.h \
    qabstractbinarydatasource.h \
    qabstractbinarydataformatter.h \
    qbinarydataformatter_cxxarray.h \
    qbinarydataformatter_base64.h \
    qbinarydataformatter_raw.h \
    qbinarydataformatter_hex.h \
    qbinarydataformatsfactory.h \
    qbinarydatacolorscheme.h \
    qbinarydatacolorschemasfactory.h \
    qbinarydataview_propertieseditor.h
FORMS += mainwindow.ui \
    qbinarydataview_propertieseditor.ui

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    color_schemas/scheme_4.xml
