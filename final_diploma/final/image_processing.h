//--->IMAGE PROCESSING VIA THE MAIN ALGORITHM (detection and descriptors)

//TODO:
//проверка макс значения в функции Calc_Image_Sum()
//correct Gauss_kernel_size 3,5,...
//drawKeypointsList() return values documentation
//correct Calc_Haar_Response to work with descriptors of different length

#ifndef IMG_PROC
#define IMG_PROC

#include "main_process.h"

#include <iostream>
#include <string>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

#define ERROR_150_V "ERROR 150: Keypoint * list is empty"
#define ERROR_151_V "ERROR 151: Keypoint * list has either zero or one point at (0,0)"

#define DRAW_ON_EMPTY_FIELD 0
#define DRAW_ON_OUTIMAGE 1

using namespace std;
using namespace cv;

//User Keypoint structure for generating lists, not regular cv::KeyPoint
#ifndef STRUCT_KEYPOINT
#define STRUCT_KEYPOINT
struct Keypoint 
{
	Point center;
	int radius;
	double * descriptor;
	struct Keypoint * next;
} ;
#endif

/*
Main image processing function provides
	- filtering
	- detection of Keypoints
	- description of keypoints
--------------------------------------------
Returns the pointer to the list of Keypoints 
or NULL, if zero keypoints were detected
--------------------------------------------
IS CALLED BY: main_process();               //see main_process.cpp
CALLS: drawKeypointsList(); list_points()   //see current
*/
Keypoint * imgproc_main(         
	Mat src,                     //in buffer (unchanged)
	Mat dst,                     //out buffer
	int FAST_thresh = 50,        //FAST threshold value
	string winname = "imgproc",  //window name (for display)
	int gauss_ksize = 0,         //Gauss filter Kernel Size, can be 0 (don't use) or >3
	int med_ksize = 0,           //Median filter Kernel Size, can be 0 (don't use) or >3
	bool verbose = false         
	);

/*
Draws Keypoints on given image or on empty area,
size of the area is calculated via the function
-----------------------------------------------
returns 0 on success or ERROR_CODE (see above)
-----------------------------------------------
IS CALLED BY: imgproc_main()            //see current
*/
int drawKeypointsList(
	Mat src,                      //in buffer (can be empty)
	Keypoint * points,            //List of points to draw
	Mat dst,                      //out buffer (most always it will be the same as src)
	int radius = 1,               //radius of point for cv::Circle()
	Scalar color = Scalar::all(255),//color of point for cv::Circle()
	int flags = DRAW_ON_OUTIMAGE, //flags 0 or 1, see above
	int thickness = 1,            //line thickness for cv::Circle(), -1 = fill
	bool verbose = false
	);


/*
Cycles thru all points, found by detector, which are on distance 'radius'
from borders of the image, points are added to the list (memory is allocated in heap)
for any of these points calculates descriptor vector
------------------------------------------------------------------------
returns pointer to the list of Keypoints or NULL, if 0 points were found
------------------------------------------------------------------------
IS CALLED BY: imgproc_main();                //see current
CALLS: Calc_Descriptor();                    //see current
*/
Keypoint * list_points(     
	Mat src,                     //in buffer (unchanged)
	vector <KeyPoint> points,    //points found by detector
	bool verbose = false,        
	int radius = 12,             //max radius of max neighborhood (>= desc_step * desc_size)
	int desc_size = 3,           //amount of different neighborhoods
	int desc_step = 4            //step size to increase radius
	);

/*
Calculates descriptor for a given point,
allocates memory in heap: new double *[size]
for 'size' amount of descriptors, 
then deletes 'size-1' of them
--------------------------------------------------
returns pointer to descriptor, 
cannot be NULL
--------------------------------------------------
IS CALLED BY: list_points();         //see current
CALLS: Calc_Haar_Response();         //see current
*/

double * Calc_Descriptor(
	Mat src,                    //in buffer (unchanged)
	Point center,               //center of the neighborhood
	int desc_size = 3,          //amount of different neighborhoods
	int desc_step = 4,          //step size to increase radius
	bool verbose = false        
	);

/*
Расчет вектора откликов для формирования дескриптора
**вызывается из функции Calc_Descriptor
***вызывает функцию Calc_Image_Sum
Считает массив откликов типа double, возвращает указатель на него
!NULL возвращаться не должен!
Выделяет динамически память на массив откликов
*/
/*
Calculates response to primitive (Haar or non-Haar);
allocates memory in heap for the descriptor of length = 8								<----CORRECT
----------------------------------------------------------
returns a pointer to the array of responses, 
cannot be NULL
-----------------------------------------------------------
IS CALLED BY: Calc_Descriptor()                //see current
CALLS: Calc_Image_Sum()                        //see current
*/
double * Calc_Haar_Response(
	Mat src,                    //in buffer
	Point center,               //center of the neighborhood
	int radius = 8,             //radius of the neighborhood
	bool verbose = false
	);

/*
Calculates the sum of the pixels of the image src
-------------------------------------------------
returns this sum
-------------------------------------------------
IS CALLED BY: Calc_Image_Sum()                //see current
*/
int Calc_Image_Sum (
	Mat src,                    //in buffer
	bool verbose = false        
	);

/*
Clears the list of keypoints
iS CALLED BY: imgproc_main()                  //see imgproc_main.cpp
*/
void clear_keypoints(Keypoint *img_pts);

#endif