#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <cmath>

using namespace cv;

Mat rotate(Mat src, double angle)
{
    Mat dst;
    Point2f pt(src.cols/2., src.rows/2.);    
    Mat r = getRotationMatrix2D(pt, angle, 1.0);
    warpAffine(src, dst, r, Size(src.cols, src.rows));
    return dst;
}

int main(int argc, char ** argv)
{
  const char* filename;
  if (argc > 1)
    filename = argv[1];
  else
    std::cout << "Usage: ./test_dft <filename>" << std::endl;

  Mat I = imread(filename, IMREAD_GRAYSCALE);

  if(I.empty())
    return -1;

  Mat save;
  I.copyTo(save);

  bitwise_not(I, I);
  threshold(I, I, 127, 255, THRESH_BINARY);

  Mat structure = getStructuringElement(MORPH_RECT, Size(2, 2));
  erode(I, I, structure);

  std::vector<Point> ps;
  
  for (Mat_<uchar>::iterator i = I.begin<uchar>(); i != I.end<uchar>(); i++)
    if (*i)
      ps.push_back(i.pos());

  RotatedRect bb = minAreaRect(Mat(ps));

  float angle = bb.angle + (bb.angle < -45 ? 90 : 0);

  std::cout << "angle " << angle << std::endl;
  save = rotate(save, angle);

  imshow("img", save);

  char k = 0;
  while ((k = waitKey()))
    if (k == 27)
      break;
}
