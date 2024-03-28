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

void getNewImage()
{
    double fps = 0;

    while (true)
    {
        auto tStartSteady = std::chrono::steady_clock::now();

        imageStatus = cam.getVideoFrame(unsizedImage, 1000);
        newFrame = 0;
        // std::cout << "image pulled" << std::endl;
        serialWrite(tx_data.bytes, sizeof(tx_data.bytes));
        auto tEndSteady = std::chrono::steady_clock::now();
        std::chrono::nanoseconds diff = tEndSteady - tStartSteady;
        fps = 0.9 * fps + 0.1 * (1000000000 / diff.count());
        std::cout << fps << std::endl;
    }
}

int main()
{
    setUpSerial();
    // lccv::PiCamera cam;
    cam.options->video_width = video_x;
    cam.options->video_height = video_y;
    cam.options->framerate = 120;
    cam.options->verbose = true;
    // cam.options->af_index = AutoFocus_Modes::AF_MANUAL;
    cam.options->lens_position = 100.0f;

    cam.startVideo();
    imageStatus = cam.getVideoFrame(unsizedImage, 1000);
    newFrame = 0;

    while (true)
    {
        std::thread trackOrange(trackColour, 0);
        std::thread trackYellow(trackColour, 1);
        std::thread trackBlue(trackColour, 2);
        std::thread getImage(getNewImage);

        trackOrange.join();
        trackYellow.join();
        trackBlue.join();
        getImage.join();
    }
    cam.stopVideo();
    cv::destroyAllWindows();
}
