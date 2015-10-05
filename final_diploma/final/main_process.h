//ќ—Ќќ¬Ќјя ѕ–ќ√–јћћј

//TODO:
//очистка пам€ти (списка дескрипторов, самих дескрипторов) после каждого прохода основного цикла

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

//‘”Ќ ÷»»  ќЌ“–ќЋя
int main_process(
	int CAM_id = 0,               //номер камеры: 0 = default, 1 = secondary, etc
	int GAUSS_ksize = 9,          //размер гауссова €дра
	int MED_ksize = 0,            //размер медианного €дра
	int FAST_thresh = 25,         //FAST threshold 
	int DESC_thresh = 10,         //descriptor matching threshold, 10 = 0.01 * 1000
	string winname = "out",       //им€ окна вывода
	bool verbose = true,          //по умолч. = true: флаг вывода сообщений
	bool grayscale = true         //по умолч. = true: не гарантируетс€ работа алгоритма с RGB
	);



#endif