#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

int fil = 0, n, col = 0;
int m;

int main(){

	Mat imagen, imagenGris, imagenCanny;

	Mat canny_output;

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	vector<Point> contour;

	namedWindow("window", 0);

	namedWindow("window1", 0);

	namedWindow("window2", 0);

	imagen = imread("lena.bmp");

	cvtColor(imagen, imagenGris, CV_RGB2GRAY);

	blur(imagenGris, imagenGris, Size(3, 3));

	threshold(imagenGris, imagenGris, 128, 255, THRESH_BINARY_INV);

	double duration;

	duration = static_cast<double>(cv::getTickCount());

	/// Detect edges using canny
	Canny(imagenGris, imagenCanny, 50, 200, 3);

	/// Find contours
	findContours(imagenCanny, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// Get the moments
	vector<Moments> mu(contours.size());
	int countMayor = 0;
	int mayor=0;
	Mat hu;

	// Encontrar el mayor contorno


	for (int i = 0; i < contours.size(); i++)
	{
		if (countMayor < contours[i].size()){
			mayor = i;
			countMayor=contours[i].size();
		}
	}

	//Hallar momentos

	approxPolyDP(cv::Mat(contours[mayor]), contour, 0.025, true);
	cv::Rect rect = cv::boundingRect(contour);
	mu[mayor] = moments(contours[mayor], false);

	HuMoments(mu[mayor], hu);

	const cv::Point* p = &contour[0];
	int n = (int)contour.size();
	polylines(imagen, &p, &n, 1, true, cv::Scalar(0, 255, 0), 1, CV_AA);

	cv::Point pt1, pt2;
	pt1.x = rect.x;
	pt1.y = rect.y;
	pt2.x = rect.x + rect.width;
	pt2.x = rect.y + rect.height;
	cv::rectangle(imagen, rect, cv::Scalar(0, 0, 255), 1);

	cv::FileStorage fs("hu Data s", cv::FileStorage::WRITE);
	fs << "hu Moments" << hu;

	/// Get the mass centers:
	vector<Point2f> mc(1);
	mc[0] = Point2f(mu[mayor].m10 / mu[mayor].m00, mu[mayor].m01 / mu[mayor].m00);

	
	duration = static_cast<double>(cv::getTickCount()) - duration;

	duration /= cv::getTickFrequency(); // the elapsed time in ms

	imshow("window", imagen);

	imshow("window1", imagenGris);

	printf("duracin: %f", duration);

	waitKey(0);

	getchar();

	return 0;


}