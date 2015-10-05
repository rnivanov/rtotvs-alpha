//--->��������� ����������

#ifndef MATCHING
#define MATCHING

#include "main_process.h"

#include <iostream>
#include <string>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
//#include <opencv2\features2d\features2d.hpp>

using namespace std;
using namespace cv;

//�������: ��� ������?
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

/*  
BF matching
�����. NULL ��� ���. ��� ���� ��� ��� ������ ������ ��� �� ������� ����������,
�����. ��������� �� ������ �����, ��� ���. ������� ����������
*/
Keypoint * matches_BF_simple(
	Mat dst,                  //������ ��� �����������
	Keypoint * pts_obj,       //Keypoints �� ���������� �����. �������
	Keypoint * pts_img,       //Keypoints �� ������� ����������� ��� ROI
	double thresh1 = 0.01,    //����������� ��� ��������� ������������
	double thresh2 = 0.0,     //����������� ��� ��������� ������������ ��� ���������
	bool adaptive = true,     //� �����������, �����. = true
	int desc_size = 8,        //������ �����������, ����� = 8
	bool verbose = true       //���� ���. ���.
	);
/*
 �������� ���������� ������������//���������
 */
int Match_Descriptors(
	double * a,			      //���������� a (�����. �������)
	double * b,               //���������� b (��� �����������)
	double thresh1 = 0.01,    //����������� ��� ��������� ������������, ������������ �� 0.01
	double thresh2 = 0.0,     //����������� ��� ��������� ������������ ��� ���������
	bool adaptive = true,     //� �����������, �����. = true
	int desc_size = 8,        //������ �����������, ����� = 8
	bool verbose = false      //���� ���. ���. ����� = false
	);

#endif