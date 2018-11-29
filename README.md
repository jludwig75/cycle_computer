# cycle_computer
ESP-32 GPS Cycling Computer with BLE HRM

This project is for a GPS cycling computer using an ESP-32. It will support BLE for connecting to a Bluetooth heart rate monitor.

Componenent List
---
- ESP-WROOM-32 MCU with 64 MB flash
- MTK3329 GPS with PPS output
- MPL3115A2 baramoter/thermometer - Provides refined altitude data and temperature
- MPU9250 accelerometer/magnetometer - Will use compass for possible navigation and could use accelerometer as INS
- OLDE (st7735R) - Display
- P-channel MOSFETs as switches for GPS, barometer, accerlerometer and display
- LiOn Charging circuit

Power
---
The device will always be connected to the battery. When powered down, all devices will be powered down and the ESP-32 will enter its deepest sleep mode from which it can be awakened via an interrupt through a button push. This may require the use of N-channel MOSFET's. I'm not sure if it matters.

Usage
---
The cycling computer will use the GPS for positioning. When powered on, the user can start tracking or stop tracking. When the computer is not tracking, the user will be able to connect it to a WiFi network. By default it will act as a WiFi AP until it has been configured to connect to an AP. The device will provide a web server from which tracks can be downloaded and deleted. The tracks will be stored on the flash of the ESP-WROOM-32.

Future Enhancements
---
- Use the accelerometer to provide INS navigation abilities
- Allow uploading directly to Strava, Garmin Connect, etc. from the devices web interface.
- Add a power output estimate. Using INS navigation will improve this accuracy. It will have to average power over the last N seconds.

Implementation Notes
---
- I plan to use the PPS to synchronize the MCU time. I think this will work by having and interrupt that will keep the time in synch.
- I think it is best to keep a queue of location data points with timestamps and a queue of HRM and barometer readings and then have a logger that uses all data from the queues. I think this may be necessary if the GPS location update times are in the past. I will have to have a timer to read the serial connection from the GPS and queue location updates. I will also want a timer to read the barometric pressure and another to read the hear rate, although I think the Bluetooth connection to the HRM will receive hear rate updates when the HRM sends them. I think I will have several different timings and update intervals for each data type. I think I can get the barometric pressure whenever I want. I think the HRM will send me an update at 1Hz and I can get GPS updates at up to 10Hz. One possibility is to get the barometric pressure reading when I get the heart rate update, but I think I should probably get it in a timer at the same resolution as the GPS updates so I can match them up better. I can interpolate from two data points with timestamps to get heart rate or pressure readings that match the location updates. Either way, I need to somehow match data up as best I can to log to the track file.

Track File
---
I plan to store the tracks to a GPX file. The GPX file will not contain the footer until the track is stopped. Pausing will start a new segment. If the device is powered on, it needs to check for un-terminated GPX track files and terminate them using data from the last track point.

Display
---
The display needs a way to get stats from the last update. It needs to indicate a lost GPS signal. I will need to use big circle math to calculate the distance between points to maintain the total distance traveled.
