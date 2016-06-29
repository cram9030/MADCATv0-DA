# MADCATv0-DA
Code base for MADCAT version 0 data aquisition

This project will be using the Beaglebone Black and LSM9DS1 sensor (http://www.st.com/content/ccc/resource/technical/document/datasheet/1e/3f/2a/d6/25/eb/48/46/DM00103319.pdf/files/DM00103319.pdf/jcr:content/translations/en.DM00103319.pdf) to measure tip twist, tip twist rate and wing vibrations on MADCATv0 during flight testing. 

The code to interact with the LSM9DS1 was adapted from https://github.com/sparkfun/SparkFun_LSM9DS1_Arduino_Library
The code Beaglebone SPI and I2C code was based off a combination of https://github.com/derekmolloy/exploringBB and http://www.raspberry-projects.com/pi/programming-in-c/spi/using-the-spi-interface

#Software Setup
In order to use this code the Beaglebone Black Spi ports need to be intialized. This can be done by following these directions https://www.linux.com/learn/how-access-chips-over-spi-beaglebone-black which are also shown bellow. 

root@beaglebone:~# dtc -O dtb -o BB-SPI0-01-00A0.dtbo -b 0 -@ BB-SPI0-01-00A0.dts

root@beaglebone:~# cp BB-SPI0-01-00A0.dtbo /lib/firmware/

root@beaglebone:~# echo BB-SPI0-01 > /sys/devices/bone_capemgr.*/slots

root@beaglebone:~# ls -lh /dev/spi*

crw------- 1 root root 153, 0 Oct 16 04:12 /dev/spidev1.0


The BB-SPI0-00A0.dts is a device tree file that can be found here http://elinux.org/images/1/1f/BB-SPI0-01-00A0.txt. Make sure to rename the extension to .dts. In order to have the SPI ports working at set up add echo BB-SPI0-01 > /sys/devices/bone_capemgr.*/slots" to ~/.profile.

#Hardware Setup

LSM9DS1 Pin | BBB Pin

1           |  3

2           |  22

3           |  3

4           |  18

5           |  21

7           |  17

8           |  3

19          |  1

20          |  1

22          |  3

23          |  3

Pin outs for Beaglebone Black are all from P9 header Beaglebone Black pins 1 and 3 are ground and power respectively and they can simply be refrenced to the same ground if needed.
