#include <stdio.h>
#include <iostream>
#include <string>
#include "methodsForImage.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <ctime>
#include "list.h"

using namespace cv;
using namespace std;

// ������ ����������� �����������.
IplImage *img = 0;

// ���������� ������� ���������. �� ����� (�;�) �� (� + width - 2; y + height - 2).
inline void fillingMatrix(bool **matrix, int x, int y, int width, int height);

// ����������� ������� �� ����������� y ������.
inline bool searchCellOnHorizontal(bool **matrix, int y);

int main(int argc, char* argv[])
{
	float startTime =  clock();

	// � ������ ������� ����� ������� ���������� ���� �����.
	CellCoordinate cellCoordinate[1200];
	int sizeCellCoordinate = 0;

	char* filename = argc >= 2 ? argv[1] : "Image.jpg";
    // �������� ��������.
    img = cvLoadImage(filename,1); 
	// ������, ���� ������������ �� �����������.
    assert(img != 0);

	// ����, � ������� ������������ ���� �����������.
	cvNamedWindow("Window1", CV_WINDOW_AUTOSIZE);

	// ������� ����������� �� �����.
	cvShowImage("Window1", img);

	// �������� �������, � ������� �� ����� ������� ���������� � ���, ������ �� ������ ��� ���.
	bool **matrix = new bool *[img->height];
	for (int i = 0; i < img->height; i++)
	{
		matrix[i] = new bool [img->width];
		memset(matrix[i], 0, sizeof(bool) * img->width);
	}
	
	List *list = new List;
	// ����� ���������� ���� ����� � ���������.	
	for (int y = 0; y < img->height; ++y) 
	{
        uchar* ptr = (uchar*) (img->imageData + y * img->widthStep);
		int width = 0;
		int height = 0;
        for(int x = 0; x < img->width; ++x) 
		{
			if (matrix[y][x] == 1)
				continue;
			else if (isCell(cellCoordinate, sizeCellCoordinate, img, x, y, width, height, 180, 9))
			{
				if (width < 1500 && height < 500 && checkRepetition(cellCoordinate, x, y, width, height, sizeCellCoordinate))
				{
					list->addElement(cellCoordinate, img, x, y, width, height, sizeCellCoordinate);

					fillingMatrix(matrix, x, y, width, height);
				}
			}
		}
    }

	int tempSizeCellCoordinate = sizeCellCoordinate;
	for (int i = 0; i < tempSizeCellCoordinate; i++)
	{
		if (matrix[cellCoordinate[i].yCoordinate + int(cellCoordinate[i].height * 1.5)][cellCoordinate[i].xCoordinate + int(cellCoordinate[i].width * 1.5)] == 0 
			&& searchCellOnHorizontal(matrix, cellCoordinate[i].yCoordinate + int(cellCoordinate[i].height * 1.5)))
		{
			addCellInArray(cellCoordinate, cellCoordinate[i].xCoordinate, cellCoordinate[i].yCoordinate + cellCoordinate[i].height, cellCoordinate[i].width, cellCoordinate[i].height, sizeCellCoordinate);
			fillingMatrix(matrix, cellCoordinate[i].xCoordinate, cellCoordinate[i].yCoordinate + cellCoordinate[i].height, cellCoordinate[i].width, cellCoordinate[i].height);
		}
		
		if (matrix[cellCoordinate[i].yCoordinate - int(cellCoordinate[i].height * 0.5)][cellCoordinate[i].xCoordinate + int(cellCoordinate[i].width * 1.5)] == 0 
			&& searchCellOnHorizontal(matrix, cellCoordinate[i].yCoordinate - int(cellCoordinate[i].height * 0.5)))
		{
			addCellInArray(cellCoordinate, cellCoordinate[i].xCoordinate, cellCoordinate[i].yCoordinate - cellCoordinate[i].height, cellCoordinate[i].width, cellCoordinate[i].height, sizeCellCoordinate);
			fillingMatrix(matrix, cellCoordinate[i].xCoordinate, cellCoordinate[i].yCoordinate - cellCoordinate[i].height, cellCoordinate[i].width, cellCoordinate[i].height);
		}
	}
	//*/
	/*
	for (int y = 0; y < img->height; y++) 
	{
        uchar* ptr = (uchar*) (img->imageData + y * img->widthStep);
		int width = 0;
		int height = 0;
        for(int x = 0; x < img->width; x++) 
		{
			if (matrix[y][x] == 1)
				continue;
			else if (isBotRightCorner(img, x, y, 200, 15))
			{
				cout << x << " " << y << endl;
			}
		}
    }
	//*/
	
	// �������� ������ � ��������� ��� ��������� �����������.
	for (int i = 0; i < sizeCellCoordinate; i++)
	{
		// ��������� ���.
		cvSetImageROI(img, cvRect(cellCoordinate[i].xCoordinate, cellCoordinate[i].yCoordinate, 
			cellCoordinate[i].width + 3, cellCoordinate[i].height + 3));

		// �������� ������ ��� ���������� ����������� �����������.
		// ������� cvGetSize ���������� ������ (width) � ������ (height) ���.
		IplImage *sub_img = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);

		// ����������� ����������� ������� � ��������� �����
		cvCopy(img, sub_img, NULL);

		string imgNameString = createImageName(cellCoordinate, i);

		char imgName[24];
		for (int i = 0; i < 24; i++)
			imgName[i] = imgNameString[i];

		// ��������� ����������� � ����.
		cvSaveImage(imgName, sub_img);

		// ������� ���.
		cvResetImageROI(img);
	}
	
	float endTime = clock();

	cout << "Time of work (sec): " << (endTime - startTime) / 1000 << endl;

	// ������� ������� ����� �������.
	waitKey(0);
	return 0;
}

inline void fillingMatrix(bool **matrix, int x, int y, int width, int height)
{
	for (int i = y; i < y + height - 2; i++)
		for (int j = x; j < x + width - 2; j++)
			matrix[i][j] = 1;
}


inline bool searchCellOnHorizontal(bool **matrix, int y)
{
	for (int i = 0; i < img->width; i++)
		if (matrix[y][i] == 1)
			return true;

	return false;
}