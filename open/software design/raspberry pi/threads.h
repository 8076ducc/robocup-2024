#pragma once

#ifndef THREADS_H
#define THREADS_H

cv::Mat unsizedImage;
bool imageStatus;
bool new_orange_frame, new_yellow_frame, new_blue_frame;

int video_scaled = 300;
bool show_debug_windows = false;

double regress(double distance)
{
    return (0.0000001144 * pow(distance, 5)) - (0.0000350263 * pow(distance, 4)) + (0.0042239901 * pow(distance, 3)) - (0.2146899125 * pow(distance, 2)) + (8.3434697519 * distance) - 0.3594513448;
}

class Colour
{
public:
    std::vector<int> threshold;
    int minArea;
    Colour(std::vector<int> ithreshold, int iminArea) : threshold(ithreshold),
                                                        minArea(iminArea) {}
};

void trackColour(int icase)
{
    Colour colour({0}, 0);
    Colour orange({0, 20, 140, 255, 53, 255}, 10);
    // Colour orange({0, 30, 150, 255, 53, 255}, 10);
    Colour yellow({25, 45, 175, 255, 130, 255}, 40);
    // Colour yellow({35, 38, 200, 255, 180, 255}, 40);
    Colour blue({88, 179, 55, 255, 47, 119}, 30);
    // Colour blue({88, 130, 237, 255, 40, 140}, 40);

    // set up kalman stuff
    int state_size = 6;
    int meas_size = 4;
    int contr_size = 0;

    unsigned int type = CV_32F;
    cv::KalmanFilter kf(state_size, meas_size, contr_size, type);

    cv::Mat state(state_size, 1, type); // [x,y,v_x,v_y,w,h]
    cv::Mat meas(meas_size, 1, type);   // [z_x,z_y,z_w,z_h]

    // Transition State Matrix A
    // Note: set dT at each processing step!
    // [ 1 0 dT 0  0 0 ]
    // [ 0 1 0  dT 0 0 ]
    // [ 0 0 1  0  0 0 ]
    // [ 0 0 0  1  0 0 ]
    // [ 0 0 0  0  1 0 ]
    // [ 0 0 0  0  0 1 ]
    cv::setIdentity(kf.transitionMatrix);

    // Measure Matrix H
    // [ 1 0 0 0 0 0 ]
    // [ 0 1 0 0 0 0 ]
    // [ 0 0 0 0 1 0 ]
    // [ 0 0 0 0 0 1 ]
    kf.measurementMatrix = cv::Mat::zeros(meas_size, state_size, type);
    kf.measurementMatrix.at<float>(0) = 1.0f;
    kf.measurementMatrix.at<float>(7) = 1.0f;
    kf.measurementMatrix.at<float>(16) = 1.0f;
    kf.measurementMatrix.at<float>(23) = 1.0f;

    // Process Noise Covariance Matrix Q
    // [ Ex   0   0     0     0    0  ]
    // [ 0    Ey  0     0     0    0  ]
    // [ 0    0   Ev_x  0     0    0  ]
    // [ 0    0   0     Ev_y  0    0  ]
    // [ 0    0   0     0     Ew   0  ]
    // [ 0    0   0     0     0    Eh ]
    kf.processNoiseCov = cv::Mat::zeros(state_size, state_size, type);
    kf.processNoiseCov.at<float>(0) = 1e-2;
    kf.processNoiseCov.at<float>(7) = 1e-2;
    kf.processNoiseCov.at<float>(14) = 5.0f;
    kf.processNoiseCov.at<float>(21) = 5.0f;
    kf.processNoiseCov.at<float>(28) = 1e-2;
    kf.processNoiseCov.at<float>(35) = 1e-2;

    // Measures Noise Covariance Matrix R
    cv::setIdentity(kf.measurementNoiseCov, cv::Scalar(1e-1));

    switch (icase)
    {
    case 0:
        colour = orange;
        break;
    case 1:
        colour = yellow;
        break;
    case 2:
        colour = blue;
        break;
    }

    std::string window_name;

    if (show_debug_windows)
    {
        switch (icase)
        {
        case 0:
            window_name = "orange control";
            break;
        case 1:
            window_name = "yellow control";
            break;
        case 2:
            window_name = "blue control";
            break;
        }

        cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);

        // Create trackbars in "Control" window
        cv::createTrackbar("LowH", window_name, &colour.threshold[0], 179); // Hue (0 - 179)
        cv::createTrackbar("HighH", window_name, &colour.threshold[1], 179);

        cv::createTrackbar("LowS", window_name, &colour.threshold[2], 255); // Saturation (0 - 255)
        cv::createTrackbar("HighS", window_name, &colour.threshold[3], 255);

        cv::createTrackbar("LowV", window_name, &colour.threshold[4], 255); // Value (0 - 255)
        cv::createTrackbar("HighV", window_name, &colour.threshold[5], 255);
    }

    double fps = 0;
    double ticks = 0;
    bool found = false;

    int notFoundCount = 0;

    while (true)
    {
        bool run_thread;
        switch (icase)
        {
        case 0:
            run_thread = rx_data.track_orange;
            break;
        case 1:
            run_thread = rx_data.track_yellow;
            break;
        case 2:
            run_thread = rx_data.track_blue;
            break;
        }

        if (run_thread)
        {
            auto tStartSteady = std::chrono::steady_clock::now();

            bool new_image;

            switch (icase)
            {
            case 0:
                new_image = new_orange_frame;
                break;
            case 1:
                new_image = new_yellow_frame;
                break;
            case 2:
                new_image = new_blue_frame;
                break;
            }

            if (imageStatus && new_image)
            {
                double precTick = ticks;
                ticks = (double)cv::getTickCount();
                double dT = (ticks - precTick) / cv::getTickFrequency(); // seconds

                cv::Mat image;
                cv::resize(unsizedImage, image, cv::Size(video_scaled, video_scaled), cv::INTER_LINEAR);
                cv::flip(image, image, 0);
                cv::GaussianBlur(image, image, cv::Size(5, 5), 0);

                // draw contours on the original image
                cv::Mat image_copy = image.clone();

                if (found)
                {
                    // >>>> Matrix A
                    kf.transitionMatrix.at<float>(2) = dT;
                    kf.transitionMatrix.at<float>(9) = dT;
                    // <<<< Matrix A

                    // std::cout << "dT:" << std::endl << dT << std::endl;

                    state = kf.predict();
                    // std::cout << "State post:" << std::endl << state << std::endl;

                    cv::Rect predRect;
                    predRect.width = state.at<float>(4);
                    predRect.height = state.at<float>(5);
                    predRect.x = state.at<float>(0) - predRect.width / 2;
                    predRect.y = state.at<float>(1) - predRect.height / 2;

                    cv::Point center;
                    center.x = state.at<float>(0);
                    center.y = state.at<float>(1);

                    center.x -= video_scaled / 2;
                    center.y = video_scaled / 2 - center.y;

                    double distance = regress(std::sqrt(pow(center.x, 2) + pow(center.y, 2)));
                    ;

                    switch (icase)
                    {
                    case 0:
                        tx_data.data.ball_detected = true;
                        tx_data.data.ball_x = sin(atan2(center.x, center.y)) * distance;
                        tx_data.data.ball_y = cos(atan2(center.x, center.y)) * distance;
                        // std::cout << "ball " << sqrt(pow(tx_data.data.ball_x, 2) + pow(tx_data.data.ball_y, 2)) << " " << regress(sqrt(pow(tx_data.data.ball_x, 2) + pow(tx_data.data.ball_y, 2))) << std::endl;
                        break;
                    case 1:
                        tx_data.data.yellow_goal_detected = true;
                        tx_data.data.yellow_goal_x = sin(atan2(center.x, center.y)) * distance;
                        tx_data.data.yellow_goal_y = cos(atan2(center.x, center.y)) * distance;
                        // std::cout << "yellow " << distance << " " << tx_data.data.yellow_goal_y  << " " << tx_data.data.yellow_goal_yr << " ";
                        break;
                    case 2:
                        tx_data.data.blue_goal_detected = true;
                        tx_data.data.blue_goal_x = sin(atan2(center.x, center.y)) * distance;
                        tx_data.data.blue_goal_y = cos(atan2(center.x, center.y)) * distance;
                        // std::cout << "blue " << distance << " " << tx_data.data.blue_goal_y  << " " << tx_data.data.blue_goal_yr << std::endl;
                        break;
                    }

                    if (show_debug_windows)
                    {
                        cv::rectangle(image_copy, predRect, CV_RGB(255, 0, 0), 2);
                    }
                }
                else
                {
                    switch (icase)
                    {
                    case 0:
                        tx_data.data.ball_detected = false;
                        break;
                    case 1:
                        tx_data.data.yellow_goal_detected = false;
                        break;
                    case 2:
                        tx_data.data.blue_goal_detected = false;
                        break;
                    }
                }

                cv::Mat imgHSV;
                cv::cvtColor(image, imgHSV, cv::COLOR_BGR2HSV); // Convert the captured frame from BGR to HSV

                cv::Mat imgThresholded;

                cv::inRange(imgHSV, cv::Scalar(colour.threshold[0], colour.threshold[2], colour.threshold[4]), cv::Scalar(colour.threshold[1], colour.threshold[3], colour.threshold[5]), imgThresholded); // Threshold the image

                // morphological opening (remove small objects from the foreground)
                // cv::erode(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
                cv::dilate(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

                // morphological closing (fill small holes in the foreground)
                // cv::dilate(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
                // cv::erode(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

                if (show_debug_windows)
                {
                    switch (icase)
                    {
                    case 0:
                        window_name = "orange thresholded";
                        break;
                    case 1:
                        window_name = "yellow thresholded";
                        break;
                    case 2:
                        window_name = "blue thresholded";
                        break;
                    }

                    cv::imshow(window_name, imgThresholded); // show the thresholded image
                }

                // cv::threshold(imgHSV, imgThresholded, 150, 255, cv::THRESH_BINARY);
                std::vector<std::vector<cv::Point>> contours;
                std::vector<cv::Vec4i> hierarchy;
                std::vector<cv::Point> convexList;
                std::vector<cv::Point> hull;
                cv::findContours(imgThresholded, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);
                // cv::drawContours(image_copy, contours, -1, cv::Scalar(0, 255, 0), 2);
                cv::Rect rect;
                cv::Point obj_start, obj_end;
                cv::Point blue_open_start, blue_open_end;
                int blue_max_area = 0;
                cv::Point yellow_open_start, yellow_open_end;
                int yellow_max_area = 0;

                cv::circle(image_copy, cv::Point(150, 150), 2, CV_RGB(255, 0, 0), -1);

                for (int i = 0; i < contours.size(); i++)
                {
                    cv::Rect rect = cv::boundingRect(contours[i]);
                    cv::Point p1, p2;
                    p1.x = rect.x;
                    p1.y = rect.y;
                    p2.x = rect.x + rect.width;
                    p2.y = rect.y + rect.height;
                    if (cv::contourArea(contours[i]) > colour.minArea)
                    {
                        convexList.push_back(p1);
                        convexList.push_back(p2);
                    }
                    switch (icase)
                    {
                    case 0:
                        break;

                    case 1:
                        if (cv::contourArea(contours[i]) > yellow_max_area)
                        {
                            yellow_max_area = cv::contourArea(contours[i]);
                            yellow_open_start = p1;
                            yellow_open_end = p2;
                            tx_data.data.yellow_open_x = (yellow_open_start.x + yellow_open_end.x) / 2;
                            tx_data.data.yellow_open_y = (yellow_open_start.y + yellow_open_end.y) / 2;
                        }
                        break;

                    case 2:
                        if (cv::contourArea(contours[i]) > blue_max_area)
                        {
                            blue_max_area = cv::contourArea(contours[i]);
                            blue_open_start = p1;
                            blue_open_end = p2;
                            tx_data.data.blue_open_x = (blue_open_start.x + blue_open_end.x) / 2;
                            tx_data.data.blue_open_y = (blue_open_start.y + blue_open_end.y) / 2;
                        }
                        break;
                    }
                }

                for (int i = 0; i < convexList.size(); i += 2)
                {
                    if (obj_start.x == 0 || convexList[i].x < obj_start.x)
                        if (obj_start.x == 0 || convexList[i].x < obj_start.x)
                        {
                            obj_start.x = convexList[i].x;
                        }

                    if (obj_start.y == 0 || convexList[i].y < obj_start.y)
                    {
                        obj_start.y = convexList[i].y;
                    }

                    if (obj_end.x == 0 || convexList[i + 1].x > obj_end.x)
                    {
                        obj_end.x = convexList[i + 1].x;
                    }

                    if (obj_end.y == 0 || convexList[i + 1].y > obj_end.y)
                    {
                        obj_end.y = convexList[i + 1].y;
                    }
                }

                if (convexList.size() <= 0)
                {
                    notFoundCount++;
                    // std::cout << "notFoundCount:" << notFoundCount << std::endl;
                    if (notFoundCount >= 100)
                    {
                        found = false;
                    }
                }
                else
                {
                    notFoundCount = 0;

                    meas.at<float>(0) = obj_start.x + (obj_end.x - obj_start.x) / 2;
                    meas.at<float>(1) = obj_start.y + (obj_end.y - obj_start.y) / 2;
                    meas.at<float>(2) = (float)(obj_end.x - obj_start.x);
                    meas.at<float>(3) = (float)(obj_end.y - obj_start.y);

                    if (!found) // First detection!
                    {
                        // >>>> Initialization
                        kf.errorCovPre.at<float>(0) = 1; // px
                        kf.errorCovPre.at<float>(7) = 1; // px
                        kf.errorCovPre.at<float>(14) = 1;
                        kf.errorCovPre.at<float>(21) = 1;
                        kf.errorCovPre.at<float>(28) = 1; // px
                        kf.errorCovPre.at<float>(35) = 1; // px

                        state.at<float>(0) = meas.at<float>(0);
                        state.at<float>(1) = meas.at<float>(1);
                        state.at<float>(2) = 0;
                        state.at<float>(3) = 0;
                        state.at<float>(4) = meas.at<float>(2);
                        state.at<float>(5) = meas.at<float>(3);
                        // <<<< Initialization

                        kf.statePost = state;

                        found = true;
                    }
                    else
                        kf.correct(meas); // Kalman Correction

                    // std::cout << "Measure matrix:" << std::endl << meas << std::endl;
                    if (show_debug_windows)
                    {
                        cv::rectangle(image_copy, obj_start, obj_end, cv::Scalar(0, 255, 0), 1);
                        if (yellow_max_area)
                        {
                            cv::rectangle(image_copy, yellow_open_start, yellow_open_end, cv::Scalar(0, 255, 255), 1);
                        }
                        if (blue_max_area)
                        {
                            cv::rectangle(image_copy, blue_open_start, blue_open_end, cv::Scalar(255, 0, 0), 1);
                        }
                    }
                }
                // <<<<< Kalman Update

                if (show_debug_windows)
                {
                    switch (icase)
                    {
                    case 0:
                        window_name = "orange final";
                        break;
                    case 1:
                        window_name = "yellow final";
                        break;
                    case 2:
                        window_name = "blue final";
                        break;
                    }

                    cv::imshow(window_name, image_copy);
                }

                cv::waitKey(1);
            }
            else
            {
                // std::cout << "Timeout error" << std::endl;
            }

            switch (icase)
            {
            case 0:
                new_orange_frame = false;
                break;
            case 1:
                new_yellow_frame = false;
                break;
            case 2:
                new_blue_frame = false;
                break;
            }
        }
    }
}

#endif
