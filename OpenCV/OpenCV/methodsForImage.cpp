#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <string>
#include "methodsForImage.h"

using namespace std;

// Параментры прямоугольников, котрый мы ищем, чтобы обнаружить угол.
int lengthRectangle = 60;
int widthRectangle = 1;

// Определяем является ли пиксель серым или чёрным.
inline bool isGreyOrBlack(int a, int b, int c, int sensitivity)
{
	if (abs(a - b) < 5 && abs(a - c) < 5  && a < sensitivity + 10)
		return true;

	return false;
}

// Определяем является ли пиксель серым или чёрным, но с учётом того, что проверяем для датчика.
inline bool isGreyOrBlackForSensor(IplImage *img, int x, int y, int sensitivity)
{
	uchar* ptr = (uchar*) (img->imageData + y * img->widthStep);
	if (abs(ptr[x * 3] - ptr[x * 3 + 1]) < (sensitivity / 40) && abs(ptr[x * 3] - ptr[x * 3 + 2]) < (sensitivity / 40)  
		&& ptr[x * 3] < sensitivity)
		return true;

	return false;
}

// Определяем является ли пиксель белым.
inline bool isWhite(int a, int b, int c)
{
	if (abs(a - b) < 3 && abs(a - c) < 3 && a > 245)
		return true;

	return false;
}

// Определяем является ли точка основанием левого-верхнего угла.
bool isTopLeftCorner(IplImage *img, int x, int y, int sensitivity, int amountWhitePixels)
{
	// Проверяем границы.
	if (x + lengthRectangle > img->width || x + widthRectangle > img->width 
		|| y + widthRectangle > img->height || y + lengthRectangle > img->height)
		return false;

	int count = 0; // Для подсчёта не чёрных клеток.
	// Для этого проверим наличие двух пересекающихся прямоугольников. 
	// Проверяем наличие горизонтального.
	for (int y2 = y; y2 < y + widthRectangle; y2++) 
	{
		uchar* ptr = (uchar*) (img->imageData + y2 * img->widthStep);
		for (int x2 = x; x2 < x + lengthRectangle; x2++)
		{
			if (!isGreyOrBlack(ptr[x2 * 3], ptr[x2 * 3 + 1], ptr[x2 * 3 + 2], sensitivity))
			{
				count++;
				if (count > amountWhitePixels)
					return false;
			}
		}
	}

	count = 0;
	// Проверяем наличие вертикального.
	for (int y2 = y; y2 < y + lengthRectangle; y2++) 
	{
		uchar* ptr = (uchar*) (img->imageData + y2 * img->widthStep);
		for (int x2 = x; x2 < x + widthRectangle; x2++)
		{
			if (!isGreyOrBlack(ptr[x2 * 3], ptr[x2 * 3 + 1], ptr[x2 * 3 + 2], sensitivity))
			{
				count++;
				if (count > amountWhitePixels)
					return false;
			}
		}
	}

	return true;
}

// Определяем является ли точка основанием правого-верхнего угла.
bool isTopRightCorner(IplImage *img, int x, int y, int sensitivity, int amountWhitePixels)
{

	// Проверяем границы.
	if (x - lengthRectangle < 0 || x - widthRectangle < 0 
		|| y + widthRectangle > img->height || y + lengthRectangle > img->height)
		return false;

	int count = 0; // Для подсчёта не чёрных пикселей.
	// Для этого проверим наличие двух пересекающихся прямоугольников. 
	// Проверяем наличие горизонтального.
	for (int y2 = y; y2 < y + widthRectangle; y2++) 
	{
		uchar* ptr = (uchar*) (img->imageData + y2 * img->widthStep);
		for (int x2 = x; x2 > x - lengthRectangle; x2--)
		{
			if (!isGreyOrBlack(ptr[x2 * 3], ptr[x2 * 3 + 1], ptr[x2 * 3 + 2], sensitivity))
			{
				count++;
				if (count > amountWhitePixels)
					return false;
			}
		}
	}

	count = 0;
	// Проверяем наличие вертикального.
	for (int y2 = y; y2 < y + lengthRectangle; y2++) 
	{
		uchar* ptr = (uchar*) (img->imageData + y2 * img->widthStep);
		for (int x2 = x; x2 > x - widthRectangle; x2--)
		{
			if (!isGreyOrBlack(ptr[x2 * 3], ptr[x2 * 3 + 1], ptr[x2 * 3 + 2], sensitivity))
			{
				count++;
				if (count > amountWhitePixels)
					return false;
			}
		}
	}

	return true;
}

// Аналогично функциии, которая выше.
bool isBotLeftCorner(IplImage *img, int x, int y, int sensitivity, int amountWhitePixels)
{
	// Проверяем границы.
	if (x + lengthRectangle > img->width || x + widthRectangle > img->width 
		|| y - widthRectangle < 0 || y - lengthRectangle < 0)
		return false;

	int count = 0; // Для подсчёта не чёрных пикселей.
	// Для этого проверим наличие двух пересекающихся прямоугольников. 
	// Проверяем наличие горизонтального.
	for (int y2 = y; y2 > y - widthRectangle; y2--) 
	{
		uchar* ptr = (uchar*) (img->imageData + y2 * img->widthStep);
		for (int x2 = x; x2 < x + lengthRectangle; x2++)
		{
			if (!isGreyOrBlack(ptr[x2 * 3], ptr[x2 * 3 + 1], ptr[x2 * 3 + 2], sensitivity))
			{
				count++;
				if (count > amountWhitePixels)
					return false;
			}
		}
	}

	count = 0;
	// Проверяем наличие вертикального.
	for (int y2 = y; y2 > y - lengthRectangle; y2--) 
	{
		uchar* ptr = (uchar*) (img->imageData + y2 * img->widthStep);
		for (int x2 = x; x2 < x + widthRectangle; x2++)
		{
			if (!isGreyOrBlack(ptr[x2 * 3], ptr[x2 * 3 + 1], ptr[x2 * 3 + 2], sensitivity))
			{
				count++;
				if (count > amountWhitePixels)
					return false;
			}
		}
	}

	return true;
}

// Аналогично функции, которая выше.
bool isBotRightCorner(IplImage *img, int x, int y, int sensitivity, int amountWhitePixels)
{
	// Проверяем границы.
	if (x - lengthRectangle < 0 || x - widthRectangle < 0 
		|| y - widthRectangle < 0 || y - lengthRectangle < 0)
		return false;

	int count = 0; // Для подсчёта не чёрных пикселей.
	// Для этого проверим наличие двух пересекающихся прямоугольников. 
	// Проверяем наличие горизонтального.
	for (int y2 = y; y2 > y - widthRectangle; y2--) 
	{
		uchar* ptr = (uchar*) (img->imageData + y2 * img->widthStep);
		for (int x2 = x; x2 > x - lengthRectangle; x2--)
		{
			if (!isGreyOrBlack(ptr[x2 * 3], ptr[x2 * 3 + 1], ptr[x2 * 3 + 2], sensitivity))
			{
				count++;
				if (count > amountWhitePixels)
					return false;
			}
		}
	}

	count = 0;
	// Проверяем наличие вертикального.
	for (int y2 = y; y2 > y - lengthRectangle; y2--) 
	{
		uchar* ptr = (uchar*) (img->imageData + y2 * img->widthStep);
		for (int x2 = x; x2 > x - widthRectangle; x2--)
		{
			if (!isGreyOrBlack(ptr[x2 * 3], ptr[x2 * 3 + 1], ptr[x2 * 3 + 2], sensitivity))
			{
				count++;
				if (count > amountWhitePixels)
					return false;
			}
		}
	}

	return true;
}

// Проверяем является ли точка основанием ячейки.
bool isCell(CellCoordinate *cellCoordinate, int sizeCellCoordinate, IplImage *img, int xCoordinate, int yCoordinate, int &width, int &height, int sensitivity, int amountWhitePixels)
{
	// Проверим является ли точка основанием левого-верхнего угла.
	if (!isTopLeftCorner(img, xCoordinate, yCoordinate, sensitivity, amountWhitePixels))
		return false;

	// Создаём три датчика (2 внутренних и один внешний). Датчик срабатывает, когда натыкается на чёрный пиксель.
	int sensorTop = yCoordinate - 12;
	int sensorBot = yCoordinate + 12;
	int sensorBotSecond = yCoordinate + 25;
	int sensorBotThird = yCoordinate + 45;

	width = 0;
	// Проверяем наличие правого-верхнего угла и запоминаем ширину ячейки.
	bool hasCorner = false;
	for (int x = xCoordinate + 50; x < img->width && x < xCoordinate + 1500; x++)
	{
		if (hasCorner)
			break;

		// Если сработал хотя бы один датчик.
		if (isGreyOrBlackForSensor(img, x, sensorTop + x % 3, sensitivity) || isGreyOrBlackForSensor(img, x, sensorBot + x % 3, sensitivity) 
			|| isGreyOrBlackForSensor(img, x, sensorBotSecond + x % 3, sensitivity) || isGreyOrBlackForSensor(img, x, sensorBotThird + x % 3, sensitivity))
		{
			for (int y = yCoordinate - 2; y <= yCoordinate + 5; y++)
			{
				if (isTopRightCorner(img, x, y, sensitivity, amountWhitePixels))
				{
					hasCorner = true;
					width = x - xCoordinate;
					break;
				}
			}
		}
	}

	// Если правого-верхнего угла нет - проверяем наличие ячейки выше.
	if (!hasCorner)
	{
		//return false;
		return searchTopCell(cellCoordinate, sizeCellCoordinate, xCoordinate, width, height);
	}

	// Аналогичные датчики.
	int sensorRight = xCoordinate + 12;
	int sensorRightSecond = xCoordinate + 25;
	int sensorRightThird = xCoordinate + 45;
	int sensorLeft = xCoordinate - 12;

	hasCorner = false;
	height = 0;
	// Проверяем наличие левого-нижнего угла и запоминаем высоту ячейки.
	for (int y = yCoordinate + 50; y < img->height && y < yCoordinate + 500; y++)
	{
		if (hasCorner)
			break;
		
		// Если сработал хотя бы один счётчик.
		if (isGreyOrBlackForSensor(img, sensorRight + y % 3, y, sensitivity) || isGreyOrBlackForSensor(img, sensorLeft + y % 3, y, sensitivity) 
			|| isGreyOrBlackForSensor(img, sensorRightSecond + y % 3, y, sensitivity) || isGreyOrBlackForSensor(img, sensorRightThird + y % 3, y, sensitivity))
		{
			for (int x = xCoordinate - 4; x <= xCoordinate + 4; x++)
			{
				if (isBotLeftCorner(img, x, y, sensitivity, amountWhitePixels))
				{
					hasCorner = true;
					height = y - yCoordinate;
					break;
				}
			}
		}
	}

	// Если левого-нижнего угла нет - проверяем наличие ячейки выше.
	if (!hasCorner)
		return searchTopCell(cellCoordinate, sizeCellCoordinate, xCoordinate, width, height);

	// Проверяем наличие правого-нижнего угла.
	for (int y = yCoordinate + height - 3; y < yCoordinate + height + 3; y++)
	{
		for (int x = xCoordinate + width - 3; x <= xCoordinate + width + 3; x++)
		{
			if (isBotRightCorner(img, x, y, sensitivity, amountWhitePixels))
			{
				return true;
			}
		}
	}

	return searchTopCell(cellCoordinate, sizeCellCoordinate, xCoordinate, width, height);
}

// Проверяем нет ли уже в структуре данной ячейки.
bool checkRepetition(CellCoordinate *cellCoordinate, int x, int y, int width, int height, int sizeCellCoordinate)
{
	for (int i = 0; i < sizeCellCoordinate; i++)
		if (abs(cellCoordinate[i].xCoordinate - x) < 20 && abs(cellCoordinate[i].yCoordinate - y) < 20)
		{
			if (cellCoordinate[i].width > width + 10)
				cellCoordinate[i].width = width;
			if (cellCoordinate[i].height > height + 10)
				cellCoordinate[i].height = height;
			return false;
		}

	return true;
}

// Переводим число в строку.
string intToString(int number)
{
	string result;

	if (number < 10)
	{
		result = "000";
		result += char(number + 48);
	}
	else if (number < 100)
	{
		result = "00";
		result += char(number / 10 + 48);
		result += char((number % 10) + 48);
	}
	else if (number < 1000)
	{
		result = "0";
		result += char((number / 100) + 48);
		result += char(((number / 10) % 10) + 48);
		result += char((number % 10) + 48);
	}
	else
	{
		result += char((number / 1000) + 48);
		result += char(((number / 100) % 10) + 48);
		result += char(((number / 10) % 10) + 48);
		result += char((number % 10) + 48);
	}

	return result;
}

// Создаём имя изображения. (координата х, координата у, ширина, высота и формат)
string createImageName(CellCoordinate *cellCoordinate, int i)
{
	string imgName;
	imgName = intToString(cellCoordinate[i].xCoordinate) + " ";
	imgName += intToString(cellCoordinate[i].yCoordinate) + " ";
	imgName += intToString(cellCoordinate[i].width) + " ";
	imgName += intToString(cellCoordinate[i].height) + ".bmp";

	return imgName;
}

void addCellInArray(CellCoordinate *cellCoordinate, int x, int y, int width, int height, int &sizeCellCoordinate)
{
	cellCoordinate[sizeCellCoordinate].xCoordinate = x;
	cellCoordinate[sizeCellCoordinate].yCoordinate = y;
	cellCoordinate[sizeCellCoordinate].width = width;
	cellCoordinate[sizeCellCoordinate].height = height;
	sizeCellCoordinate++;
}

bool searchTopCell(CellCoordinate *cellCoordinate, int sizeCellCoordinate, int xCoordinate, int &width, int &height)
{
	bool isSearch = false;
	int tempY = 0;
	for (int i = 0; i < sizeCellCoordinate; i++)
	{
		if (abs(xCoordinate - cellCoordinate[i].xCoordinate) < 15 && cellCoordinate[i].yCoordinate > tempY)
		{
			tempY = cellCoordinate[i].yCoordinate;
			width = cellCoordinate[i].width;
			height = cellCoordinate[i].height;
			isSearch = true;
		}
	}

	return isSearch;
}