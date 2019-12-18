#pragma comment(lib, "opencv_world412")
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

#include <iostream>
    using std::cout;
    using std::endl;
#include    <string>
    using   std::string;
#include <deque>
    using   std::deque;
using namespace cv;

void search_edit_points(string, deque<double>*);
