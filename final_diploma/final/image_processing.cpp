//--->ОБРАБОТКА ИЗОБРАЖЕНИЙ ПО АЛГОРИТМУ

#include "image_processing.h"

Keypoint * imgproc_main(
	Mat src,                     //вход (неизменяемый)
	Mat dst,                     //выход (для отображения)
	int FAST_thresh,             //параметр FAST
	string winname,              //имя окна вывода, умолч. = "imgproc"
	int gauss_ksize,             //размер ядра гауссова фильтра (0, 3, 4,..k>0), 0 = не использ.
	int med_ksize,               //размер ядра медианного фильтра (0, 3, 4,..k>0), 0 = не использ.
	bool verbose                 //доп. инф. умолч. false
	)
{
	//проверки
	assert( !src.empty() );
	assert( gauss_ksize == 0 || gauss_ksize > 2 );
	assert( med_ksize == 0 || med_ksize > 2 );

	//преобразования
	if( src.type() != CV_8U )
		cvtColor(src, src, CV_BGR2GRAY);

	//фильтрация шума
	if( med_ksize != 0)
		cv::medianBlur(src, src, med_ksize);
	if( gauss_ksize != 0)
		cv::GaussianBlur(src, src, Size(gauss_ksize, gauss_ksize), 0, 0);

	//поиск о.т. на изобр.
	vector<KeyPoint> FAST_points;
	cv::FAST(src, FAST_points, FAST_thresh);

	//if( verbose )
		//drawKeypoints(src, FAST_points, dst, Scalar::all(255), DrawMatchesFlags::DRAW_OVER_OUTIMG);

	//расчет дескрипторов о.т. 	
	Keypoint *points = list_points(src, FAST_points);

	if( verbose )
		drawKeypointsList(src, points, dst, 3);
	
	//возврат массива о.т. с рассчитанными дескрипторами
	if( points == NULL )
		return NULL;
	else
		return points;
}

int drawKeypointsList(
	Mat src,                      //вход (м.б. пустым, но нежелательно)
	Keypoint * points,            //список точек
	Mat dst,                      //выход (в общем случае совпадает со входом)
	int radius,                   //радиус окрестности точки > 0, умолч. = 1
	Scalar color,                 //цвет отрисовки, умолч. = белый
	int flags,                    //два флага 0 и 1, умолч. = 1
	int thickness,                //толщина линии, -1 - закрасить, умолч. = 1
	bool verbose                  //флаг доп. инф. умолч. = false
	)
{
	
	assert( radius > 0 );
	assert( thickness == -1 || thickness > 0 );

	if( points == NULL || points->descriptor == NULL )
	{
		if( verbose )
			cout << ERROR_150_V << endl;
		return 150;
	}

	/*  Тяжелый кусок кода для обработки ситуации, когда входная матрица пуста  */
	if( src.empty() && flags == DRAW_ON_EMPTY_FIELD )
	{
		//ищем максимум для координат центров точек и проверяем, есть ли точки на границах
		bool pts_on_borders = false;
		int max_x = 0;
		int max_y = 0;
		for(Keypoint * it = points->next ; it != NULL; it = it->next)
		{
			if( it->center.x > max_x )
				max_x = it->center.x;
			if( it->center.y > max_y)
				max_y = it->center.y;
			if ( pts_on_borders == false && (it->center.x < radius || it->center.y < radius) )
				pts_on_borders = true;
		}
		if (max_x == 0 || max_y == 0)
		{
			if( verbose )
				cout << ERROR_151_V << endl;
			return 151;
		}
		//генерим пустую матрицу, исходя из найденных координат
		if( pts_on_borders ) 
		{
			dst = Mat( max_y + 3 * radius, max_x + 3 * radius, CV_8U );
			//сдвиг координат точек для случая, когда точки на границах
			for(Keypoint * it = points->next ; it != NULL; it = it->next)
			{	
				it->center.x += radius;
				it->center.y += radius;
			}
		}
		else
			dst = Mat( max_y + 2 * radius, max_x + 2 * radius, CV_8U );
	}
	/*  нормальный случай  */
	if ( flags == DRAW_ON_EMPTY_FIELD )
		dst = Mat(src.rows, src.cols, src.type());

	/* Если с матрицей все в порядке, то рисуем точки  */
	if ( flags == DRAW_ON_OUTIMAGE )
	{
		for( Keypoint * it = points ; it != NULL ; it = it->next ) 
			circle(dst, it->center, radius, color, thickness);

	}
	return 0;
}

Keypoint * list_points(
	Mat src,                     //вход (неизменяемый)
	vector <KeyPoint> points,    //точки, найденные детектором (FAST)
	bool verbose,                //доп. инф., умолч. = false
	int radius,                  //максимальный радиус максимальной окрестности о.т., умолч = 12
	int desc_size,               //число различных окрестностей, умолч = 3
	int desc_step                //шаг увеличения размера окрестности, умолч = 4
	)
{
	assert( !src.empty() );
	assert( desc_size > 0 && desc_step > 0 );
	assert( radius >= desc_size * desc_step );
	
	Keypoint * end = NULL;
	Keypoint * keys_list = NULL;

	for(int i = 0 ; i < points.size() ; i++)
	{
		
		int x = points[i].pt.x;
		int y = points[i].pt.y;
		
		if (x - radius >= 0 && x + radius < src.cols && y - radius >= 0 && y + radius < src.rows)
		{
			keys_list = new Keypoint;
			keys_list->center.x = x;
			keys_list->center.y = y;
			keys_list->radius = radius;
			keys_list->descriptor = Calc_Descriptor(src, Point(x,y), desc_size, desc_step, false);
			keys_list->next = end;
			end = keys_list;
		}
	}

	/*  Для пустого списка (если все точки детектора были пропущены) возвр. NULL  */
	if (keys_list == NULL)
		return NULL;

	/*  Вывод построенных дескрипторов в консоль  */
	int counter = 0;

	if (verbose)
		for(Keypoint * it = keys_list; it->next != NULL; it = it->next)
		{
			cout << ++counter << ". "<< it->center << endl;
			for(int i = 0; i < 8; i++)
				cout << it->descriptor[i] << " ";
			cout << endl;
		}
	
	/*  Возврат указателя на список  */
	return keys_list;
}

double * Calc_Descriptor(
	Mat src,                    //вход (неизменяемый)
	Point center,               //центр окрестности
	int desc_size,              //число различных окрестностей, умолч = 3
	int desc_step,              //шаг увеличения размера окрестности, умолч = 4
	bool verbose                //флаг доп.инф., умолч = false
	)  
{
	//Выделение памяти для векторов откликов по окрестностям различного радиуса
	double ** vectors = new double *[desc_size];
	
	//Расчет векторов откликов для каждой окрестности
	for(int i = 0 ; i < desc_size ; i++)
	{
		vectors[i] = Calc_Haar_Response(src, center, desc_step*(i+1), verbose);

		assert( vectors[i] != NULL );
	}

	//Расчет покомпонентных средних для полученных векторов откликов
	double sum = 0;

	for(int j = 0 ; j < 8 ; j++)
	{
		for(int i = 1 ; i < desc_size ; i++)
			vectors[0][j] += vectors[i][j];

		vectors[0][j] /= desc_size;

		sum += vectors[0][j];

		if ( verbose )
			cout << "== type = " << j+1 << " avg = " << vectors[0][j] << "==" << endl;
	}

	//Покомпонентная нормировка средних
	for(int i = 0; i < 8 ; i++)
	{
		vectors[0][i] /= sum;

		if ( verbose )
			cout << "norm " << i << ": " << vectors[0][i] << endl; 
	}

	//Удаление ненужных векторов
	for (int i = 1; i < desc_size; i++)
		delete [] vectors[i];

	//Возврат дескриптора
	return vectors[0];
}

double * Calc_Haar_Response(
	Mat src,                    //вход (неизменяемый)
	Point center,               //центр окрестности
	int radius,                 //радиус окрестности
	bool verbose                //флаг доп.инф., умолч = false
	)
{
	//Проверки
	assert( radius > 1 );
	assert( center.x + radius < src.cols && center.x - radius >= 0
		&& center.y + radius < src.rows && center.y - radius >= 0 );

	//Выбирает квадрат вокруг точки (патч)
	Mat square = Mat(src, Rect(center.x - radius, center.y - radius, radius*2, radius*2));

	//Считает четыре суммы по частям квадрата (от лев.верх. по ч.с.)
	int * sum = new int[4];
	for (int i = 0 ; i < 4 ; i++)
		sum[i] = 0;

	Mat lh_patch = Mat(square, Rect(0, 0, radius, radius));          //лев.верх.
	sum[0] = Calc_Image_Sum (lh_patch, verbose);

	Mat rh_patch = Mat(square, Rect(radius, 0, radius, radius));     //прав.верх.
	sum[1] = Calc_Image_Sum (rh_patch, verbose);

	Mat rl_patch = Mat(square, Rect(radius, radius, radius, radius));//прав.нижн.
	sum[2] = Calc_Image_Sum (rl_patch, verbose);

	Mat ll_patch = Mat(square, Rect(0, radius, radius, radius));     //лев.нижн.
	sum[3] = Calc_Image_Sum (ll_patch, verbose);

	//Создает массив откликов: индексы = типы примитивов, значения = отклики
	double *resp_arr = new double[8]; 

	resp_arr[0] = (sum[0] + sum[1]) / (sum[2] + sum[3] + 0.0); //тип 1
	resp_arr[1] = (sum[0] + (sum[1]+sum[3])/2) / 
				(sum[2] + (sum[1]+sum[3])/2 + 0.0); //тип 2
	resp_arr[7] = (sum[1] + (sum[0]+sum[2])/2) / 
				(sum[3] + (sum[0]+sum[2])/2 + 0.0); //тип 8

	resp_arr[4] = (sum[2] + sum[3]) / (sum[0] + sum[1] + 0.0); //тип 5
	resp_arr[3] = (sum[3] + (sum[0]+sum[2])/2) / 
				(sum[1] + (sum[0]+sum[2])/2 + 0.0); // тип 4
	resp_arr[5] = (sum[2] + (sum[1]+sum[3])/2) / 
				(sum[0] + (sum[1]+sum[3])/2 + 0.0); // тип 4

	resp_arr[2] = (sum[0]+sum[3])/(sum[1]+sum[2] + 0.0); //тип 3
	resp_arr[6] = (sum[1]+sum[2])/(sum[0]+sum[3] + 0.0); //тип 7

	if( verbose )
	{
		for(int i = 0; i < 8; i++)
			cout << "type = " << i+1 << " resp = "<< resp_arr[i] << endl;
		cout << endl;
	}

	//Очищает память, выделенную для сумм
	delete [] sum;

	//Возвращает указатель на массив откликов
	assert(resp_arr != NULL);
	return resp_arr;
}

int Calc_Image_Sum (Mat src, bool verbose)
{
	assert(!src.empty());

	int sum = 0;
    for( int i = 0 ; i < src.rows ; ++i)
		for( int j = 0 ; j < src.cols ; ++j )
			sum += src.at<uchar>(i,j);

	if (verbose )
	{
		cout << "patch.size = " << src.size() << "; sum = " << sum << endl;
	}

	return sum;
}

void clear_keypoints(Keypoint *img_pts)
{
	while (img_pts != NULL)
	{
			Keypoint * buf = img_pts->next;
			//temporary
			assert(img_pts->center.x >= 0 && img_pts->center.y >= 0);
			
			if( img_pts->descriptor != NULL )
				delete [] img_pts->descriptor;
			delete img_pts;
			img_pts = buf;
	}
	assert(img_pts == NULL);
}

