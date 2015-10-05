//�������� ���������

#include "main_process.h"

void FAST_trackbar (int, void *) {;};
void DESC_trackbar (int, void *) {;};

int main_process(
	int CAM_id,               //����� ������: 0 = default, 1 = secondary, etc
	int GAUSS_ksize,          //������ �������� ����
	int MED_ksize,            //������ ���������� ����
	int FAST_thresh,          //FAST threshold, default = 25
	int DESC_thresh,          //descriptor matching threshold, default = 10 = 0.01 * 1000
	string winname,           //��� ���� ������, �� �����. = "out"
	bool verbose,             //�� �����. = true: ���� ������ ���������
	bool grayscale            //�� �����. = true: �� ������������� ������ ��������� � RGB
	)
{
	/*  ���������� �����/������  */
	VideoCapture cam(CAM_id); //������ ��� ������ � �������
	Mat src;                  //���� � ������ (������ ������, �� ��������)
	Mat dst;                  //����� � �����/������ � ���� � ��������
	
	/*  �������� � ���������������� ���� ����������  */
	string winname_roi = "selection";
	string winname_trackbars = "trackbars";

	namedWindow(winname, WINDOW_AUTOSIZE);
	namedWindow(winname_roi, WINDOW_AUTOSIZE);
	namedWindow(winname_trackbars, WINDOW_AUTOSIZE);
	
	moveWindow(winname, 0, 0);
	moveWindow(winname_roi, 640 + 20, 0);                  //���� ���������
	moveWindow(winname_trackbars, 0, 480 + 40);            //���� ���������
	resizeWindow(winname_trackbars, 640, 80);

	createTrackbar("FAST_thr", winname_trackbars, &FAST_thresh, 80, FAST_trackbar);
	createTrackbar("DESC_thr", winname_trackbars, &DESC_thresh, 30, DESC_trackbar);

	int ERROR_CODE = 0;                  //���� ������������ ��� ������ (����� ��� ����. verbose)
	
	/*  ������� � ������������� ������ �������, 0 = success  */
	ERROR_CODE = cam_init(cam, src, 100, verbose);
	if( ERROR_CODE != 0 )
		return ERROR_CODE;

	/*  ������� ������� fps ��� ��������. !fps ������ ���� double!  */
	
	double fps_noweight;                 //������� fps ��� �������� (��� ���. ��������)
	fps_noweight = avg_fps_noweight(cam, src, dst, winname);

	ERROR_CODE = (int)fps_noweight;
	if(ERROR_CODE >= 100)
		return ERROR_CODE;

	/*  ������������� ����������� ������� ����  */

	cam.read(src);
	Mouse mouse = Mouse(src, dst, winname);
	cv::setMouseCallback(winname, onMouse, (void*) &mouse);
	
	/*  ���������� ��� ������� ������ �����  */

	Keypoint * roi_pts = NULL;           //��� ����������� �������
	Keypoint * img_pts = NULL;           //��� �����-�����������
	Keypoint * matches = NULL;           //��� ����������

	/*  �������� ����  */

	while(true)
	{
		/*  ������ �����-����������� � ������ */

		if( !cam.read(src) )
		{
			if( verbose )
				cout << ERROR_105_V << endl;
			return 105;
		}

		/*  ��������, ������������ �� ������ ����������� ������� ����������,
			�������� ����� ������� �����������  */

		while( mouse.m_is_dragged )
		{
			if( waitKey(10) == 27 )
				return 0;
		}

		/*  ��������� ������������ �����������, ���� ������ ������������  */

		if( mouse.is_roi_selected ) 
		{
			Mat roi_template;                                //��� ����������� ����� �� ROI
			mouse.roi.copyTo(roi_template);                  //��� ����������� ����� �� ROI
			
			if( roi_pts != NULL ) 
				clear_keypoints(roi_pts);                    //������� ������
			
			roi_pts = imgproc_main(mouse.roi, roi_template, FAST_thresh, winname_roi, GAUSS_ksize, MED_ksize, true);
			
			imshow(winname_roi, roi_template);               //��� ����������� ����� �� ROI
			mouse.is_roi_selected = false;                   //!!!����� ����� ��� ����������� ����� �� ROI � ������� ������
		}
		 
		/*  ��������� ����� �����-�����������  */

		if( grayscale && src.type() != CV_8U )
			cvtColor(src, dst, CV_BGR2GRAY); 
		else
			src.copyTo(dst);

		img_pts = imgproc_main(src, dst, FAST_thresh, winname, GAUSS_ksize, MED_ksize, true); 
		
		/*  ������������� ������������, ������� ���������� �� dst  */

		if( grayscale )
			cvtColor(dst, dst, CV_GRAY2BGR);
		
		if( !mouse.roi.empty() && roi_pts && img_pts )
			matches = matches_BF_simple(dst, roi_pts, img_pts, DESC_thresh/1000.0, 0, false);
		
		/*  ������ �������  */

		/*  ������������ ������ ����������  */

		/*  ������� ������ (��������� ����� ���������� ������ �������)  */

		//clear_keypoints(matches);
		clear_keypoints(img_pts);


		/*  ����������� ����������� �����-�����������  */

		imshow(winname, dst);

		/*  ���� ���������� ����������:  */
		if( waitKey(10) == 27 ) //ESC = �����
			return 0;

	}

	return 0;
}
