1. Get the dt-blob-bin file by running `sudo wget https://datasheets.raspberrypi.com/cmio/dt-blob-dualcam.bin -O /boot/dt-blob.bin` in the terminal.

2. Run `sudo nano /boot/firmware/config.txt` in the terminal, scroll down and add the following lines to the bottom of the file:
```
dtoverlay=imx708,cam0
dtoverlay=imx708,cam1
```

3. Run `libcamera-hello` in the terminal to determine if the cameras are detected.

4. Compile OpenCV from source; follow the guide [here](https://qengineering.eu/install-opencv-on-raspberry-64-os.html).

5. Install LCCV by following the instructions [here](https://github.com/8076ducc/lccv).

6. Create symbolic links by running the following commands in the terminal:
```bash
sudo ln -s /usr/local/include/opencv4/opencv2 /usr/local/include/opencv2
sudo ln -s /usr/include/libcamera/libcamera /usr/local/include/libcamera
```

7. Install wiringpi by following the guide [here](https://github.com/WiringPi/WiringPi).

8. In Geany, manage your build settings and configure it to compile via the following command:
```bash
sudo g++ -Wall -o "%e" "%f" -lopencv_core -lopencv_videoio -lopencv_highgui -lopencv_imgproc -lopencv_video -llccv -lwiringPi
```

9. Transfer the two files in the `systemctl` folder in this repository to the `/lib/systemd/system` folder on the Raspberry Pi.

10. Individually check that the services are working using the following commands:
```bash
sudo systemctl start configure-gpio.service
sudo systemctl status configure-gpio.service
sudo systemctl stop configure-gpio.service

sudo systemctl start tracker.service
sudo systemctl status tracker.service
sudo systemctl stop tracker.service
```

11. Enable both services to run on startup using the following commands:
```bash
sudo systemctl enable configure-gpio.service
sudo systemctl enable tracker.service
```

13. Run `sudo systemctl disable NetworkManager-wait-online.service` in the terminal.