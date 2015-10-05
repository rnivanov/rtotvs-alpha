//---> ÊÎÍÒĞÎËÜ ÌÛØÈ

#include "mouse_control.h"

Mouse::Mouse(
	Mat src, 
	Mat dst, 
	string winname,
	int radius,
	bool is_grayscale,
	bool save_roi_to_file,
	Scalar selection_color,
	int line_thickness,
	bool verbose
	)
{
		m_is_dragged = false;
		m_is_clicked = false;
		is_roi_selected = false;
		this->is_grayscale = is_grayscale;
		this->save_roi_to_file = save_roi_to_file;
		this->winname = winname;
		
		assert(!src.empty());

		this->src = src;
		if ( radius > 0 )
			this->radius = radius;
		else 
			this->radius = 0;

		if(is_grayscale && src.type() != CV_8U)
			cvtColor(this->src, this->dst, CV_BGR2GRAY);

		assert( !this->src.empty() && !this->src.empty() );

		t_down_up = 0; 
		t_up_down = 0; 
		start_pt = Point(0,0); 
		end_pt = Point(0,0);
		
		this->selection_color = selection_color;
		this->line_thickness = line_thickness;
		this->verbose = verbose;

		/*//TESTING
		putText(this->dst, "dst", Point(10,20), 5, 1, Scalar::all(255));
		imshow("check_constructor", this->src);
		waitKey(0);
		imshow("check_constructor", this->dst);
		waitKey(0);
		*/
}

int Mouse::mouse_chk_coord()
{
	
	/*  Check if roi was selected  */ //ÑÒĞÀÍÍÎÅ ÓÑËÎÂÈÅ - ÏĞÎÂÅĞÈÒÜ è ÏÎÍßÒÜ, ÷òî ÿ èìåë â âèäó
	if (start_pt.x == end_pt.x || start_pt.y == end_pt.y) // if this condition was failed, that means that click happened
	{
		if( start_pt == end_pt)
			m_is_clicked = true; //set flag
		return -1;
	}

	/*  Check negative values and trim  */
	start_pt.x < 0 ? start_pt.x = 0: start_pt.x;
	start_pt.y < 0 ? start_pt.y = 0: start_pt.y;
	end_pt.x < 0 ? end_pt.x = 0: start_pt.x;
	end_pt.y < 0 ? end_pt.y = 0: start_pt.y;

	/*  Check if the selection exceeds window size and trim  */
	start_pt.x > src.cols ? start_pt.x = src.cols: start_pt.x;
	start_pt.y > src.rows ? start_pt.y = src.rows: start_pt.y;
	end_pt.x > src.cols ? end_pt.x = src.cols: end_pt.x;
	end_pt.y > src.rows ? end_pt.y = src.rows: end_pt.y;

	/*  Swap coordinates, if nessesary  */
	int buf;
	if(start_pt.x > end_pt.x) { 
		buf = start_pt.x; 
		start_pt.x = end_pt.x; 
		end_pt.x = buf; 
	}
	if(start_pt.y > end_pt.y) { 
		buf = start_pt.y; 
		start_pt.y = end_pt.y; 
		end_pt.y = buf; 
	}

	return 0;
}

void onMouse(int event, int x, int y, int, void * mouse_data)
{
	Mouse * mouse = (Mouse *) mouse_data;

	//ÂÀÆÍÎ: ïğè ùåë÷êå ôëàã ROI óáèâàåòñÿ
	if(event == cv::EVENT_LBUTTONDOWN && !mouse->m_is_dragged)
	{
		/*change state, set init coord and save current time */ 
		mouse->m_is_dragged = true;
		mouse->start_pt = Point(x, y);
		mouse->end_pt = mouse->start_pt;       //nessesary
		mouse->t_down_up = cv::getTickCount(); //CHECK conversion from 'int64' to 'int', possible loss of data
		mouse->is_roi_selected = false;        //if we redetermine roi, we must start another one
	}

	/*if user holds LMB and moves mouse*/
	if(event == cv::EVENT_MOUSEMOVE && mouse->m_is_dragged)
	{
		/*save current coords and trace selection region on dst image*/
		
		mouse->end_pt = Point(x, y);
		//DISPLAY
		if(mouse->is_grayscale && mouse->src.type() != CV_8U)
			cvtColor(mouse->src, mouse->dst, CV_BGR2GRAY);
		else
			mouse->src.copyTo(mouse->dst);

		cv::rectangle(mouse->dst, mouse->start_pt, mouse->end_pt, mouse->selection_color, mouse->line_thickness + 2, 8);
		
		if( mouse->radius > 0 )
		{
			Point a;
			Point b;
			if( mouse->start_pt.x > mouse->end_pt.x && mouse->start_pt.y < mouse->end_pt.y )
			{
				a = Point(mouse->start_pt.x - mouse->radius, mouse->start_pt.y + mouse->radius);
				b = Point(mouse->end_pt.x + mouse->radius, mouse->end_pt.y - mouse->radius);
			}
			if( mouse->start_pt.x < mouse->end_pt.x && mouse->start_pt.y > mouse->end_pt.y )
			{
				a = Point(mouse->start_pt.x + mouse->radius, mouse->start_pt.y - mouse->radius);
				b = Point(mouse->end_pt.x - mouse->radius, mouse->end_pt.y + mouse->radius);
			}
			if( mouse->start_pt.x < mouse->end_pt.x && mouse->start_pt.y < mouse->end_pt.y ) 
			{
				a = Point(mouse->start_pt.x + mouse->radius, mouse->start_pt.y + mouse->radius);
				b = Point(mouse->end_pt.x - mouse->radius, mouse->end_pt.y - mouse->radius);
			}
			if( mouse->start_pt.x > mouse->end_pt.x && mouse->start_pt.y > mouse->end_pt.y ) 
			{
				a = Point(mouse->start_pt.x - mouse->radius, mouse->start_pt.y - mouse->radius);
				b = Point(mouse->end_pt.x + mouse->radius, mouse->end_pt.y + mouse->radius);
			}
			cv::rectangle(mouse->dst, a, b, mouse->selection_color, mouse->line_thickness, 4);
		}
		
		imshow(mouse->winname, mouse->dst);
	}

	/*if user released LMB*/
	if(event == cv::EVENT_LBUTTONUP && mouse->m_is_dragged)
	{
		if (mouse->mouse_chk_coord() == 0) //test if coordinates are appropriate and redefine if nessesary
		{
			Point a = mouse->start_pt;
			Point b = mouse->end_pt;
			//cout << "mouse_chk_coord returned 0" << endl;
			if(mouse->is_grayscale && mouse->src.type() != CV_8U)
				cvtColor(mouse->src, mouse->dst, CV_BGR2GRAY);
			mouse->roi = Mat(mouse->dst, Rect(a.x, a.y, b.x-a.x, b.y-a.y));
			mouse->is_roi_selected = true;
			if(mouse->save_roi_to_file)
				imwrite("roi.bmp", mouse->roi);
			imshow("selection", mouse->roi);
		}
		mouse->m_is_dragged = false;
	}

	//EDIT THIS SECTION IN FUTURE, problem: double click is a part of click
	if(mouse->m_is_clicked && event != cv::EVENT_LBUTTONDBLCLK)
	{
		//EDIT THIS TO SET STATE
		if(mouse->verbose)
			cout << "mouse was clicked once" << endl;
		mouse->m_is_clicked = false;
	}
	if(event == cv::EVENT_LBUTTONDBLCLK && !mouse->m_is_dragged)
	{
		if(mouse->verbose)
		{
			cout << "mouse was clicked twice" << endl;
			cout << mouse->m_is_clicked << endl;
		}
	}
}