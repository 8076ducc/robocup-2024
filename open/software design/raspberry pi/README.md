Get the dt-blob-bin file

Set dtoverlay=imx708,cam0 + dtoverlay=imx708,cam1

Compile OpenCV from source (https://qengineering.eu/install-opencv-on-raspberry-64-os.html).

Get LCCV; follow instructions at https://github.com/neohaojun/lccv

Create symbolic links
sudo ln -s /usr/local/include/opencv4/opencv2 /usr/local/include/opencv2
sudo ln -s /usr/include/libcamera/libcamera /usr/local/include/libcamera

install wiringpi

Set geany compile to sudo g++ -Wall -o "%e" "%f" -lopencv_core -lopencv_videoio -lopencv_highgui -lopencv_imgproc -lopencv_video -llccv -lwiringPi

set 2 services; enable 2 services

sudo systemctl disable NetworkManager-wait-online.service