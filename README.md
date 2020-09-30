# Wireless-shifter-pedals
wireless shifter pedals. compatible with unified Sim controller.

## Firmware
Sleep mode occurs after 10 minutes. Pressing any button resets this timer.
Right paddle wakes the device. Because the ESP8266 can only be woken up by reset, an active pin will be needed to signify the device is awake, external circuitry handles the disabling of the reset.
The packet is documented in \PacketInfo.docx
## Power Supply

The charger is handled by tp4056 module