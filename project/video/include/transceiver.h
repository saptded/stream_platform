//
// Created by saptded on 22.11.2020.
//

#ifndef STREAM_PLATFORM__TRANSCEIVER_H_
#define STREAM_PLATFORM__TRANSCEIVER_H_

#include <opencv2/opencv.hpp>

namespace video {
class Transceiver {
 public:
  Transceiver();
  void activate();
 private:
  cv::Mat frame;
  cv::VideoCapture cap;
  cv::VideoWriter video_write;
};
}

#endif //STREAM_PLATFORM__TRANSCEIVER_H_
