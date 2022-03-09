QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        aerodynamic_function.cpp \
        afterburner.cpp \
        base_calcu.cpp \
        burner.cpp \
        bypass.cpp \
        calcu_fuction.cpp \
        compressor.cpp \
        engine_law.cpp \
        engineerror.cpp \
        gas.cpp \
        gaussfunction.cpp \
        global_variables.cpp \
        inlet.cpp \
        main.cpp \
        mixer.cpp \
        nozzle.cpp \
        turbine.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    aerodynamic_function.h \
    afterburner.h \
    base_calcu.h \
    burner.h \
    bypass.h \
    calcu_fuction.h \
    compressor.h \
    engine_law.h \
    engineerror.h \
    gas.h \
    gaussfunction.h \
    global_variables.h \
    inlet.h \
    mixer.h \
    nozzle.h \
    turbine.h
