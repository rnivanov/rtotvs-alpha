//ОСНОВНАЯ ПРОГРАММА

#include "main_process.h"

void FAST_trackbar (int, void *) {;};
void DESC_trackbar (int, void *) {;};

int main_process(
	int CAM_id,               //номер камеры: 0 = default, 1 = secondary, etc
	int GAUSS_ksize,          //размер гауссова ядра
	int MED_ksize,            //размер медианного ядра
	int FAST_thresh,          //FAST threshold, default = 25
	int DESC_thresh,          //descriptor matching threshold, default = 10 = 0.01 * 1000
	string winname,           //имя окна вывода, по умолч. = "out"
	bool verbose,             //по умолч. = true: флаг вывода сообщений
	bool grayscale            //по умолч. = true: не гарантируется работа алгоритма с RGB
	)
{
	/*  Переменные ввода/вывода  */
	VideoCapture cam(CAM_id); //объект для работы с камерой
	Mat src;                  //вход с камеры (чистый буффер, не меняется)
	Mat dst;                  //вывод в поток/запись в файл с плюшками
	
	/*  Создание и позиционирование окон приложения  */
	string winname_roi = "selection";
	string winname_trackbars = "trackbars";

	namedWindow(winname, WINDOW_AUTOSIZE);
	namedWindow(winname_roi, WINDOW_AUTOSIZE);
	namedWindow(winname_trackbars, WINDOW_AUTOSIZE);
	
	moveWindow(winname, 0, 0);
	moveWindow(winname_roi, 640 + 20, 0);                  //пока константы
	moveWindow(winname_trackbars, 0, 480 + 40);            //пока константы
	resizeWindow(winname_trackbars, 640, 80);

	createTrackbar("FAST_thr", winname_trackbars, &FAST_thresh, 80, FAST_trackbar);
	createTrackbar("DESC_thr", winname_trackbars, &DESC_thresh, 30, DESC_trackbar);

	int ERROR_CODE = 0;                  //сюда сбрасывается код ошибки (нужно при откл. verbose)
	
	/*  Тестинг и проскакивание плохих фреймов, 0 = success  */
	ERROR_CODE = cam_init(cam, src, 100, verbose);
	if( ERROR_CODE != 0 )
		return ERROR_CODE;

	/*  Считаем средний fps без нагрузки. !fps должно быть double!  */
	
	double fps_noweight;                 //средний fps без нагрузки (без доп. расчетов)
	fps_noweight = avg_fps_noweight(cam, src, dst, winname);

	ERROR_CODE = (int)fps_noweight;
	if(ERROR_CODE >= 100)
		return ERROR_CODE;

	/*  Инициализация обработчика событий мыши  */

	cam.read(src);
	Mouse mouse = Mouse(src, dst, winname);
	cv::setMouseCallback(winname, onMouse, (void*) &mouse);
	
	/*  Переменные для списков особых точек  */

	Keypoint * roi_pts = NULL;           //для изображения объекта
	Keypoint * img_pts = NULL;           //для кадра-изображения
	Keypoint * matches = NULL;           //для совпадений

	/*  Основной цикл  */

	while(true)
	{
		/*  Чтение кадра-изображения с камеры */

		if( !cam.read(src) )
		{
			if( verbose )
				cout << ERROR_105_V << endl;
			return 105;
		}

		/*  Проверка, производится ли захват изображения объекта оператором,
			ожидание конца захвата изображения  */

		while( mouse.m_is_dragged )
		{
			if( waitKey(10) == 27 )
				return 0;
		}

		/*  Обработка захваченного изображения, если захват производился  */

		if( mouse.is_roi_selected ) 
		{
			Mat roi_template;                                //для отображения точек на ROI
			mouse.roi.copyTo(roi_template);                  //для отображения точек на ROI
			
			if( roi_pts != NULL ) 
				clear_keypoints(roi_pts);                    //очистка памяти
			
			roi_pts = imgproc_main(mouse.roi, roi_template, FAST_thresh, winname_roi, GAUSS_ksize, MED_ksize, true);
			
			imshow(winname_roi, roi_template);               //для отображения точек на ROI
			mouse.is_roi_selected = false;                   //!!!сброс флага для отображения точек на ROI и очистки памяти
		}
		 
		/*  Обработка всего кадра-изображения  */

		if( grayscale && src.type() != CV_8U )
			cvtColor(src, dst, CV_BGR2GRAY); 
		else
			src.copyTo(dst);

		img_pts = imgproc_main(src, dst, FAST_thresh, winname, GAUSS_ksize, MED_ksize, true); 
		
		/*  Сопоставление дескрипторов, маппинг совпадений на dst  */

		if( grayscale )
			cvtColor(dst, dst, CV_GRAY2BGR);
		
		if( !mouse.roi.empty() && roi_pts && img_pts )
			matches = matches_BF_simple(dst, roi_pts, img_pts, DESC_thresh/1000.0, 0, false);
		
		/*  Запись истории  */

		/*  Отбрасывание ложных совпадений  */

		/*  Очистка памяти (перенести после добавления записи истории)  */

		//clear_keypoints(matches);
		clear_keypoints(img_pts);


		/*  Отображение измененного кадра-изображения  */

		imshow(winname, dst);

		/*  Блок управления программой:  */
		if( waitKey(10) == 27 ) //ESC = выход
			return 0;

	}

	return 0;
}
