//---> КОНТРОЛЬ МЫШИ
#ifndef MOUSE_CONTROL
#define MOUSE_CONTROL

#include "main_process.h"

#include <iostream>
#include <string>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace std;
using namespace cv;

void onMouse(int event, int x, int y, int, void * mouse_data);

class Mouse
{
public:
	//флаги состояния/управления
	bool m_is_dragged;           //determines if mouse is dragged or not
	bool m_is_clicked;           //determines if mouse was clicked once
	bool is_roi_selected;        //determines if ROI selected or not
	bool save_roi_to_file;       //save ROI to file or not
	bool is_grayscale;           //determines if images are converted to grayscale, default = true 

	//параметры состояния
	Point start_pt;              //start point of the selection, gets value when mouse is clicked
	Point end_pt;                //end point of the selection, gets value when mouse is released
	int radius;                  //gap between selection border and real selection
	string winname;              //name of window, where mouse operates
	
	//дополнительные параметры состояния
	int t_down_up;               //amount of time, passed between click and release
	int t_up_down;               //amount of time, passed between release and next click

	//указатели на контейнеры хранения данных
	Mat src;                     //source image, cannot be changed
	Mat dst;                     //destination image (buffer)
	Mat roi;                     //ROI for storing

	//параметры отображения
	Scalar selection_color;      //color of selection
	int line_thickness;          //thickness of selection
	bool verbose;                //доп. инф., умолч = false

	Mouse(
		Mat src, 
		Mat dst, 
		string winname, 
		int radius = 12,
		bool is_grayscale = true,
		bool save_roi_to_file = true,
		Scalar selection_color = Scalar(255, 0, 0),
		int line_thickness = 1,
		bool verbose = false
		);

	int mouse_chk_coord();
};

#endif