# Ostur Firmware

The latest Ostur hardware uses the NRF52 microcontroller and the [Apache Mynewt](https://mynewt.apache.org/) OS for the firmware.

## Building the Firmware

1. Install the newt tool. [Instructions here](https://mynewt.apache.org/os/get_started/get_started/).
2. Install the [GNU Arm Embedded Toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads)
3. Clone this repo `git clone https://github.com/alvarop/ostur.git`
4. Cd into the fw directory (this one) `cd fw/2.0/`
5. Run `newt install` to get the Mynewt libraries
6. Run `newt build nrf52_boot` to build the bootloader
7. Run `newt build ostur_v2p0`

## Loading the Firmware (using an [NRF52DK](https://www.nordicsemi.com/eng/Products/Bluetooth-low-energy/nRF52-DK) as a programmer)
1. Make sure the [JLink tools](https://www.segger.com/downloads/jlink/) are installed
2. Connect the **Debug Out** connector on the NRF52DK (or on any JLink programmer) to the Ostur's SWD connector (on the bottom)
3. Run `newt load nrf52_boot` to load the bootloader
4. Run `newt run ostur_v2p0 0` to load the main firmware and enter a gdb debugging session. Press `c + enter` to start running the code.
