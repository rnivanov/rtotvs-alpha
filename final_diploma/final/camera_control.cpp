//---> КОНТРОЛЬ КАМЕРЫ

#include "camera_control.h"

int cam_init( 
	VideoCapture cam,        //камера
	Mat src,                 //буффер
	int max_frames_skipped,  //по умолч. = 100 максимум пропущенных кадров
	bool verbose             //флаг до. инф, по умолч. включен
	)
{
	/*  Проверка доступности камеры:  */
	if( !cam.isOpened() )
	{
		if( verbose )
			cout << ERROR_100_V << endl;
		return 100;
	}

	/*  Проверка чтения с камеры (не более max_frames_skipped кадров может быть пропущено)  */
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

	/*  Успех!  */
	if( verbose )
		cout << "Success! cam is ready and reading data" << endl;
	return 0;

}

double avg_fps_noweight(
	VideoCapture cam,             //камера
	Mat src,                      //буффер для ввода
	Mat dst,                      //буффер для вывода
	string win_name,              //имя окна вывода
	int max_frames,               //максимум кадров для расчета, умолч = 36
	Point org,                    //точка, к которой привязывается вывод fps на экран, умолч. = (10,20), (-1,-1) = не выводить
	int tip_thickness,            //толщина текста fps, умолч. = 5
	int tip_text_size,            //размер текста fps, умолч. = 1
	Scalar tip_clr,               //цвет вывода fps, умолч. = Green = (0, 255, 0)
	int delay,                    //задержка вывода, 0 = игнорируется, умолч. = 10
	bool verbose                  //флаг доп. инф, по умолч. включен
	)
{
	assert((org.x >= 0 && org.y >= 0) || (org.x == -1 && org.y == -1));
	assert(delay >= 0);

	double t = 0;                 //буффер время
	double sum_t = 0;             //сумма время
	string fps_s_buf = "";        //строка для вывода fps

	for( int i = 0; i < max_frames ; i++)
	{
		t = (double)getTickCount();
		
		/*  Чтение кадра с проверкой  */
		if( !cam.read(src) )
		{
			if( verbose )
				cout << ERROR_105_V << endl;
			return 105.0;        //все ОК, 105 fps может быть вряд-ли =)
		}

		src.copyTo(dst);
		
		/*  Вывод плюшек  */
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