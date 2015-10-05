//---> �������� ������

#include "camera_control.h"

int cam_init( 
	VideoCapture cam,        //������
	Mat src,                 //������
	int max_frames_skipped,  //�� �����. = 100 �������� ����������� ������
	bool verbose             //���� ��. ���, �� �����. �������
	)
{
	/*  �������� ����������� ������:  */
	if( !cam.isOpened() )
	{
		if( verbose )
			cout << ERROR_100_V << endl;
		return 100;
	}

	/*  �������� ������ � ������ (�� ����� max_frames_skipped ������ ����� ���� ���������)  */
	int i = 0;
	while( i++ < max_frames_skipped && !cam.read(src) )
	{
		if( verbose )
			cout << "frame " << i << " was skipped" << endl;
	}
	if( i >= max_frames_skipped )
	{
		if( verbose )
			cout << ERROR_105_V << i << " frames skipped" << endl;
		return 105;
	}

	/*  �����!  */
	if( verbose )
		cout << "Success! cam is ready and reading data" << endl;
	return 0;

}

double avg_fps_noweight(
	VideoCapture cam,             //������
	Mat src,                      //������ ��� �����
	Mat dst,                      //������ ��� ������
	string win_name,              //��� ���� ������
	int max_frames,               //�������� ������ ��� �������, ����� = 36
	Point org,                    //�����, � ������� ������������� ����� fps �� �����, �����. = (10,20), (-1,-1) = �� ��������
	int tip_thickness,            //������� ������ fps, �����. = 5
	int tip_text_size,            //������ ������ fps, �����. = 1
	Scalar tip_clr,               //���� ������ fps, �����. = Green = (0, 255, 0)
	int delay,                    //�������� ������, 0 = ������������, �����. = 10
	bool verbose                  //���� ���. ���, �� �����. �������
	)
{
	assert((org.x >= 0 && org.y >= 0) || (org.x == -1 && org.y == -1));
	assert(delay >= 0);

	double t = 0;                 //������ �����
	double sum_t = 0;             //����� �����
	string fps_s_buf = "";        //������ ��� ������ fps

	for( int i = 0; i < max_frames ; i++)
	{
		t = (double)getTickCount();
		
		/*  ������ ����� � ���������  */
		if( !cam.read(src) )
		{
			if( verbose )
				cout << ERROR_105_V << endl;
			return 105.0;        //��� ��, 105 fps ����� ���� ����-�� =)
		}

		src.copyTo(dst);
		
		/*  ����� ������  */
		if( sum_t > 0 && org.x != -1)
			putText(dst, fps_s_buf, org, tip_thickness, tip_text_size, tip_clr);
		
		if( i%2 == 0 && src.rows > 20)
			putText(dst, "testing...", Point(src.cols/2, 20), tip_thickness, tip_text_size, Scalar(0,0,255));

		imshow(win_name, dst);

		if( delay )
			waitKey(delay);

		t = ((double)getTickCount() - t)/getTickFrequency()*1000;

		if( verbose )
			cout << i << ". time elapsed: " << t << endl;

		sum_t += t;

		fps_s_buf = DoubleToString(1000/t);
	}

	double fps = (max_frames*1000)/sum_t;
	if ( verbose )
		cout << "average fps without weight is: " << fps << endl;

	if (fps < MIN_FPS_CONSTRAINT)
	{
		if( verbose )
			cout << ERROR_110_V <<endl;
		return 110.0;
	} else
		return fps;
}