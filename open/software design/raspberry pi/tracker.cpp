#include <lccv.hpp>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <vector>
#include <unistd.h>
#define byte uint8_t

#include "serial.h"

int video_x = 1000;
int video_y = 1000;

CamRxDataUnion rx_data;
CamTxDataUnion tx_data;

int main()
{
    setUpSerial();
    cv::Mat unsizedImage;
    lccv::PiCamera cam;
    cam.options->video_width = video_x;
    cam.options->video_height = video_y;
    cam.options->framerate = 120;
    cam.options->verbose = true;

    cv::namedWindow("Control", cv::WINDOW_AUTOSIZE);
    int iLowH = 0;
    int iHighH = 15;

    int iLowS = 162;
    int iHighS = 255;

    int iLowV = 100;
    int iHighV = 255;

    int minArea = 30;

    // Create trackbars in "Control" window
    cv::createTrackbar("LowH", "Control", &iLowH, 179); // Hue (0 - 179)
    cv::createTrackbar("HighH", "Control", &iHighH, 179);

    cv::createTrackbar("LowS", "Control", &iLowS, 255); // Saturation (0 - 255)
    cv::createTrackbar("HighS", "Control", &iHighS, 255);

    cv::createTrackbar("LowV", "Control", &iLowV, 255); // Value (0 - 255)
    cv::createTrackbar("HighV", "Control", &iHighV, 255);

    cam.startVideo();
    int ch = 0;
    double fps = 0;
    while (ch != 27)
    {
        auto tStartSteady = std::chrono::steady_clock::now();
        if (!cam.getVideoFrame(unsizedImage, 1000))
        {
            std::cout << "Timeout error" << std::endl;
        }
        else
        {
            cv::Mat image;
            cv::resize(unsizedImage, image, cv::Size(300, 300), cv::INTER_LINEAR);
            cv::Mat imgHSV;

            cv::cvtColor(image, imgHSV, cv::COLOR_BGR2HSV); // Convert the captured frame from BGR to HSV

            cv::Mat imgThresholded;

            cv::inRange(imgHSV, cv::Scalar(iLowH, iLowS, iLowV), cv::Scalar(iHighH, iHighS, iHighV), imgThresholded); // Threshold the image

            // morphological opening (remove small objects from the foreground)
            cv::erode(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
            cv::dilate(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

            // morphological closing (fill small holes in the foreground)
            cv::dilate(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
            cv::erode(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

            // imshow("Thresholded Image", imgThresholded); // show the thresholded image
            // cv::imshow("Original", image);                   // show the original image
            // cv::imshow("Video",image);

            // threshold(imgHSV, imgThresholded, 150, 255, THRESH_BINARY);
            std::vector<std::vector<cv::Point>> contours;
            std::vector<cv::Vec4i> hierarchy;
            std::vector<cv::Point> convexList;
            std::vector<cv::Point> hull;
            cv::findContours(imgThresholded, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);
            // draw contours on the original image
            cv::Mat image_copy = image.clone();
            // drawContours(image_copy, contours, -1, Scalar(0, 255, 0), 2);
            int area = 0;
            int idx;
            for (int i = 0; i < contours.size(); i++)
            {
                // if (area < contours[i].size())
                //{
                // idx = i;
                //}
                if (contours.size() > 0)
                {
                    cv::Rect rect = cv::boundingRect(contours[i]);
                    cv::Point p1, p2;
                    p1.x = rect.x;
                    p1.y = rect.y;
                    // if (rect.width > rect.height) {
                    //  rect.height = rect.width;
                    // } else if (rect.height > rect.width) {
                    //  rect.width = rect.height;
                    //}
                    p2.x = rect.x + rect.width;
                    p2.y = rect.y + rect.height;
                    if (cv::contourArea(contours[i]) > minArea)
                    {
                        convexList.push_back(p1);
                        convexList.push_back(p2);
                        // cam_1_tx_data.data.ball_detected = true;
                    }
                    else
                    {
                        // cam_1_tx_data.data.ball_detected = false;
                    }
                    cv::rectangle(image_copy, p1, p2, cv::Scalar(0, 255, 0), 1);
                }
                else
                {
                    // cam_1_tx_data.data.ball_detected = false;
                }
            }

            // if (contours.size() > 0) {
            // cv::Rect rect = cv::boundingRect(contours[idx]);
            // cv::Point p1, p2;
            // p1.x = rect.x;
            // p1.y = rect.y;
            // if (rect.width > rect.height)
            //{
            // rect.height = rect.width;
            //} else if (rect.height > rect.width)
            //{
            //  rect.width = rect.height;
            //}
            // p2.x = rect.x + rect.width;
            // p2.y = rect.y + rect.height;

            // double ball_x = 0, ball_y = 0;

            // std::cout << p1.x << " " << p1.y << " " << p2.x << " " << p2.y << std::endl;

            // ball_x = p1.x + rect.width / 2 - video_x/ 2;
            // ball_y = p1.y + rect.height / 2 - video_y / 2;

            // std::cout << ball_x << " " << ball_y << " " << std::endl;
            // cv::rectangle(image_copy, p1, p2, cv::Scalar(0, 255, 0), 1);
            //}
            // if (convexList.size() > 0) {
            // cv::convexHull(convexList, hull); }

            cv::Point ball_start, ball_end;
            for (int i = 0; i < convexList.size(); i += 2)
            {
                if (ball_start.x == 0 || convexList[i].x < ball_start.x)
                {
                    ball_start.x = convexList[i].x;
                }

                if (ball_start.y == 0 || convexList[i].y < ball_start.y)
                {
                    ball_start.y = convexList[i].y;
                }

                if (ball_end.x == 0 || convexList[i + 1].x > ball_end.x)
                {
                    ball_end.x = convexList[i + 1].x;
                }

                if (ball_end.y == 0 || convexList[i + 1].y > ball_end.y)
                {
                    ball_end.y = convexList[i + 1].y;
                }
            }
            tx_data.data.ball_pose_x = (ball_start.x + ball_end.x) / 2;
            tx_data.data.ball_pose_y = (ball_start.y + ball_end.y) / 2;

            cv::rectangle(image_copy, ball_start, ball_end, cv::Scalar(255, 0, 0), 1);

            // cv::imshow("None approximation", image_copy);

            ch = cv::waitKey(1);

            std::cout << tx_data.data.ball_pose_x << " " << tx_data.data.ball_pose_y << " ";
        }
        auto tEndSteady = std::chrono::steady_clock::now();
        std::chrono::nanoseconds diff = tEndSteady - tStartSteady;
        fps = 0.9 * fps + 0.1 * (1000000000 / diff.count());
        std::cout << fps << std::endl;

        tx_data.data.ball_detected = true;

        serialWrite(tx_data.bytes, sizeof(tx_data.bytes));
    }
    cam.stopVideo();
    cv::destroyAllWindows();
}
