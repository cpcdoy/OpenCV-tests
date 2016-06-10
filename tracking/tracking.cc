#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <cmath>

using namespace cv;

int main(int argc, char ** argv)
{
  //VideoCapture cap(0);
  const char* filename;
  char* end;
  int l = 0, r = 0;
  if (argc == 4)
  {
    filename = argv[1];
    l = strtol(argv[2], &end, 10);
    r = strtol(argv[3], &end, 10);
  }
  else
    std::cout << "Usage: ./test_dft <filename> <rgb left bound> <rgb right bound>" << std::endl;

  Mat I = imread(filename, CV_LOAD_IMAGE_COLOR);

  if(I.empty())
    return -1;

  Mat I_HSV;
  Mat th;

  cvtColor(I, I_HSV, COLOR_BGR2HSV);
  inRange(I_HSV, Scalar(l, 100, 75), Scalar(r, 255, 255), th);

  erode(th, th, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
  dilate(th, th, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
  dilate(th, th, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
  erode(th, th, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

  Canny(th, th, 150, 150, 3);

  std::vector<std::vector<Point> > contours;
  std::vector<Vec4i> hierarchy;
  std::vector<Point> curve;

  findContours(th, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

  for (size_t i = 0; i < contours.size(); i++)
  {
    approxPolyDP(Mat(contours[i]), curve, arcLength(Mat(contours[i]), true) * 0.01, true);
    if (curve.size() > 10 && curve.size() < 20)
      drawContours(I, contours, i, Scalar(100, 100, 100), 2, 8, hierarchy, 0, Point());
  }

  imshow("Img th", th);
  imshow("Img", I);

  char k = 0;
  while ((k = waitKey()))
    if (k == 27)
      break;

  return 0;
}
