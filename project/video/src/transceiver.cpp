//
// Created by saptded on 22.11.2020.
//

#include "transceiver.h"

namespace video {
Transceiver::Transceiver() {
  cap = cv::VideoCapture(0);

  int width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
  int height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
  double fps = cap.get(cv::CAP_PROP_FPS);

  const std::string gst_pipline =
      "appsrc "
      "! videoconvert "
      "! video/x-raw, format=YUY2,"
      "width = " + std::to_string(width) + ","
      "height = " + std::to_string(height) + ","
      "framerate =" + std::to_string((int)fps) + "/1 "
      "! jpegenc "
      "! rtpjpegpay "
      "! udpsink "
      "host = 192.168.0.108 "
      "port = 50050";

  video_write = cv::VideoWriter(gst_pipline, cv::CAP_GSTREAMER, 0, (int)fps, cv::Size(width, height), true);
  if (!video_write.isOpened() || !cap.isOpened()) {
    throw "camera error";
  }
}

void Transceiver::activate() {
  while (true) {
    cap >> frame;
    video_write << frame;

    if (cv::waitKey(1) == 27) { //ESC
      break;
    }
  }
}

}