# Adafruit Trinket M0 watchdog for EVNotiPi
I shared this board on two manufacturers sites:
- https://aisler.net/p/TADOVIMT (shipping from Germany and USA; quicker but more expensive; also can source all parts for assembly by ordering "Precious Parts")
- https://oshpark.com/shared_projects/yOh5dQH3 (shipping from USA; slow)
See hardware/BOM.txt

## Building
You will need PlatformIO for building and installing the firmware. See https://docs.platformio.org/en/latest/installation.html for installation instructions.

To build and upload attach the Trinket M0 by USB and run ```pio run -t upload```.

# Easy adaptor for connecting the watchdog to the Pi
Plugging in patch cables in top of the CAN hat uses lots of space which the case does not provide. So the original solution was to bend the pins to save some verical space. Now there is a simple adaptor board which saves some millimeters and also allows connecting through a 5 pin ribbon cable.

Available here: https://aisler.net/p/ODQEXOXX


I suggest using a 10cm female-female jumper cable with (at least) 5 pins.

