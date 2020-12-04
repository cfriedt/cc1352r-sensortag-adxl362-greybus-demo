SPIDev Demo for CC1352R SensorTag / ADXL362 Accelerometer

This code demonstrates taking accelerometer readings from the ADXL362 sensor on the CC1352R SensorTag.

Please use the 'greybus-service-subg-v2' branch when building the 'samples/subsys/greybus/net' application.

Also, ensure that the 'spidev' Linux kernel module is loaded.

To set up the network, please follow the previous instructions on setting up the CC1352R1 with Zephyr, Greybus,
and Linux over IEEE 802.15.4.

After the Greybus Manifest has been transferred from Zephyr to the Linux kernel and after the Linux kernel
has processed the manifest, you should see a '/dev/spidev0.0' device node appear. That device node represents
the ADXL362 accelerometer on the CC1352R SensorTag, but it is accessible as if it were a SPI device directly
connected to the Linux host.

This program actually borrows the Zephyr driver and compiles it for use from Linux userspace using the existing
Linux spidev API.

To build the code, simply run make.

To run the code, simply run './adxl362'.

To change configuration options, edit 'config.h' and 'Makefile' appropriately.

Example output:
```
(x,y,z,°C) =   -0.823758,   0.147099,  11.169774, -28.990000
(x,y,z,°C) =   -0.823758,   0.147099,  11.169774, -28.990000
(x,y,z,°C) =   -0.804145,   0.156906,  11.140354, -28.990000
(x,y,z,°C) =   -0.804145,   0.156906,  11.140354, -28.990000
(x,y,z,°C) =   -0.804145,   0.156906,  11.140354, -28.990000
(x,y,z,°C) =   -0.823758,   0.117679,  11.140354, -28.925000
```

Note: some of the headers / source files in this project were borrowed from [Zephyr](https://github.com/zephyrproject-rtos/zephyr)
