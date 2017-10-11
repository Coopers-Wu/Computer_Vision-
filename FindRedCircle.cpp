#include<iostream>
#include<opencv2\opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
	int x, y, h, w;
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	vector<vector<Point>> contours;
	Mat src = imread("circle.jpg");
	Mat gray,binary;
	if (!src.data)
	{
		cout << "empty..";
		waitKey(0);
		return 0;
	}
	cvtColor(src, gray, COLOR_BGR2GRAY);
	threshold(gray, binary, 90, 255, THRESH_BINARY_INV || THRESH_OTSU);
	morphologyEx(binary, binary, MORPH_OPEN,element);
	morphologyEx(binary, binary, MORPH_CLOSE, element);
	findContours(binary, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	vector<Rect> boundrect(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		boundrect[i] = boundingRect(contours[i]);
		x = boundrect[i].x;
		y = boundrect[i].y;
		h = boundrect[i].height;
		w = boundrect[i].width;
		rectangle(src, boundrect[i], Scalar(0, 255, 0), 1, 8);
		string height = to_string(h);
		string width = to_string(w);
		putText(src, height, Point(x - 25, y + (h / 2)), FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 255), 1);
		putText(src, width, Point(x,y-5), FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 255), 1);
	}

	//imshow("g", gray);
	//imshow("b", binary);


	imshow("src", src);
	waitKey(0);
	return 0;
}
