/**
* Provides blocks to send events to SBricks
* A micro:bit event value contains an SBrick command and parameters.
* The most significant 4 bit is the command, the rest are the parameters.
*
* Events sent to SBrick:
*
* Brake:       EVENT_SBRICK_CMD | [4: 0] [3: Port] [9: n/a ]
* Drive:       EVENT_SBRICK_CMD | [4: 1] [3: Port] [1: direction] [8: power]
* Set m:       EVENT_SBRICK_CMD | [4: 2] [4: Channel] [8: compensation profile]
* Clr m:       EVENT_SBRICK_CMD | [4: 3] [4: Channel] [8: n/a]
* Send signal: EVENT_SBRICK_CMD | [4: 4] [4: Port] [8: duty cycle]
*
* Events received from SBrick:
*
* Connection established:  EVENT_SBRICK_RSP | [4: 0] [12: n/a]
* Signal complete:         EVENT_SBRICK_RSP | [4: 1] [12: n/a]
* 
*/

//% color=#e84a24 weight=100 icon="\uf11b" block="SBrick"
namespace sbrick {

    //% blockId=sbrick_connect
    //% block="connect to SBrick|%n"
    //shim=sbrick::connect
    export function connect(n: string): void
    {
       n = n.trim();

       let append = 7 - n.length;

        for (let i = 0; i < append; i++) {
            n = n + " ";
        }

        let hash = 0
        
        for (let i = 0; i < n.length; i++) {
            hash = (hash << 5) - hash + n.charCodeAt(i) | 0;
        }

        bluetooth.setTransmitPower(7)
        bluetooth.advertiseUid(0x0198, hash, 7, true)
    }

    //% blockId=sbrick_on_connected
    //% block="on sbrick connected" shim=sbrick::onConnected
    export function onConnected(handler: () => void): void
    {
        return
    }

    //% blockId=sbrick_drive
    //% block="drive with|power %power|on port %port|in the direction: %direction" shim=sbrick::drive
    export function drive(power: number, port: SBPort, direction: SBDirection): void
    { 
        return
    }

    //% blockId=sbrick_lightmono
    //% block="light on with |power %power|on channel %c" shim=sbrick::lightmono
    // export function lightMono(power: number, c: number): void
    // { 
    //     return
    // }

    //% blockId=sbrick_lightrgb
    //% block="RGB light on with |red %red|green %green|blue %blue|on port %p" shim=sbrick::lightrgb
    // export function lightRGB(red: number, green: number, blue: number, p: number): void
    // { 
    //     return
    // }

    //% blockId=sbrick_drive_from_accel
    //% block="drive |port %p|with acceleration read across dimension %d" shim=sbrick::driveFromAccel
    // export function driveFromAccel(p: SBPort, d: SBDimension): void
    // {
    //     return
    // }

    //% blockId=sbrick_brake
    //% block="brake on|port %p" shim=sbrick::brake
    export function brake(p: SBPort): void
    {
        return
    }

    //% blockId=sbrick_set_device
    //% block="use device|type %d|on port %p" shim=sbrick::setDevice
    // export function setDevice(d: SBConnectedDevice, p: SBPort): void
    // {
    //     return
    // }

    //% blockId=sbrick_on_measurement
    //% block="on measurement" shim=sbrick::onMeasurement
    // export function onMeasurement(handler: () => void): void
    // {
    //     return
    // }

    //% blockId=sbrick_measured_value
    //% block="measured value" shim=sbrick::measuredValue
    // export function measuredValue(): number
    // {
    //     return 0
    // }

    //% blockId=sbrick_measured_port_is
    //% block="measured port is|%p" shim=sbrick::measuredPortIs
    // export function measuredPortIs(p: SBPort): boolean
    // {
    //     return false
    // }
}
