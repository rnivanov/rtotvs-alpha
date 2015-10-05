//--->ОБРАБОТКА ИЗОБРАЖЕНИЙ ПО АЛГОРИТМУ

//TODO:
//очистка памяти (списка дескрипторов, самих дескрипторов) после каждого прохода основного цикла
/* динамический массив протаскивается из CalcHaarResponse до самого верха  */
/* изменить константу для размера дескриптора на переменную  */
//проверка макс значения в функции Calc_Image_Sum()

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

/*  Основная функция обработки изображения
	- фильтрация шума
	- поиск о.т.
	- расчет дескрипторов
Возвращает указатель на список о.т. с рассчитанными дескрипторами
*/
Keypoint * imgproc_main(         
	Mat src,                     //вход (неизменяемый)
	Mat dst,                     //выход (для отображения)
	int FAST_thresh = 50,        //параметр FAST
	string winname = "imgproc",  //имя окна вывода
	int gauss_ksize = 0,         //размер ядра гауссова фильтра (0, 3, 4,..k>0), 0 = не использ.
	int med_ksize = 0,           //размер ядра медианного фильтра (0, 3, 4,..k>0), 0 = не использ.
	bool verbose = false         //доп. инф.
	);

/*  
Функция отрисовки найденных точек для структуры Keypoint 
(аналог CV::drawKeypoints)
работает с пустыми изображениями,
возвращает коды ошибок см. выше
*/
int drawKeypointsList(
	Mat src,                      //вход (м.б. пустым, но нежелательно)
	Keypoint * points,            //список точек
	Mat dst,                      //выход (в общем случае совпадает со входом)
	int radius = 1,               //радиус окрестности точки > 0
	Scalar color = Scalar::all(255),//цвет отрисовки
	int flags = 1,                //два флага 0 и 1, см. выше
	int thickness = 1,            //толщина линии, -1 = закрасить
	bool verbose = false          //флаг доп. инф.
	);

/*  
Цикл по всем точкам, найденным детектором, 
расстояние от которых до границы изображения не превышает radius.
Такие точки записываются в связный список (память выделяется внутри функции), 
и для них считаются дескрипторы.
Если точки не были найдены, то возвр. NULL, иначе указатель на начало списка
**вызывается из функции imgproc_main
***вызывает функцию list_points
*/
Keypoint * list_points(     
	Mat src,                     //вход (неизменяемый)
	vector <KeyPoint> points,    //точки, найденные детектором (FAST)
	bool verbose = false,        //доп. инф.
	int radius = 12,             //максимальный радиус максимальной окрестности о.т.
	int desc_size = 3,           //(???) число различных окрестностей (???)
	int desc_step = 4            //(???) шаг увеличения размера окрестности (???)
	);

/*
Расчет дескриптора для заданной точки
**вызывается из функции list_points
***вызывает функцию Calc_Descriptor
 - Выполняется выделение памяти для массива указателей 
типа double размерности size.
 - Выполняется вызов функции для расчета каждого вектора отклика
 - Выполняется усреднение и нормировка для дескриптора
 - освобождается память size-1
Возвращается указатель на динамический массив double,
!NULL возвращаться не должен!
*/

double * Calc_Descriptor(
	Mat src,                    //вход (неизменяемый)
	Point center,               //центр окрестности
	int desc_size = 3,          //число различных окрестностей
	int desc_step = 4,          //шаг увеличения размера окрестности
	bool verbose = false        //флаг доп.инф.
	);
/*
Расчет вектора откликов для формирования дескриптора
**вызывается из функции Calc_Descriptor
***вызывает функцию Calc_Image_Sum
Считает массив откликов типа double, возвращает указатель на него
!NULL возвращаться не должен!
Выделяет динамически память на массив откликов
*/

double * Calc_Haar_Response(
	Mat src,                    //вход (неизменяемый)
	Point center,               //центр окрестности
	int radius = 8,             //радиус окрестности
	bool verbose = false        //флаг доп.инф.
	);

/*
Считает сумму яркостей пикселей изображения src
и сохраняет эту сумму в int
! Аккуратно с int ! возможно переполнение
*/
int Calc_Image_Sum (Mat src, bool verbose = false);

/*
Очистка памяти
*/
void clear_keypoints(Keypoint *img_pts);

#endif