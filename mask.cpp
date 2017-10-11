#include<opencv2\opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

Mat low_hsv_red = Mat(1, 3, CV_8UC3, Scalar(155, 70, 46));
//暂时不太理解这种操作,直接在参数里面使用scalar也没有问题
Mat high_hsv_red = Mat(1, 3, CV_8UC3, Scalar(180, 255, 255));
//Scalar low = (155, 70, 46);

Mat low_hsv_2red = Mat(1, 3, CV_8UC3, Scalar(0, 70, 46));
Mat high_hsv_2red = Mat(1, 3, CV_8UC3, Scalar(11, 255, 255));
vector<Mat> channels;
//Mat test = Mat(2, 3, CV_8UC3, Scalar(155, 70, 46));

int main()
{
	
	Mat src, mask, mask1, HSV, Final;
	src = imread("1.jpg",CV_LOAD_IMAGE_COLOR);
	if (!src.data)//为了确保程序不会崩溃，以后都需要这样的一个判断。
	{
		cout << "没有读到图像" << endl;
		waitKey(0);
		return 0;
	}
	putText(src, "Cooper_Wu", Point(5, 25), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 0));
	imshow("src", src);
	cvtColor(src, HSV, COLOR_BGR2HSV);
	//转换到HSV通道，对于颜色的筛选有着很好的帮助，还有一些其他的通道，例如YCbCr

	//用一个vector容器可以很好的存放几个通道，vector容器的使用是必须要去掌握的
	split(HSV, channels);
	Mat H = channels.at(0);
	Mat S = channels.at(1);
	Mat V = channels.at(2);
	//channels.push_back(H);
	imshow("H", H);

	imshow("HSV", HSV);//更换了通道之后，show出来是很不一样的。
	inRange(HSV, low_hsv_red, high_hsv_red, mask);//比threshold要方便一些
	inRange(HSV, low_hsv_2red, high_hsv_2red, mask1);
	Mat mask_and = mask + mask1;
	//这个强行相加。。。。，你可以查看定义，然后这个加号有几十种重载。
	imshow("mask", mask_and);
	bitwise_and(src, src, Final, mask = mask_and);//掩膜操作。书上貌似没有介绍这种用法。
	putText(Final, "Code by:Cooper_Wu", Point(5, 25), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 0));
	imshow("Final", Final);
	//cout << test << endl;
	waitKey(0);
	return 0;
}
