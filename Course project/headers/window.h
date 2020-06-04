#ifndef _ARKANOID_WINDOW_H_
#define _ARKANOID_WINDOW_H_

#include <opencv2/opencv.hpp>
#include <utility>

class Window
{
public:
    explicit Window(std::string  name):
        _name(std::move(name))
    {
        cv::namedWindow(_name, cv::WINDOW_AUTOSIZE);
    }

    Window(const Window&) = delete;
    Window& operator =(const Window&) = delete;

    Window(Window&& old) {
        *this = std::move(old);
    }
    Window& operator =(Window&& old) {
        _name = std::move(old._name);
        old._name.clear();
        return *this;
    }

    ~Window() {
        if (!_name.empty()) {
            cv::destroyWindow(_name);
        }
    }

    void showImage(const cv::Mat& image) {
        cv::imshow(_name, image);
    }

private:
    std::string _name;
};

#endif
