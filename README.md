# MULTI-BootReloader
MULTI-BootReloader is a special MULTI-Module firmware application will upgrade / replace the bootloader on a MULTI-Module.

It is intended for use with an iRangeX IRX4 / IRX4+, Vantac MPM, or URUAV TMX5 MULTI-Module, as these modules are known to ship with old or partially working bootloaders.

Until now the only way to replace the bootloader on these modules was by using a USB-to-Serial adapter to flash new firmware with [Flash Multi](https://github.com/benlye/flash-multi).

**Note:** There is absolutely no need to use this process if your module's bootloader is working correctly.

## How it Works
The MULTI-BootReloader app is written to the flash memory of the MULTI-Module, replacing the standard firmware.  Once written, it will run one time and will replace the existing bootloader with the new version.  It will erase itself after it has run, ensuring that the next time the module is started it remains in DFU mode, ready to receive firmware.

## Prerequisites
Your MULTI-Module must have a bootloader to start with - the module must be able to be updated from the computer using the USB port.

If your module does not have a bootloader at all your will need to use a USB-to-Serial adapter to flash the bootloader.

## Preparation

### Read the instructions
Please read the instructions all the way through before starting.  Although the process is simple, it will help you to know what to expect.

### Download MultiBootReloader
1. Download the Multi-BootReloader-[version] file from the [latest release](https://github.com/benlye/MultiBootReloader/releases/latest)
1. Unzip the downloaded file to a convenient location

### Download Flash Multi
In order to write the BootReloader app to the MULTI-Module you will need the appropriate drivers installed.  The easiest way get the drivers is to download the [latest Flash Multi release](https://github.com/benlye/flash-multi/releases/latest) and install them using the [driver installation script](https://github.com/benlye/flash-multi#additional-drivers).

You will also need Flash Multi to write the MULTI firmware back to the module after the bootloader is updated.

### Download the latest firmware for your MULTI-Module
The MULTI-Module will no longer contain firmware after the BootReloader app has been used and will need new firmware installed.

Download the latest firmware for your module from https://downloads.multi-module.org/.

## Instructions
1. Plug in your MULTI-Module and make a note of the COM port which appears in **Device Manager**
1. Open a **Command Prompt**
   Click **Start** -> **Windows System** -> **Command Prompt**
1. Change to the directory where you unzipped the MULTI-BootReloader zip file

   `CD C:\Temp\Multi-BootReloader-0.1.0\`
   
1. Run `Upgrade-Multi-Bootloader.bat` specifying the COM port noted in step 1

   `Upgrade-Multi-Bootloader.bat COM5`
   
1. When prompted, press a key to continue (or press **Ctrl-C** to abort)

Once the BootReloader application has been written to the module it will begin the bootloader upgrade.  The red LED will indicate progress.  

The red LED will:
* Go on for ~2sec - upgrade is about to start
* Blink very rapidly for ~2sec - bootloader is being written
* Go on for ~1sec - bootloader write is complete
* Go off - BootReloader application is done

**DO NOT UNPLUG THE MULTI-MODULE UNTIL THE RED LED GOES OFF**

Once the LED has gone off, and remains off for more than 5sec, the module can be unplugged.

**Note:** It is safe to unplug the module if the red LED remains on solid for more than 30s after the update.  This has been reported to happen on TMX5 MULTI-Modules and is being investigated.

When the module is unplugged and plugged back in, the red LED will blink continuously.  This indicates that it is in bootloader mode and ready to receive firmware.  Use Flash Multi or your radio to write new firmware to the module.

## Video
This video shows the BootReloader working, including the state of the MULTI-Module as it is updated.

[![Video of BootReloader doing its thing.](http://img.youtube.com/vi/wrGWF0qd2Xo/0.jpg)](http://www.youtube.com/watch?v=wrGWF0qd2Xo "MULTI-BootlReloader")

## Troubleshooting
If the process goes wrong there is a chance that the bootloader will become corrupt.  If this happens the module will be unusable until you use a USB-to-Serial adapter (e.g. CP2102 or CH340G) to reprogram the it.  Depending on your MULTI-Module, you may have to solder pins onto it in order to do this.

Effectively you will need to take the same steps as if you were upgrading the bootloader the 'old' way.

### Red LED remains on
It has been reported that on some MULTI-Modules (e.g. URUAV TMX5) the red LED remains on at the end of the flashing process.  If the red LED has been on solid (not blinking) for 30sec or more, it is safe to unplug the module and continue with flashing the firmware.
