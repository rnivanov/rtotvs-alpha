//---> �������� ������
#ifndef CAM_CONTROL
#define CAM_CONTROL

#include "main_process.h"

#include <iostream>
#include <string>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

#define ERROR_100_V "ERROR 100: camera cannot be opened"
#define ERROR_105_V "ERROR 105: cannot read frames from camera: "
#define ERROR_110_V "ERROR 110: ultimate low fps"
#define MIN_FPS_CONSTRAINT 10

using namespace std;
using namespace cv;

//---> �������� ������
int cam_init(                     /*  �������� ����������� ����������  */
	VideoCapture cam,             //������
	Mat src,                      //������
	int max_frames_skipped = 100, //�������� ����������� ������
	bool verbose = true           //���� ���. ���
	);

double avg_fps_noweight(          /*  ���������� �������� fps ��� ��������  */
	VideoCapture cam,             //������
	Mat src,                      //������ ��� �����
	Mat dst,                      //������ ��� ������
	string win_name = "test",     //��� ���� ������
	int max_frames = 36,          //�������� ������ ��� �������
	Point org = Point(10,20),     //�����, � ������� ������������� ����� fps �� �����, (-1,-1) = �� ��������
	int tip_thickness = 5,        //������� ������ fps
	int tip_text_size = 1,        //������ ������ fps
	Scalar tip_clr = Scalar(0, 255, 0),//���� ������ fps
	int delay = 10,               //�������� ������, 0 = ������������
	bool verbose = true           //���� ���. ���
	);

#endif