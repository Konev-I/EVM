#include <opencv2/opencv.hpp>
#include <thread>
#include <atomic>
#include <utility>

#include <csignal>
#include <arkanoid.h>
#include <timer.h>

#include "motion_detector.h"
#include "window.h"
#include "message_queue.h"

using namespace cv;

class CaptureThread: public std::thread
{
public:
    std::atomic<bool> running;

    CaptureThread():
        std::thread(),
        running(true)
    {
        std::thread actual_thread(&CaptureThread::run, this);
        swap(actual_thread);
    }

    void run() {
        cv::VideoCapture capture(0);
        capture.set(CAP_PROP_FRAME_WIDTH, 1300);
        capture.set(CAP_PROP_FRAME_HEIGHT, 720);
        capture.set(CAP_PROP_FPS, 60);

        if (capture.isOpened()) {
        } else {
            running = false;
        }

        while (running) {
            Image frame;
            if (!capture.read(frame)) {
                running = false;
            }

            images->try_push(std::move(frame));
        }
    }

    std::shared_ptr<message_queue<Image>> images = std::make_shared<message_queue<Image>>(3);
};

class DetectorThread: public std::thread
{
public:
    std::atomic<bool> running;

    DetectorThread(size_t width,
                   size_t height,
                   std::shared_ptr<message_queue<Image>>  capture):
        running(true),
        _capture(std::move(capture))
    {
        std::thread actual_thread(&DetectorThread::run, this, width, height);
        swap(actual_thread);
    }

  void run(size_t width,
             size_t height)
    {
        MotionDetector detector(width, height);
        Image background;

        while (running) {
            if (_capture->try_pop(background)) {
                background.flip(Image::FlipAxis::Y);
                detector.nextFrame(background);
            }

            images->try_push(detector.toImage(background));
            marker_positions->try_push(detector.getMarkerPos());
        }
    }

    std::shared_ptr<message_queue<Image>> images = std::make_shared<message_queue<Image>>(3);
    std::shared_ptr<message_queue<cv::Point2f>> marker_positions = std::make_shared<message_queue<cv::Point2f>>(3);

private:
    std::shared_ptr<message_queue<Image>> _capture;
};

int main() {

    Window window("arkanoid");

    const size_t WIDTH = 1300;
    const size_t HEIGHT = 720;

    CaptureThread capture;
    DetectorThread detector(WIDTH, HEIGHT, capture.images);
    Game arkanoid(WIDTH, HEIGHT);

    try {
        char key = 0;

      Timer timer;
        double dt = -3.0;
        const double UPDATE_STEP_S = 1.0 / 60.0;

        Image background(WIDTH, HEIGHT, CV_8UC3);

        while (key != 27) {
            cv::Point2f marker_pos;
            if (detector.marker_positions->try_pop(marker_pos)) {
                arkanoid.setPaddlePos((size_t)marker_pos.x);
            }

            dt += timer.getElapsedSeconds();
            timer.reset();
            while (dt > UPDATE_STEP_S) {
                arkanoid.update((float)UPDATE_STEP_S);
                dt -= UPDATE_STEP_S;
            }

            if (arkanoid.isGameOver() || arkanoid.isGameWon()) {
                arkanoid.reset();
            }

            detector.images->try_pop(background);
            arkanoid.drawOnto(background);
            window.showImage(background);

            key = (char) waitKey(20);
        }
    } catch (...) {

    }

    capture.running = false;
    detector.running = false;
    capture.join();
    detector.join();

    return 0;
};
