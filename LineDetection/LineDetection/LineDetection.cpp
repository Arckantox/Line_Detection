#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

using namespace cv;
using namespace std;

int edgeThresh = 1;
int lowThreshold = 50;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;

Mat src, mat_lines;
Mat src_gray, detected_edges;
String window_name = "Canny_test";

void DetectLines()
{
	src.copyTo(mat_lines);

	vector<Vec4i> lines;
	vector<Point2i> points;
	HoughLinesP(detected_edges, lines, 1, CV_PI / 180, 50, 50, 15);
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		line(mat_lines, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 2, CV_AA);
		points.push_back(Point(l[0], l[1]));
		points.push_back(Point(l[2], l[3]));
	}
	
	int sumX = 0;
	int sumY = 0;
	int Psize = points.size();
	for (size_t i = 0; i < points.size(); i++)
	{
	sumX += points[i].x;
	sumY += points[i].y;
	}
	Point2f impact = Point2f((float)(sumX / Psize), (float)(sumY / Psize));
	circle(mat_lines, impact, 5, Scalar(255, 0, 0), 2, CV_AA);
	

	imshow("detected lines", mat_lines);

}

void ReProcess(int, void*)
{
	/// Canny detector
	Canny(src_gray, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);

	imshow(window_name, detected_edges);

	DetectLines();
}



int main(int argc, char** argv)
{
	const char* filename = argc >= 2 ? argv[1] : "glass.jpg";

	src = imread(filename, IMREAD_COLOR);
	if (src.empty())
	{
		cout << "can not open " << filename << endl;
		return -1;
	}

	// Mise en niveau de gris de l'image en input
	cvtColor(src, src_gray, CV_BGR2GRAY);

	// Ajout d'un flou
	blur(src_gray, src_gray, Size(3, 3));

	namedWindow(window_name, CV_WINDOW_AUTOSIZE);

	// Trackbar for user to enter threshold
	createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, ReProcess);

	ReProcess(0,0);

	waitKey();

	return 0;
}