#ifndef ARKANOID_ARKANOID_H
#define ARKANOID_ARKANOID_H

#include <opencv2/core/core.hpp>
#include <utility>
#include <array_2d.h>
#include <image.h>
#include <cstdint>

struct MovingObject
{
    MovingObject(cv::Point2f  pos,
                 cv::Point2f  v,
                 cv::Scalar  color):
        position(std::move(pos)),
        velocity(std::move(v)),
        color(std::move(color))
    {}

    cv::Point2f position;
    cv::Point2f velocity;
    cv::Scalar color;
};

cv::Point2f reflect(const cv::Point2f& v,
                    const cv::Point2f& normal)
{
    return v - normal * ((v * 2.0f).dot(normal) / normal.dot(normal));
}

bool ballHitsRect(const cv::Point2f& ball_center,
                  float ball_radius,
                  const cv::Rect& rect,
                  cv::Point2f* normal = nullptr)
{
    cv::Point2f tl { (float) rect.tl().x, (float) rect.tl().y };
    cv::Point2f br { (float) rect.br().x, (float) rect.br().y };

    if (ball_center.x + ball_radius < tl.x
        || ball_center.x - ball_radius >= br.x
        || ball_center.y + ball_radius < tl.y
        || ball_center.y - ball_radius >= br.y) {
        return false;
    }

    if (normal) {
        struct Pair
        {
            float distance;
            cv::Point2f normal;

            bool operator <(const Pair& other) const
            { return distance < other.distance; }
        };
        Pair to_top { std::abs(tl.y - ball_center.y), { 0.0f, -1.0f }};
        Pair to_bottom { std::abs(br.y - ball_center.y), { 0.0f, 1.0f }};
        Pair to_left { std::abs(tl.x - ball_center.x), { -1.0f, 0.0f }};
        Pair to_right { std::abs(br.x - ball_center.x), { 1.0f, 0.0f }};

        Pair tuples[] = { to_top, to_bottom, to_left, to_right };
        *normal = (*std::min_element(tuples, tuples + 4)).normal;
    }

    return true;
}

class Game
{
public:
    static constexpr float BALL_SPEED = 250.0f;
    static constexpr int BALL_RADIUS = 10;
    static constexpr float PADDLE_WIDTH = 400.0f;
    static constexpr float PADDLE_HEIGHT = 30.0f;
    static constexpr size_t BLOCK_WIDTH = 100;
    static constexpr size_t BLOCK_HEIGHT = 60;

    Game(size_t board_width,
         size_t board_height):
        _board_width(board_width),
        _board_height(board_height),
        _blocks(board_width / BLOCK_WIDTH, board_height / 2 / BLOCK_HEIGHT),
        _ball(*new MovingObject(cv::Point2f(_board_width / 2, _board_height / 2 + 40), cv::Point2f(0.0f, BALL_SPEED), cv::Scalar(255, 255, 255))),
        _paddle(board_width / 2.0f - PADDLE_WIDTH / 2, board_height - PADDLE_HEIGHT, PADDLE_WIDTH, PADDLE_HEIGHT),
        _score(0)
    {
        reset();
    }

    void reset()
    {
        _ball = *new MovingObject(cv::Point2f(_board_width / 2, _board_height / 2 + 40),
                            cv::Point2f(0.0f, BALL_SPEED),
                            cv::Scalar(255, 255, 255));
        _blocks.fill(1);
        _score = 0;
        std::cout<<"New game!\n";
        std::cout<<"Score: "<<_score<<"\n";
    }

    void update(float dt)
    {
            _ball.position += _ball.velocity * dt;
            handleCollisions();
    }

    void setPaddlePos(size_t pos)
    {
        _paddle.x = pos - _paddle.width / 2;
    }

    void drawOnto(Image& img)
    {
        Image board_img(img.size(), img.type(), cv::Scalar(0, 0, 0));
        cv::Rect board_rect { 0, 0, (int)_board_width, (int)_board_height };

        for (size_t y = 0; y < _blocks.height; y+=2) {
            for (size_t x = 0; x < _blocks.width; x+=2) {
                if (!_blocks[x][y]) {
                    continue;
                }

                cv::Scalar color = cv::Scalar(0.0f, 255.0f, 0.0f);
                cv::rectangle(board_img, rectForBlock(x, y), color, -1);
            }
        }

        cv::circle(img, _ball.position, BALL_RADIUS, _ball.color, -1);

        cv::rectangle(img, _paddle, cv::Scalar(255, 255, 255), -1);
        img += board_img;
    };

    bool isGameOver() const
    {
      if (_ball.position.y > _board_height)
      {
        std::cout<<"You are loose!\n";
        return true;
      }
      return false;
    }

    bool isGameWon() const
    {
        for (size_t x = 0; x < _blocks.width; ++x) {
            for (size_t y = 0; y < _blocks.height; ++y) {
                if (_blocks[x][y]) {
                    return false;
                }
            }
        }
        std::cout<<"You are win!\n";
        return true;
    }

private:
    void handleCollisions()
    {
        MovingObject& ball = _ball;

        if (ball.position.x < 0.0f) {
            ball.velocity = reflect(ball.velocity, { 1.0f, 0.0f });
            ball.position.x = 0.0f;
        } else if (ball.position.x > _board_width) {
            ball.velocity = reflect(ball.velocity, { -1.0f, 0.0f });
            ball.position.x = _board_width;
        }

        if (ball.position.y < 0.0f) {
            ball.velocity = reflect(ball.velocity, { 0.0f, 1.0f });
            ball.position.y = 0.0f;
        }

        if (ballHitsRect(ball.position, (float)BALL_RADIUS, _paddle)) {
            ball.velocity = velocityFromBallPos(ball.position.x);
        }

        for (size_t y = 0; y < _blocks.height; ++y) {
            for (size_t x = 0; x < _blocks.width; ++x) {
                if (!_blocks[x][y]) {
                    continue;
                }

                cv::Point2f normal;
                cv::Rect rect = rectForBlock(x, y);
                if (ballHitsRect(ball.position, (float)BALL_RADIUS, rect, &normal)) {
                    ball.position += normal * BALL_RADIUS;
                    ball.velocity = reflect(ball.velocity, normal);
                    _blocks[x][y] = 0;
                    _score += 1;
                    std::cout<<"Score: "<<_score<<"\n";
                    return;
                }
            }
        }
    };

    cv::Point2f velocityFromBallPos(float ball_x)
    {
        float relative_x = (ball_x - _paddle.x) / _paddle.width;
        float angle = (relative_x - 0.5f) * ((float)CV_PI * 0.5f);
        return { std::sin(angle) * BALL_SPEED,
                 -std::cos(angle) * BALL_SPEED };
    }


    static cv::Rect rectForBlock(size_t x, size_t y)
    {
        return {
            (int) x * (int)BLOCK_WIDTH,
            (int) y * (int)BLOCK_HEIGHT,
            (int)BLOCK_WIDTH,
            (int)BLOCK_HEIGHT
        };
    }

    size_t _board_width;
    size_t _board_height;
    Array2D<uint8_t> _blocks;
    MovingObject _ball;
    cv::Rect_<float> _paddle;
    int _score;
};

#endif
