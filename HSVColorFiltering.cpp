#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "HSVRange.h"

void trackbarCallback(int pos, void *userdata) {
    *(static_cast<int *>(userdata)) = pos;
}

void createTrackbars(HSVRange &range) {
    cv::namedWindow("Trackbars");
    cv::createTrackbar("Hue Min", "Trackbars", nullptr, 179, trackbarCallback, &range.getHMin());
    cv::createTrackbar("Hue Max", "Trackbars", nullptr, 179, trackbarCallback, &range.getHMax());
    cv::createTrackbar("Sat Min", "Trackbars", nullptr, 255, trackbarCallback, &range.getSMin());
    cv::createTrackbar("Sat Max", "Trackbars", nullptr, 255, trackbarCallback, &range.getSMax());
    cv::createTrackbar("Val Min", "Trackbars", nullptr, 255, trackbarCallback, &range.getVMin());
    cv::createTrackbar("Val Max", "Trackbars", nullptr, 255, trackbarCallback, &range.getVMax());
}

void applyColorThreshold(const cv::Mat &imgHSV, cv::Mat &mask, HSVRange &range) {
    cv::Scalar lower(range.getHMin(), range.getSMin(), range.getVMin());
    cv::Scalar upper(range.getHMax(), range.getSMax(), range.getVMax());
    cv::inRange(imgHSV, lower, upper, mask);
}

void displayImages(const cv::Mat &img, const cv::Mat &mask, const cv::Mat &imgMasked) {
    cv::Mat resizedMask, resizedImg, resizedMaskedImg;
    cv::resize(mask, resizedMask, cv::Size(), 0.5, 0.5);
    cv::resize(img, resizedImg, cv::Size(), 0.5, 0.5);
    cv::resize(imgMasked, resizedMaskedImg, cv::Size(), 0.5, 0.5);

    cv::imshow("Original", resizedImg);
    cv::imshow("Mask", resizedMask);
    cv::imshow("Masked Image", resizedMaskedImg);
}

void saveImages(const cv::Mat &img, const cv::Mat &mask, const cv::Mat &imgMasked, int &count) {
    std::string frameName = "frame" + std::to_string(count) + ".jpg";
    std::string maskName = "mask" + std::to_string(count) + ".jpg";
    std::string maskedName = "masked" + std::to_string(count) + ".jpg";

    cv::imwrite(frameName, img);
    cv::imwrite(maskName, mask);
    cv::imwrite(maskedName, imgMasked);

    std::cout << "Images saved: " << frameName << ", " << maskName << ", " << maskedName << std::endl;

    count++;
}

void processFrame(cv::Mat &img, HSVRange &range, HSVRange &prevRange, int &count) {
    cv::Mat imgHSV, mask, imgMasked;
    cv::cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);

    applyColorThreshold(imgHSV, mask, range);

    if (range != prevRange) {
        std::cout << "Threshold Values: " << range.getHMin() << "," << range.getSMin() << "," << range.getVMin() << ","
                  << range.getHMax() << "," << range.getSMax() << "," << range.getVMax() << std::endl;
        prevRange = range;
    }

    img.copyTo(imgMasked, mask);

    displayImages(img, mask, imgMasked);

    int key = cv::waitKey(1);
    if (key == 27) {
        exit(0);
    } else if (key == 's' || key == 'S') {
        saveImages(img, mask, imgMasked, count);
    }
}

int main() {
    cv::VideoCapture cap(0, cv::CAP_DSHOW);
    if (!cap.isOpened()) {
        std::cout << "Error opening video stream" << std::endl;
        return -1;
    }

    HSVRange range{0, 179, 0, 255, 0, 255};
    HSVRange prevRange = range;

    createTrackbars(range);

    std::cout << "Press 'ESC' to exit, 'S' to save the current frame" << std::endl;

    cv::Mat frame;
    int imageCount = 0;

    while (true) {
        if (!cap.read(frame)) {
            std::cout << "No frame" << std::endl;
            break;
        }

        processFrame(frame, range, prevRange, imageCount);
    }
}
