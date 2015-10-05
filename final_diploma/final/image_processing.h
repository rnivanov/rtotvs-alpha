//--->��������� ����������� �� ���������

//TODO:
//������� ������ (������ ������������, ����� ������������) ����� ������� ������� ��������� �����
/* ������������ ������ �������������� �� CalcHaarResponse �� ������ �����  */
/* �������� ��������� ��� ������� ����������� �� ����������  */
//�������� ���� �������� � ������� Calc_Image_Sum()

#ifndef IMG_PROC
#define IMG_PROC

#include "main_process.h"

#include <iostream>
#include <string>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
//#include <opencv2\features2d\features2d.hpp>

#define ERROR_150_V "ERROR 150: Keypoint * list is empty"
#define ERROR_151_V "ERROR 151: Keypoint * list has either zero or one point at (0,0)"

#define DRAW_ON_EMPTY_FIELD 0
#define DRAW_ON_OUTIMAGE 1

using namespace std;
using namespace cv;

#ifndef STRUCT_KEYPOINT
#define STRUCT_KEYPOINT
struct Keypoint //user keypoint, not regular cv::KeyPoint
{
	Point center;
	int radius;
	double * descriptor;
	struct Keypoint * next;
} ;
#endif

/*  �������� ������� ��������� �����������
	- ���������� ����
	- ����� �.�.
	- ������ ������������
���������� ��������� �� ������ �.�. � ������������� �������������
*/
Keypoint * imgproc_main(         
	Mat src,                     //���� (������������)
	Mat dst,                     //����� (��� �����������)
	int FAST_thresh = 50,        //�������� FAST
	string winname = "imgproc",  //��� ���� ������
	int gauss_ksize = 0,         //������ ���� �������� ������� (0, 3, 4,..k>0), 0 = �� �������.
	int med_ksize = 0,           //������ ���� ���������� ������� (0, 3, 4,..k>0), 0 = �� �������.
	bool verbose = false         //���. ���.
	);

/*  
������� ��������� ��������� ����� ��� ��������� Keypoint 
(������ CV::drawKeypoints)
�������� � ������� �������������,
���������� ���� ������ ��. ����
*/
int drawKeypointsList(
	Mat src,                      //���� (�.�. ������, �� ������������)
	Keypoint * points,            //������ �����
	Mat dst,                      //����� (� ����� ������ ��������� �� ������)
	int radius = 1,               //������ ����������� ����� > 0
	Scalar color = Scalar::all(255),//���� ���������
	int flags = 1,                //��� ����� 0 � 1, ��. ����
	int thickness = 1,            //������� �����, -1 = ���������
	bool verbose = false          //���� ���. ���.
	);

/*  
���� �� ���� ������, ��������� ����������, 
���������� �� ������� �� ������� ����������� �� ��������� radius.
����� ����� ������������ � ������� ������ (������ ���������� ������ �������), 
� ��� ��� ��������� �����������.
���� ����� �� ���� �������, �� �����. NULL, ����� ��������� �� ������ ������
**���������� �� ������� imgproc_main
***�������� ������� list_points
*/
Keypoint * list_points(     
	Mat src,                     //���� (������������)
	vector <KeyPoint> points,    //�����, ��������� ���������� (FAST)
	bool verbose = false,        //���. ���.
	int radius = 12,             //������������ ������ ������������ ����������� �.�.
	int desc_size = 3,           //(???) ����� ��������� ������������ (???)
	int desc_step = 4            //(???) ��� ���������� ������� ����������� (???)
	);

/*
������ ����������� ��� �������� �����
**���������� �� ������� list_points
***�������� ������� Calc_Descriptor
 - ����������� ��������� ������ ��� ������� ���������� 
���� double ����������� size.
 - ����������� ����� ������� ��� ������� ������� ������� �������
 - ����������� ���������� � ���������� ��� �����������
 - ������������� ������ size-1
������������ ��������� �� ������������ ������ double,
!NULL ������������ �� ������!
*/

double * Calc_Descriptor(
	Mat src,                    //���� (������������)
	Point center,               //����� �����������
	int desc_size = 3,          //����� ��������� ������������
	int desc_step = 4,          //��� ���������� ������� �����������
	bool verbose = false        //���� ���.���.
	);
/*
������ ������� �������� ��� ������������ �����������
**���������� �� ������� Calc_Descriptor
***�������� ������� Calc_Image_Sum
������� ������ �������� ���� double, ���������� ��������� �� ����
!NULL ������������ �� ������!
�������� ����������� ������ �� ������ ��������
*/

double * Calc_Haar_Response(
	Mat src,                    //���� (������������)
	Point center,               //����� �����������
	int radius = 8,             //������ �����������
	bool verbose = false        //���� ���.���.
	);

/*
������� ����� �������� �������� ����������� src
� ��������� ��� ����� � int
! ��������� � int ! �������� ������������
*/
int Calc_Image_Sum (Mat src, bool verbose = false);

/*
������� ������
*/
void clear_keypoints(Keypoint *img_pts);

#endif