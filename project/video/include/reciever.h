//
// Created by saptded on 22.11.2020.
//

#ifndef STREAM_PLATFORM__RECIEVER_H_
#define STREAM_PLATFORM__RECIEVER_H_

#include <opencv2/opencv.hpp>

namespace video{
class Reciever {
 public:
  Reciever();
  void recieve();
 private:
  cv::Mat frame;
  cv::VideoCapture cap;
};
}

#endif //STREAM_PLATFORM__RECIEVER_H_
