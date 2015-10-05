//---> КОНТРОЛЬ КАМЕРЫ
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

//---> КОНТРОЛЬ КАМЕРЫ
int cam_init(                     /*  Проверка доступности устройства  */
	VideoCapture cam,             //камера
	Mat src,                      //буффер
	int max_frames_skipped = 100, //максимум пропущенных кадров
	bool verbose = true           //флаг доп. инф
	);

double avg_fps_noweight(          /*  Вычисление среднего fps без нагрузки  */
	VideoCapture cam,             //камера
	Mat src,                      //буффер для ввода
	Mat dst,                      //буффер для вывода
	string win_name = "test",     //имя окна вывода
	int max_frames = 36,          //максимум кадров для расчета
	Point org = Point(10,20),     //точка, к которой привязывается вывод fps на экран, (-1,-1) = не выводить
	int tip_thickness = 5,        //толщина текста fps
	int tip_text_size = 1,        //размер текста fps
	Scalar tip_clr = Scalar(0, 255, 0),//цвет вывода fps
	int delay = 10,               //задержка вывода, 0 = игнорируется
	bool verbose = true           //флаг доп. инф
	);

#endif