# micro:bit SBrick extension

This extension allows the micro:bit to connect to an SBrick.

## How does it work?

The micro:bit can not connect to other BLE devices, i.e. it cannot initiate a
connection. However it happily accepts incoming connections.

An SBrick will connect to a micro:bit if it sends out Eddystone packages with 
the manufacturer's Bluetooth Company Identifier (VENGIT kft. 0x0198) as the 
namespace, and the JavaScript hashCode() of the name of the SBrick as the 
instance. The byte order used is little endian. SBrick listens to such packages 
and decides to initiate a connection accordingly.

Make sure you have the latest firmware on your SBrick.

## Setup

The SBrick extension requires Bluetooth, and therefore conflicts with Radio.

Since SBrick hasn't got a keypad or display, you have to configure your
micro:bit to use JustWorks pairing, or use no pairing at all. This can be done
in the project settings.

## Blocks

You can use the following blocks to interact with the SBrick.

### Connect to SBrick

The single parameter is the name of the SBrick. 

### On SBrick connected

The code inside this block executes when an SBrick is successfully connected.
This is different from a successful Bluetooth connection. After the Bluetooth
connection is successfully built up, SBrick sends a special event, this block
reacts to that event. Any code inside the Bluetooth "on connected" block
executes strictly before this one.

### Drive with the given power on given port & direction

The power parameter should be in the range 0 - 255. Ports A, B, C, or D can be
choosen from the dropdown menu, along with directions Forward and Backward.

### Brake

This command will cause an electrical short circuit on the given port of the
SBrick. If a motor is connected to that port, this will make it hard to turn,
and will act a brake.

## Examples

### Connecting to an SBrick

This example shows how to manage the connection lifecycle with SBrick. The
micro:bit will display different images according to the connection state:

* At startup, it shows a smiley
* When a Bluetooth connection is established, display the letter 'B'
* When the handshake with SBrick is successfully completed, display the letter 'S'
* If the connection is broken, display an 'X'

```blocks
bluetooth.onBluetoothConnected(() => {
    basic.showLeds(`
        . # # . .
        . # . # .
        . # # . .
        . # . # .
        . # # . .
        `)
})

sbrick.onConnected(() => {
    basic.showLeds(`
        . # # # .
        . # . . .
        . . # . .
        . . . # .
        . # # # .
        `)
})

bluetooth.onBluetoothDisconnected(() => {
    basic.showLeds(`
        # . . . #
        . # . # .
        . . # . .
        . # . # .
        # . . . #
        `)
})

basic.showLeds(`
    . # . # .
    . # . # .
    . . . . .
    # . . . #
    . # # # .
    `)

sbrick.connect("SBrick")

```

## Building on Ubuntu 20.04 Focal Fossa

```
npm -g install pxt
npm install
pxt target microbit
pxt build
pxt serve

```

## License

MIT

## Supported targets

* for PXT/microbit
