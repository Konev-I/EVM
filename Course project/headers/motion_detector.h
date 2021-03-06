#ifndef _ARKANOID_MOTION_DETECTOR_H_
#define _ARKANOID_MOTION_DETECTOR_H_

#include <opencv2/opencv.hpp>

#include <image.h>
#include "window.h"

template<size_t N, size_t I, typename T, typename... Tail>
struct n_tuple
{
    typedef typename n_tuple<N, I - 1, T, T, Tail...>::tuple_type tuple_type;
    typedef typename std::array<T, N> array_type;

    static tuple_type from_array(array_type&& array, Tail&&... args)
    {
        return n_tuple<N, I - 1, T, T, Tail...>::from_array(
                std::forward<array_type>(array),
                std::forward<Tail>(args)...,
                std::move(array[I - 1]));
    }
};

template<size_t N, typename T, typename... Tail>
struct n_tuple<N, 1, T, Tail...>
{
    typedef typename std::tuple<T, Tail...> tuple_type;
    typedef typename std::array<T, N> array_type;

    static std::tuple<T, Tail...> from_array(array_type&& array, Tail&&... args)
    {
        return std::make_tuple(std::move(array[0]), std::forward<Tail>(args)...);
    }
};

template<typename T, size_t N>
typename n_tuple<N, N, T>::tuple_type unpack(std::array<T, N>&& array)
{
    return n_tuple<N, N, T>::from_array(std::forward<std::array<T, N>>(array));
}

namespace {

  cv::Rect enclosingRect(const cv::Rect& a,
                           const cv::Rect& b) {
        cv::Rect rect;
        rect.x = std::min(a.x, b.x);
        rect.y = std::min(a.y, b.y);
        rect.width = (std::max(a.x + a.width, b.x + b.width) - rect.x);
        rect.height = (std::max(a.y + a.height, b.y + b.height) - rect.y);
        return rect;
    }
}

class Marker
{
public:
    Marker():
        _grip(false),
        _kalman(4, 2, 0)
    {
        _kalman.statePre = cv::Mat::zeros(2, 1, CV_32F);
        cv::setIdentity(_kalman.measurementMatrix);
        cv::setIdentity(_kalman.processNoiseCov, cv::Scalar::all(1e-1));
        cv::setIdentity(_kalman.measurementNoiseCov, cv::Scalar::all(10));
        cv::setIdentity(_kalman.errorCovPost, cv::Scalar::all(.5));
    }

    void nextPosition(const cv::Point2f& pos,
                      const cv::Point2i& image_size) {
        nextPosition({ pos.x / image_size.x,
                       pos.y / image_size.y });
    }

    void nextPosition(const cv::Point2f& pos) {
        _last_position = pos;
        update();
    }

    void update() {
        _kalman.correct(cv::Mat(_last_position));
    }

    cv::Point2f getSmoothedPosition(const cv::Point2i& image_size) const {
        const cv::Mat& prediction = _kalman.predict(cv::Mat());

        return { prediction.at<float>(0) * image_size.x,
                 prediction.at<float>(1) * image_size.y };
    }

    cv::Point2f getLastPosition(const cv::Point2i& image_size) const {
        return { _last_position.x * image_size.x,
                 _last_position.y * image_size.y };
    }

    void grip() {
        _grip = true;
    }

    void release() {
        _grip = false;
    }

    bool hasGrip() const {
        return _grip;
    }

private:
    cv::Point2f _last_position;
    bool _grip;
    mutable cv::KalmanFilter _kalman;
};

class MotionDetector {
public:
    MotionDetector(size_t width,
                   size_t height):
        width(width),
        height(height)
    {
    }

    std::vector<std::vector<cv::Point>> _contours;

    std::vector<std::vector<cv::Point>> getSignificantContours(const Image& greyscale_image) const {
        std::vector<std::vector<cv::Point>> significant_contours;

        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(greyscale_image, contours, hierarchy,
                         cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE,
                         cv::Point(0, 0));

        for (auto& contour: contours) {
            cv::Mat contour_mat(contour);

            if (fabs(cv::contourArea(contour_mat, false)) >= settings.min_poly_area) {
                significant_contours.emplace_back(contour);
            }
        }

        return significant_contours;
    }

    cv::Rect _prev_enclosing_rect;
    std::deque<int> _last_frames_bb_area;

    cv::Point2f getMarkerPos() const
    {
        if (!_curr_frame.empty()) {
            return _marker.getLastPosition({ (int)width, (int)height });
        } else {
            return { 0.0f, 0.0f };
        }
    }

    void detectGrip(const cv::Point& marker_pos) {
        cv::Rect enclosing_rect = findEnclosingRect();
        if (_prev_enclosing_rect.contains(marker_pos)
                || enclosing_rect.contains(marker_pos)) {
            int area = enclosing_rect.area() / 10000;
            _last_frames_bb_area.push_back(area);
            if (_last_frames_bb_area.size() > 5) {
                _last_frames_bb_area.pop_front();
            }

            int last = _last_frames_bb_area.front();
            bool ascending = true;
            bool descending = true;
            for (int val: _last_frames_bb_area) {
                if (val < last) {
                    ascending = false;
                } else if (val > last) {
                    descending = false;
                }
                last = val;
            }
            if (ascending) {
                _marker.release();
            } else if (descending) {
                _marker.grip();
            }
        }

        _prev_enclosing_rect = enclosing_rect;
    }

  void nextFrame(const Image &frame) {

        _prev_frame = std::move(_curr_frame);
        _curr_frame = preprocessFrame(frame);

        if (!_curr_frame.empty() && !_prev_frame.empty()) {
            Image preprocessed = amplifyMotion(_prev_frame, _curr_frame);

            _contours = getSignificantContours(preprocessed.toGreyscale());

            cv::Point marker_pos = _marker.getSmoothedPosition(frame.size());
            if (!_contours.empty()) {
                detectGrip(marker_pos);
            }

            cv::Point2f center;
            if (tryGetCenterPoint(_contours, center)) {
                _marker.nextPosition(center, preprocessed.size());
            } else {
                _marker.update();
            }

            std::move(preprocessed);
        }
    }

    Image toImage(const Image &background) const {
        Image ret = static_cast<Image>(Image::zeros(height, width, CV_8UC3));

        bool show_bg = settings.show_background;

        if (!_curr_frame.empty() && settings.show_debug_frame) {
            ret += _curr_frame.resized(ret.size());
        }


        if (!background.empty() && show_bg) {
            ret += background.resized(ret.size());
        }

        drawDebugInfo(ret);

        settings.display(ret);
        return ret;
    }

    struct Settings {
        uint8_t motion_threshold = 100;
      int min_poly_area = 300;
        bool show_background = true;
        bool show_debug_contours = true;
        bool show_debug_frame = false;

        void display(Image &image) const {
            Image textImage(image.size(), image.type(), cv::Scalar(0, 0, 0, 255));
        }
    };

    Settings settings;
    size_t width;
    size_t height;

private:
    Image _prev_frame;
    Image _curr_frame;

    Marker _marker;

    cv::Scalar _significant_color = cv::Scalar(0, 0, 0);

    Image preprocessFrame(const Image &image)
    {
        float THRESHOLD = 40;

        Image ret(image.size(), image.type());
        cv::absdiff(image, _significant_color, ret);
        cv::threshold(ret, ret, THRESHOLD, 255, cv::THRESH_BINARY_INV);

        return ret;
    }

    static bool tryGetCenterPoint(const std::vector<std::vector<cv::Point>>& contours,
                                  cv::Point2f& out_point) {
        if (contours.empty()) {
            return false;
        }

        out_point.x = 0;
        out_point.y = 0;

        size_t num_points = 0;
        for (const auto& poly: contours) {
            for (const cv::Point& p: poly) {
                out_point.x += p.x;
                out_point.y += p.y;
            }

            num_points += poly.size();
        }

        out_point.x /= (float)num_points;
        out_point.y /= (float)num_points;
        return true;
    }

    cv::Rect findEnclosingRect() const {
        std::vector<cv::Rect> bounding_boxes;

        cv::Rect big_bb;
        cv::Point2f scale((float)width / _curr_frame.cols,
                          (float)height / _curr_frame.rows);
        for (const auto& contour: _contours) {
            std::vector<cv::Point> poly;
            cv::approxPolyDP(cv::Mat(contour), poly, 3, true);

            for (cv::Point& p: poly) {
                p.x *= scale.x;
                p.y *= scale.y;
            }

            cv::Mat poly_mat(poly);
            cv::Rect bb = cv::boundingRect(poly_mat);

            if (big_bb.area() == 0) {
                big_bb = bb;
            } else {
                big_bb = enclosingRect(big_bb, bb);
            }
        }

        return big_bb;
    }

    void drawDebugContours(Image& out_image) const {
        std::vector<std::vector<cv::Point>> contours_poly;
        std::vector<cv::Rect> bounding_boxes;

        contours_poly.reserve(_contours.size());
        bounding_boxes.reserve(_contours.size());

        cv::Rect big_bb;
        cv::Point2f scale((float)width / _curr_frame.cols,
                          (float)height / _curr_frame.rows);
        for (const auto& contour: _contours) {
            std::vector<cv::Point> poly;
            cv::approxPolyDP(cv::Mat(contour), poly, 3, true);

            for (cv::Point& p: poly) {
                p.x *= scale.x;
                p.y *= scale.y;
            }

            cv::Mat poly_mat(poly);
            cv::Rect bb = cv::boundingRect(poly_mat);

            if (big_bb.area() == 0) {
                big_bb = bb;
            } else {
                big_bb = enclosingRect(big_bb, bb);
            }

            cv::Point2f center;
            float radius;
            cv::minEnclosingCircle(poly_mat, center, radius);

            contours_poly.emplace_back(std::move(poly));
            bounding_boxes.emplace_back(std::move(bb));
        }

        if (settings.show_debug_contours) {
            for (size_t i = 0; i < contours_poly.size(); ++i) {
                cv::Scalar color = cv::Scalar(163,163,163);
                cv::drawContours(out_image, contours_poly, (int) i, color,
                                 1, 8, std::vector<cv::Vec4i>(), 0,
                                 cv::Point());
                cv::rectangle(out_image,
                              bounding_boxes[i].tl(), bounding_boxes[i].br(),
                              color, 2, 8, 0);
            }
        }
    }

    void drawDebugInfo(Image& out_image) const {
        if (settings.show_debug_contours) {
            drawDebugContours(out_image);

            cv::Rect enclosing_rect = findEnclosingRect();
            cv::rectangle(out_image, enclosing_rect.tl(), enclosing_rect.br(),
                          cv::Scalar(0, 255, 0), 2, 8, 0);
        }
    }

    Image amplifyMotion(const Image& prev_frame,
                        const Image& curr_frame) {
        cv::Size small_size(320, 240);

        Image diff = static_cast<Image>(curr_frame - prev_frame);

        Image r, g, b;
        std::tie(b, g, r) = unpack(diff.toChannels());

        for (Image* channel_ptr: { &r, &g, &b }) {
            Image& channel = *channel_ptr;
            channel = channel.resized(small_size);
            cv::equalizeHist(channel, channel);
        }

        Image rgb = Image::fromChannels(b, g, r);

        Image greyscale_big = rgb.toGreyscale();
        Image greyscale = greyscale_big.resized(small_size);

        Image preprocessed;
        cv::threshold(greyscale.blurred(7).toColored(), preprocessed, settings.motion_threshold, 255, cv::THRESH_BINARY);

        return preprocessed.resized(curr_frame.cols, curr_frame.rows);
    }
};

#endif
