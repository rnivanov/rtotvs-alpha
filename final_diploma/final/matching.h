//--->ОБРАБОТКА СОВПАДЕНИЙ

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

//КОСТЫЛЬ: КАК УБРАТЬ?
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
Возвр. NULL при усл. что один или оба списка пустые или не найдено совпадений,
Возвр. указатель на список точек, для кот. найдено совпадение
*/
Keypoint * matches_BF_simple(
	Mat dst,                  //Буффер для отображения
	Keypoint * pts_obj,       //Keypoints по эталонному изобр. объекта
	Keypoint * pts_img,       //Keypoints по полному изображению или ROI
	double thresh1 = 0.01,    //Погрешность при сравнении дескрипторов
	double thresh2 = 0.0,     //Погрешность при сравнении дескрипторов при поворотах
	bool adaptive = true,     //с подстройкой, умолч. = true
	int desc_size = 8,        //размер дескриптора, умолч = 8
	bool verbose = true       //флаг доп. инф.
	);
/*
 Проверка совпадения дескрипторов//ПРОВЕРИТЬ
 */
int Match_Descriptors(
	double * a,			      //дескриптор a (изобр. объекта)
	double * b,               //дескриптор b (все изображение)
	double thresh1 = 0.01,    //Погрешность при сравнении дескрипторов, тестировался на 0.01
	double thresh2 = 0.0,     //Погрешность при сравнении дескрипторов при поворотах
	bool adaptive = true,     //с подстройкой, умолч. = true
	int desc_size = 8,        //размер дескриптора, умолч = 8
	bool verbose = false      //флаг доп. инф. умолч = false
	);

#endif