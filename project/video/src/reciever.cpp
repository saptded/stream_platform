//
// Created by saptded on 22.11.2020.
//

#include "reciever.h"

namespace video {
Reciever::Reciever() {
  cap = cv::VideoCapture("udpsrc port=50050 !"
                       "application/x-rtp, payload = 26, clock-rate=90000, encoding-name=JPEG !"
                       "rtpjpegdepay !"
                       "jpegdec !"
                       "videoconvert !"
                       "appsink",
                       cv::CAP_GSTREAMER);

  if (!cap.isOpened()) {
    throw "no signal";
  }
}

void Reciever::recieve() {
  while (true) {
    cap >> frame;
    imshow("cap", frame);

    if (cv::waitKey(1) == 27) {
      break;
    }
  }
}
}
