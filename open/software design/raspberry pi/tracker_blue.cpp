#include <lccv.hpp>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <vector>
#include <unistd.h>
#define byte uint8_t

#include "serial.h"
#include "threads.h"

int video_x = 1000;
int video_y = 1000;
lccv::PiCamera cam;

bool tune_orange = false;
bool tune_yellow = false;
bool tune_blue = true;

void getNewImage()
{
    double fps = 0;

    while (true)
    {
        auto tStartSteady = std::chrono::steady_clock::now();

        imageStatus = cam.getVideoFrame(unsizedImage, 1000);
        new_orange_frame = true;
        new_yellow_frame = true;
        new_blue_frame = true;
        
        auto tEndSteady = std::chrono::steady_clock::now();
        std::chrono::nanoseconds diff = tEndSteady - tStartSteady;
        fps = 0.9 * fps + 0.1 * (1000000000 / diff.count());
        std::cout << fps << std::endl;
    }
}

void transmitData()
{
    while (true)
    {
        serialWrite(tx_data.bytes, sizeof(tx_data.bytes));
    }
}

void receiveData()
{	
    while (true)
    {
        serialRead();
        
        if (tune_orange) {
			rx_data.track_orange = true;
		} else {
			rx_data.track_orange = false;
		}
		
		if (tune_yellow) {
			rx_data.track_yellow = true;
		} else {
			rx_data.track_yellow = false;
		}
		
		if (tune_blue) {
			rx_data.track_blue = true;
		} else {
			rx_data.track_blue = false;
		}
    }
}

int main()
{
	show_debug_windows = true;
    setUpSerial();
    cam.options->video_width = video_x;
    cam.options->video_height = video_y;
    cam.options->framerate = 120;
    cam.options->verbose = true;
    // cam.options->af_index = AutoFocus_Modes::AF_MANUAL;
    cam.options->lens_position = 100.0f;

    cam.startVideo();
    imageStatus = cam.getVideoFrame(unsizedImage, 1000);
    new_orange_frame = false;
    new_yellow_frame = false;
    new_blue_frame = false;

    while (true)
    {
        //std::thread trackOrange(trackColour, 0);
        //std::thread trackYellow(trackColour, 1);
        std::thread trackBlue(trackColour, 2);
        std::thread getImage(getNewImage);
        std::thread transmit(transmitData);
        std::thread receive(receiveData);

        //trackOrange.join();
        //trackYellow.join();
        trackBlue.join();
        getImage.join();
        transmit.join();
        receive.join();
    }

    cam.stopVideo();
    cv::destroyAllWindows();
}
