#include "pxt.h"

using namespace pxt;

namespace sbrick {

    const uint16_t EVENT_SBRICK_CMD  = 0x300c;
    const uint16_t EVENT_SBRICK_RSP  = 0x300d;
    const uint16_t EVENT_SBRICK_ADC  = 0x300e;

    const uint16_t EVENT_VALUE_SBRICK_CONNECTED = 0x0000;
    const uint16_t EVENT_VALUE_SIGNAL_COMPLETED = 0x1000;

    enum class SBPort {
        A = 0,
        B = 2,
        C = 1,
        D = 3
    };

    enum class SBDirection {
        Forward = 0,
        Backward = 1
    };

    enum class SBDimension {
        X = 0,
        Y = 1,
        Z = 2
    };

    enum class SBConnectedDevice {
        //% block="Motor, Servo, or LED"
        Output                =  0,
        //% block="WeDo Motion Sensor"
        Wedo1Motion           =  1,
        //% block=" WeDo Tilt Sensor"
        Wedo1Tilt             =  2
    };

    int _measuredValue;
    SBPort _measuredPort;
    Action _measurementHandler;
    Action _connectedHandler;

    void _onConnected(MicroBitEvent e)
    {
        //if (NULL == _connectedHandler) return;
        pxt::runAction0(_connectedHandler);
    }

    void _onMeasurement(MicroBitEvent e)
    {
        //if (NULL == _measurementHandler) return;

        _measuredPort = (SBPort)((e.value & 0x000f) >> 1);
        _measuredValue = e.value >> 4;

        pxt::runAction0(_measurementHandler);
    }

    //% blockId=sbrick_on_connected
    //% block="on sbrick connected"
    void onConnected(Action handler)
    {
        _connectedHandler = handler;
        uBit.messageBus.listen(EVENT_SBRICK_RSP, EVENT_VALUE_SBRICK_CONNECTED, _onConnected);
    }

    //% blockId=sbrick_drive
    //% block="drive with|power %power|on port %port|in the direction: %direction"
    void drive(int power, SBPort port, SBDirection direction)
    { 
        if (power < 0) {
            power = 0;
        }
        if (power > 255) {
            power = 255;
        }
        MicroBitEvent ev(EVENT_SBRICK_CMD, 0x1000 + 512 * (int)port + 256 * (int)direction + power);
    }

    //% blockId=sbrick_lightmono
    //% block="light on with |power %power|on channel %c" shim=sbrick::lightmono
    void lightmono(int power, int c)
    { 
        if (c < 0 || c >= 24) {
            return;
        }
        if (power < 0) {
            power = 0;
        }
        if (power > 255) {
            power = 255;
        }
        MicroBitEvent ev(EVENT_SBRICK_CMD, 0x0000 + 256 * c + power);
    }

    //% blockId=sbrick_lightrgb
    //% block="RGB light on with |red %red|green %green|blue %blue|on port %p" shim=sbrick::lightrgb
    void lightrgb(int red, int green, int blue, int p)
    { 
        if (p < 0 || p >= 8) {
            return;
        }
        lightmono(red,   p * 3 + 0);
        lightmono(green, p * 3 + 1);
        lightmono(blue,  p * 3 + 2);
    }

    //% blockId=sbrick_drive_from_accel
    //% block="drive |port %p|with acceleration read across dimension %d"
    void driveFromAccel(SBPort p , SBDimension d)
    {
        int x = 0;
        
        switch (d) {
            case SBDimension::X:
                x = uBit.accelerometer.getX();
                break;
            case SBDimension::Y:
                x = uBit.accelerometer.getY();
                break;
            case SBDimension::Z:
                x = uBit.accelerometer.getZ();
                break;
        }
        x /= 2.77; // 0-1024 -> 45° -> 0-255

        if (x < 20 && x > -20) {
            x = 0;
        }

        if (x >= 0) {
            drive(x, p, SBDirection::Forward);
        }
        if (x < 0) {
            drive(-x, p, SBDirection::Backward);
        }
    }

    //% blockId=sbrick_brake
    //% block="brake on|port %p"
    void brake(SBPort p)
    {
        MicroBitEvent ev(EVENT_SBRICK_CMD, 0x0000 + 512 * (int)p);
    }

    //% blockId=sbrick_set_device
    //% block="use device|type %d|on port %p"
    void setDevice(SBConnectedDevice d, SBPort p)
    {
        // 1a. Fire up a "signal" if needed (in case of adapters)
        //     A value needs to be stored globally. This value will be sent
        //     to the SBrick next as the channel compensation type
        // 1b. in the signal completed callback, call the channel compensation
        //     setting function, then "set measurement"
        //     TODO: "set measurement" should also set channel compensation
        // 2. Just "set measurement" with compensation immediately.
        MicroBitEvent ev(EVENT_SBRICK_CMD, 0,  CREATE_ONLY);

        switch (d) {
            case SBConnectedDevice::Output:
                ev.value = 0x3000 + 256 * ((int)p * 2 + 1); // Clear measurement
                break;
            case SBConnectedDevice::Wedo1Motion:
                ev.value = 0x2000 + 256 * ((int)p * 2 + 1) + 1; // Set measurement
                break;
            case SBConnectedDevice::Wedo1Tilt:
                ev.value = 0x2000 + 256 * ((int)p * 2 + 1) + 1; // Set measurement
                break;
        }

        ev.fire();
    }

    //% blockId=sbrick_on_measurement
    //% block="on measurement"
    void onMeasurement(Action handler)
    {
        _measurementHandler = handler;
        uBit.messageBus.listen(EVENT_SBRICK_ADC, MICROBIT_EVT_ANY, _onMeasurement, MESSAGE_BUS_LISTENER_DROP_IF_BUSY);
    }

    //% blockId=sbrick_measured_value
    //% block="measured value"
    int measuredValue()
    {
        return _measuredValue;
    }

    //% blockId=sbrick_measured_port_is
    //% block="measured port is|%p"
    bool measuredPortIs(SBPort p)
    {
        return _measuredPort == p;
    }
}
