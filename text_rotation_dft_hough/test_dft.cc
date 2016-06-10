#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <cmath>

using namespace cv;

Mat I, r;
int s;
int reversed = 0;

Mat rotate(Mat src, double angle)
{
    Mat dst;
    Point2f pt(src.cols/2., src.rows/2.);    
    r = getRotationMatrix2D(pt, angle, 1.0);
    warpAffine(src, dst, r, Size(src.cols, src.rows));
    return dst;
}

void reversed_cb(int, void*)
{
  r = rotate(I, s + (reversed ? -180 : 0));

  imshow("Input Image", r);
}

int main(int argc, char ** argv)
{
  const char* filename;
  if (argc > 1)
    filename = argv[1];
  else
    std::cout << "Usage: ./test_dft <filename>" << std::endl;

  I = imread(filename, IMREAD_GRAYSCALE);
  if( I.empty())
    return -1;

  Mat padded;
  int m = getOptimalDFTSize( I.rows );
  int n = getOptimalDFTSize( I.cols );
  copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));

  Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
  Mat complexI;
  merge(planes, 2, complexI);

  dft(complexI, complexI);

  split(complexI, planes);
  magnitude(planes[0], planes[1], planes[0]);
  Mat magI = planes[0];

  magI += Scalar::all(1);
  log(magI, magI);

  magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

  int cx = magI.cols/2;
  int cy = magI.rows/2;

  Mat q0(magI, Rect(0, 0, cx, cy));
  Mat q1(magI, Rect(cx, 0, cx, cy));
  Mat q2(magI, Rect(0, cy, cx, cy));
  Mat q3(magI, Rect(cx, cy, cx, cy));

  Mat tmp;
  q0.copyTo(tmp);
  q3.copyTo(q0);
  tmp.copyTo(q3);

  q1.copyTo(tmp);
  q2.copyTo(q1);
  tmp.copyTo(q2);

  normalize(magI, magI, 0, 1, NORM_MINMAX);

  magI.convertTo(magI, CV_8U, 255);
  imwrite("m.jpg", magI);

  std::vector<Vec2f> lines;
  std::vector<Vec4i> linesP;
  std::vector<float> slopes;
  std::vector<int> intercepts;

  Mat cdst = imread("m.jpg", 0);

  Mat dst = Mat::zeros(I.rows, I.cols, CV_8UC3);

  Canny(cdst, cdst, 150, 150, 3);
  imshow("tst", cdst);
  HoughLines(cdst, lines, 1, CV_PI/180, 70);
  HoughLinesP(cdst, linesP, 1, CV_PI/180, 50, 50, 10);
  for (size_t i = 0; i < lines.size(); i++)
  {
    Vec2f l = lines[i];
    std::cout << "l " << l[0] << " " << l[1] * 180 / CV_PI << "(" << l[1] << ")" << std::endl;
    int angle = l[1] * 180 / CV_PI;
    if ((angle < 87 || angle > 93) && (angle < -1 || angle > 1) && i >= 10)
        slopes.push_back(l[1] * 180 / CV_PI);
    float a = cos(l[1]);
    float b = sin(l[1]);
    int x0 = l[0] * a;
    int y0 = l[0] * b;
    int pt_x0 = cvRound(x0 + 1000*(-b));
    int pt_y0 = cvRound(y0 + 1000*a);
    int pt_x1 = cvRound(x0 - 1000*(-b));
    int pt_y1 = cvRound(y0 - 1000*(a));

    if ((angle < 87 || angle > 93) && angle != 0)
    {
      line(dst, Point(pt_x0, pt_y0), Point(pt_x1, pt_y1), Scalar(0,0,255), 3, LINE_AA);

      Vec4i lP = linesP[i];
      line(dst, Point(lP[0], lP[1]), Point(lP[2], lP[3]), Scalar(255,0,0), 3, LINE_AA);
    }
  }

  for (int i = 0; i < slopes.size(); i++)
    std::cout << "slopes " << slopes[i] << std::endl;

  namedWindow("Input", 1);
  imshow("Frequency Domain", magI);
  imshow("Hough", dst);
  s = slopes[0];
  std::cout << "angle " << s << std::endl;
  reversed_cb(0, 0);
  createTrackbar("reversed", "Input", &reversed, 1, reversed_cb);

  char k = 0;
  while ((k = waitKey()))
    if (k == 27)
      break;

  return 0;
}
