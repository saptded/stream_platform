#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio.hpp>

using namespace cv;
using namespace std;

int main() {
  VideoCapture cap("udpsrc port=50050 ! application/x-rtp, media=video, payload=26, clock-rate=90000, encoding-name=JPEG, framerate=60/1 ! rtpjpegdepay ! jpegdec ! videoconvert ! appsink",CAP_GSTREAMER);
  if (!cap.isOpened()) {
    cout << "Failed to open camera" << endl;
    return -1;
  }

  while (true) {
    Mat frame;
    cap >> frame;
    imshow("cap", frame);


    if (waitKey(1) == 27) { //ESC
      break;
    }
  }

  return 0;
}