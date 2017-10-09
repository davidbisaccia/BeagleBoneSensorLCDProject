QT += core network
QT -= gui

CONFIG += c++11

TARGET = BeagleBoneSensorLCDExe
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    src/Controllers/BeagleBoneController.cpp \
    src/Controllers/TestController.cpp \
    src/Core/GPIO.cpp \
    src/Core/I2CDevice.cpp \
    src/Display/NewHavenLCDDisplay.cpp \
    src/Sensors/MPL3115A2.cpp \
    src/Sound/Buzzer.cpp \
    src/Controllers/BuzzerProcess.cpp \
    src/Controllers/WaterSensorProcess.cpp \
    src/Controllers/TemperatureSensorProcess.cpp \
    src/Controllers/LCDProcess.cpp \
    src/Sensors/WaterSensor.cpp \
    src/Core/AnalogInput.cpp \
    src/Sensors/TMP36.cpp \
    src/Core/UART.cpp \
    src/Communication/SocketHandler.cpp \
    src/Communication/Packets/CommunicationPackets.cpp \
    src/Communication/Packets/PacketBase.cpp \
    src/Controllers/CommunicationProcess.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    include/GPIOInfo.h \
    include/I2CInfo.h \
    include/Controllers/BeagleBoneController.h \
    include/Controllers/TestController.h \
    include/Core/GPIO.h \
    include/Core/I2CDevice.h \
    include/Display/NewHavenLCDDisplay.h \
    include/Sensors/MPL3115A2.h \
    include/Sensors/MPL3115A2SensorData.h \
    include/Sound/Buzzer.h \
    include/Controllers/BuzzerProcess.h \
    src/Controllers/watersensorcontroller.h \
    include/Controllers/WaterSensorProcess.h \
    include/Controllers/TemperatureSensorProcess.h \
    include/Controllers/LCDProcess.h \
    include/Sensors/WaterSensor.h \
    include/Core/AnalogInput.h \
    include/Sensors/TMP36.h \
    include/Core/UART.h \
    include/UARTInfo.h \
    include/Communication/CommunicationPackets.h \
    include/Communication/SocketHandler.h \
    include/Communication/Packets/CommunicationPackets.h \
    include/Communication/Packets/PacketBase.h \
    include/Controllers/CommunicationProcess.h

DISTFILES += \
    Resources/EBB-GPIO-LED.dts \
    Resources/BB-GPIO-SensorSpeaker.dts
