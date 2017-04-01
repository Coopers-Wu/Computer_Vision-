//该程序是我结合这pylon sdk 和 opencv3.1写的，框架比较乱，但识别三角形的思路大体上没有问题
#include <pylon/PylonIncludes.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;
using namespace Pylon;

Mat element = getStructuringElement(MORPH_RECT, Size(4, 4));
Mat dstImage, H, S, V, A, B, C, d, e, f, p, q, r;
vector<Point> model;

class CImageProcess :public CImageEventHandler
{
public:
	void OnImageGrabbed(CInstantCamera& camera, const CGrabResultPtr& ptrGrabResult)
	{
		CImageFormatConverter fc;
		fc.OutputPixelFormat = PixelType_BGR8packed;
		CPylonImage image;

		if (ptrGrabResult->GrabSucceeded())
		{
			vector<vector<Point> > contours;
			vector<Vec4i> hierarchy;
			Point top_point[3];
			Mat Small;
			Point media, start, vec;
			Mat result(Size(480, 480), CV_8UC1, Scalar(0));
			Mat final1(Size(480, 480), CV_8UC1, Scalar(0));
			const int64 starttime = getTickCount();
			fc.Convert(image, ptrGrabResult);
			Mat frame = Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3, (uint8_t*)image.GetBuffer());
			resize(frame, Small, Size(480, 480));
			cvtColor(Small, dstImage, COLOR_BGR2GRAY);
			threshold(dstImage, A, 100, 255, THRESH_BINARY_INV);
			morphologyEx(A, B, MORPH_OPEN, element);
			findContours(B, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
			vector<vector<Point> >	contours_poly(contours.size());
			for (unsigned int i = 0; i < contours.size(); i++)
			{
				approxPolyDP(Mat(contours[i]), contours_poly[i], 4, true);
				drawContours(result, contours_poly, i, Scalar(255), 1, 8);
			}

			for (int i = 0; i<contours_poly.size(); i++)
			{
				if (contourArea(contours_poly[i]) > 1500&& contourArea(contours_poly[i]) <5000)
				{
					double ffff = matchShapes(contours_poly[i], model, CV_CONTOURS_MATCH_I1, 1.0);//轮毂形状匹配这里用CV_CONTOURS_MATCH_I1比较好，，注意是  1   
					if (ffff < 0.1)
					{

						drawContours(final1, contours_poly, i, 255, 1);
						Point minx, maxx, miny, maxy;

						minx = contours_poly[i].at(0);
						maxx = contours_poly[i].at(0);
						miny = contours_poly[i].at(0);
						maxy = contours_poly[i].at(0);
						for (int j = 0; j<contours_poly[i].size(); j++)
						{
							//					cout << contours_poly[i].at(j)<<endl;
							if (contours_poly[i].at(j).x<minx.x) minx = contours_poly[i].at(j);
							if (contours_poly[i].at(j).x>maxx.x) maxx = contours_poly[i].at(j);
							if (contours_poly[i].at(j).y<miny.y) miny = contours_poly[i].at(j);
							if (contours_poly[i].at(j).y>maxy.y) maxy = contours_poly[i].at(j);
						}
						cout << minx << miny << maxx << maxy << endl;
						if (minx == miny)
						{
							top_point[0] = maxx;
							top_point[1] = maxy;
							top_point[2] = minx;
						}
						if (minx == maxy)
						{
							top_point[0] = maxx;
							top_point[1] = miny;
							top_point[2] = minx;
						}
						if (maxx == maxy)
						{
							top_point[0] = maxx;
							top_point[1] = miny;
							top_point[2] = minx;
						}
						if (maxx == miny)
						{
							top_point[0] = maxx;
							top_point[1] = maxy;
							top_point[2] = minx;
						}
					//	cout << top_point[0] << top_point[1] << top_point[2] << endl;

					}

					//寻找角度
					int L1 = (int)sqrt((top_point[0].x - top_point[1].x)*(top_point[0].x - top_point[1].x) +
						(top_point[0].y - top_point[1].y)*(top_point[0].y - top_point[1].y));
					int L2 = (int)sqrt((top_point[0].x - top_point[2].x)*(top_point[0].x - top_point[2].x) +
						(top_point[0].y - top_point[2].y)*(top_point[0].y - top_point[2].y));
					int L3 = (int)sqrt((top_point[2].x - top_point[1].x)*(top_point[2].x - top_point[1].x) +
						(top_point[2].y - top_point[1].y)*(top_point[2].y - top_point[1].y));

					if (L1 > L2&&L1 > L3)
					{
						start = top_point[2];
						media = (top_point[0] + top_point[1]) / 2;
						line(final1, top_point[2], media, Scalar(255), 1, LINE_AA);
					}
					if (L2 > L1&&L2 > L3)
					{
						start = top_point[1];
						media = (top_point[0] + top_point[2]) / 2;
						line(final1, top_point[1], media, Scalar(255), 1, LINE_AA);
					}
					if (L3 > L2&&L3 > L1)
					{
						start = top_point[0];
						media = (top_point[1] + top_point[2]) / 2;
						line(final1, top_point[0], media, Scalar(255), 1, LINE_AA);
					}




				}
			}


			Point middle = (top_point[0] + top_point[1] + top_point[2]) / 3;
			vec = start - media;
			vec.y = -vec.y;
			cout << vec << endl;
			cout << middle << endl;

			imshow("最后", final1);
			imshow(" ", result);
			waitKey(1);
			const double timeSec = (getTickCount() - starttime) / getTickFrequency();
			cout << "Time : " << timeSec * 1000 << " ms" << endl;
		}





	}
protected:
private:
};




int main(void)
{
	vector<vector<Point> >  cont_moban;
	vector<Vec4i>  hie_moban;
	Mat moban = imread("模板.jpg");
	cvtColor(moban, p, COLOR_BGR2GRAY);
	threshold(p, q, 120, 255, THRESH_BINARY_INV);
	morphologyEx(q, r, MORPH_OPEN, element);
	findContours(r, cont_moban, hie_moban, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	Mat result_moban(q.size(), CV_8UC1, Scalar(0));
	vector<vector<Point> >	contours_poly_moban(cont_moban.size());
	for (unsigned int i = 0; i < cont_moban.size(); i++)
	{
		approxPolyDP(Mat(cont_moban[i]), contours_poly_moban[i], 4, true);
		drawContours(result_moban, contours_poly_moban, i, Scalar(255), 1, 8);

	}
	model = contours_poly_moban[0];
//	imshow("模板轮廓", result_moban);
	waitKey(1);
	PylonInitialize();
	CInstantCamera camera(CTlFactory::GetInstance().CreateFirstDevice());
	camera.RegisterImageEventHandler(new CImageProcess, RegistrationMode_Append, Cleanup_Delete);
	camera.Open();
	camera.StartGrabbing(GrabStrategy_OneByOne, GrabLoop_ProvidedByInstantCamera);
	while (1);
	camera.StopGrabbing();
	camera.DestroyDevice();
	PylonTerminate();
	return 0;
}
