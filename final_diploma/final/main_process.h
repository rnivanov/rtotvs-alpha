//�������� ���������

//TODO:
//������� ������ (������ ������������, ����� ������������) ����� ������� ������� ��������� �����

#ifndef HEADER_1
#define HEADER_1

#include <iostream>
#include <string>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\features2d\features2d.hpp>

#include "service.h"
#include "camera_control.h"
#include "mouse_control.h"
#include "image_processing.h"
#include "matching.h"

using namespace std;
using namespace cv;

//������� ��������
int main_process(
	int CAM_id = 0,               //����� ������: 0 = default, 1 = secondary, etc
	int GAUSS_ksize = 9,          //������ �������� ����
	int MED_ksize = 0,            //������ ���������� ����
	int FAST_thresh = 25,         //FAST threshold 
	int DESC_thresh = 10,         //descriptor matching threshold, 10 = 0.01 * 1000
	string winname = "out",       //��� ���� ������
	bool verbose = true,          //�� �����. = true: ���� ������ ���������
	bool grayscale = true         //�� �����. = true: �� ������������� ������ ��������� � RGB
	);



#endif