//---> CAMERA CONTROL
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

/*  
Checks if input device (webcam) is availible for capping  frames
and skips all bad frames (lag issue on capping with OpenCV and some webcams)
and returns either 0 on success or ERROR_CODE (see above) on failure
*/
int cam_init(                     
	VideoCapture cam,             //device id (webcam id)
	Mat src,                      //buffer
	int max_frames_skipped = 100, //amount of frames allowed to be skipped before device is decided to be unavailible
	bool verbose = true           
	);

/*
Calculates average input fps and returns it
if fps is lower than MIN_FPS_CONSTRAINT, than ERROR_CODE (see above) is returned
fps tip is added to out buffer
*/
double avg_fps_noweight(          
	VideoCapture cam,             //device id (webcam id))
	Mat src,                      //in buffer (unchanged)
	Mat dst,                      //out buffer
	string win_name = "test",     //name of window, where frames are displayed
	int max_frames = 36,          //max amount of frames for calculating average
	Point org = Point(10,20),     //location of fps tip in window
	int tip_thickness = 5,        //tip font thickness
	int tip_text_size = 1,        //tip font size
	Scalar tip_clr = Scalar(0, 255, 0),//tip font color
	int delay = 10,               //delay between two frames
	bool verbose = true           
	);

#endif