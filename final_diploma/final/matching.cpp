//--->��������� ����������

#include "matching.h"

Keypoint * matches_BF_simple(
	Mat dst,                  //������ ��� �����������
	Keypoint * pts_obj,       //Keypoints �� ���������� �����. �������
	Keypoint * pts_img,       //Keypoints �� ������� ����������� ��� ROI
	double thresh1,           //����������� ��� ��������� ������������ (��������� �� 0.01)
	double thresh2,           //����������� ��� ��������� ������������ ��� ��������� (�� ��������)
	bool adaptive,            //� �����������, �����. = true
	int desc_size,            //������ �����������, ����� = 8
	bool verbose              //���� ���. ���., �����. ����-��� true
	)
{
	if( pts_obj == NULL || pts_img == NULL )
		return NULL;

	Keypoint * end = NULL;
	Keypoint * matches = NULL;

	int num_roi_desc = 0;
	int num_matching_desc = 0;

	for( Keypoint * it = pts_obj; it != NULL; it = it->next )        //it - �� �����. �������
	{
		num_roi_desc ++;
		for( Keypoint * jt = pts_img; jt != NULL; jt = jt->next )
		{
	
			if ( Match_Descriptors(it->descriptor, jt->descriptor, thresh1, thresh2, adaptive) == 0 )
			{
				num_matching_desc ++;
				
				if( verbose )
					circle(dst, jt->center, 4, Scalar(255, 0, 0), -1);

				matches = new Keypoint;

				//temporary
				assert(jt->center.x >= 0 && jt->center.y >= 0);

				matches->center = jt->center;
				matches->radius = jt->radius;
				matches->descriptor = new double [desc_size];
				for( int i = 0 ; i < desc_size ; i++)
					matches->descriptor[i] = jt->descriptor[i];
				matches->next = end;
				end = matches;
			}
		}
	}

	//cout << "roi_desc " << num_roi_desc << " matching_desc " << num_matching_desc << endl;
	
	return matches; //�.�. NULL
} 

/*  
����������� ��� �����������
����������� ������� ���� �� �����
*/
int Match_Descriptors(
	double * a,			//���������� a (�����. �������)
	double * b,         //���������� b (��� �����������)
	double thresh1,     //����� ����������������, ������������ �� 0.01
    double thresh2,     //����������� ��� ��������� ������������ ��� ���������
	bool adaptive,      //� �����������, �����. = true
	int desc_size,      //������ �����������, ����� = 8
	bool verbose        //���� ���. ���. ����� = false
	)
{
	
	assert(a != NULL && b != NULL);

	/*
	C���� ���������� �������� �������������� ���������
	|a[0] - b[0]| + |a[1] - b[1]| + |a[desc_size] - b[desc_size]|
	*/
	double sum = 0;
	
	//�������� ������� ����: �������� �� "������" ���������� � �������� thresh1
	
	for(int i = 0 ; i < desc_size; i ++)
		sum += abs(a[i] - b[i]);

	if (sum <= (thresh1 * 2) )
	{
		if ( verbose )
			cout << "!precise match!" << endl;
	
		//������ ������������ �������, ���� ���������� (����������)
		if( adaptive )
		{
			for(int i = 0; i < 8; i ++)
				a[i] = b[i];
		}

		return 0;
	}
	
//������� ���� return ����� ���������� ������ �������
	return -1;

}

