//
// Created by saptded on 12.12.2020.
//

#ifndef QT_TEST__VIDEO_RECIEVER_H_
#define QT_TEST__VIDEO_RECIEVER_H_

#include <QLabel>
#include <QTimer>
#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include "client.hpp"

class VideoReceiver : public QLabel {
 Q_OBJECT
 public:
    explicit VideoReceiver(sp::Client &client, QWidget *parent = nullptr);;

    void update_image();

 private:
    QTimer _timer;
    cv::VideoCapture cap;
    cv::Mat frame;
    sp::Client _client;
};

#endif //QT_TEST__VIDEO_RECIEVER_H_
