#include <lccv.hpp>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <vector>

using namespace lccv;
using namespace cv;
using namespace std::chrono;
using namespace std;

int main()
{

    std::cout << "Sample program for LCCV video capture" << std::endl;
    std::cout << "Press ESC to stop." << std::endl;
    cv::Mat image;
    lccv::PiCamera cam;
    cam.options->video_width = 480;
    cam.options->video_height = 270;
    cam.options->framerate = 120;
    cam.options->verbose = true;
    // cv::namedWindow("Video", cv::WINDOW_NORMAL);

    namedWindow("Control", WINDOW_AUTOSIZE); // create a window called "Control"
    int iLowH = 0;
    int iHighH = 15;

    int iLowS = 162;
    int iHighS = 255;

    int iLowV = 100;
    int iHighV = 255;

    // // Create trackbars in "Control" window
    // createTrackbar("LowH", "Control", &iLowH, 179); // Hue (0 - 179)
    // createTrackbar("HighH", "Control", &iHighH, 179);

    // createTrackbar("LowS", "Control", &iLowS, 255); // Saturation (0 - 255)
    // createTrackbar("HighS", "Control", &iHighS, 255);

    // createTrackbar("LowV", "Control", &iLowV, 255); // Value (0 - 255)
    // createTrackbar("HighV", "Control", &iHighV, 255);

    cam.startVideo();
    int ch = 0;
    while (ch != 27)
    {
        auto tStartSteady = std::chrono::steady_clock::now();
        if (!cam.getVideoFrame(image, 1000))
        {
            std::cout << "Timeout error" << std::endl;
        }
        else
        {
            Mat imgHSV;

            cvtColor(image, imgHSV, COLOR_BGR2HSV); // Convert the captured frame from BGR to HSV

            Mat imgThresholded;

            inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); // Threshold the image

            // morphological opening (remove small objects from the foreground)
            erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
            dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

            // morphological closing (fill small holes in the foreground)
            dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
            erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

            //imshow("Thresholded Image", imgThresholded); // show the thresholded image
            //imshow("Original", image);                   // show the original image
            //cv::imshow("Video",image);
            
            // threshold(imgHSV, imgThresholded, 150, 255, THRESH_BINARY);
            vector<vector<Point>> contours;
            vector<Vec4i> hierarchy;
            findContours(imgThresholded, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);
            // draw contours on the original image
            Mat image_copy = image.clone();
            //drawContours(image_copy, contours, -1, Scalar(0, 255, 0), 2);
            int area = 0;
            int idx;
            for (int i = 0; i < contours.size(); i++) {
                if (area < contours[i].size()) {
                    idx = i;
                }
            }
            Rect rect = boundingRect(contours[idx]);
            Point p1, p2;
            p1.x = rect.x;
            p1.y = rect.y;
            p2.x = rect.x + rect.width;
            p2.y = rect.y + rect.height;
            rectangle(image_copy, p1, p2, Scalar(0, 255, 0), 1);
            imshow("None approximation", image_copy);

            ch = cv::waitKey(1);
        }
        auto tEndSteady = std::chrono::steady_clock::now();
        nanoseconds diff = tEndSteady - tStartSteady;
        std::cout << (1000000000 / diff.count()) << std::endl;
    }
    cam.stopVideo();
    cv::destroyWindow("Video");
}
