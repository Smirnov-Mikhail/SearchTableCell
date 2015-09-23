#pragma once
#include <string>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;

// ��������� ��� �������� ���� ����������� ������ � �������.
struct CellCoordinate
{
	int xCoordinate;
	int yCoordinate;
	int width;
	int height;
};

// ���������� �������� �� ������� ����� ��� ������.
inline bool isGreyOrBlack(int a, int b, int c, int sensitivity);

// ���������� �������� �� ������� ����� ��� ������ ��� �������.
inline bool isGreyOrBlackForSensor(IplImage *img, int x, int y, int sensitivity);

// ���������� �������� �� ������� �����.
inline bool isWhite(int a, int b, int c);

// ���������� �������� �� ����� ���������� ������-�������� ����.
bool isTopLeftCorner(IplImage *img, int x, int y, int sensitivity, int amountWhitePixels);

// ���������� �������� �� ����� ���������� �������-�������� ����.
bool isTopRightCorner(IplImage *img, int x, int y, int sensitivity, int amountWhitePixels);

// ���������� ��������, ������� ����.
bool isBotLeftCorner(IplImage *img, int x, int y, int sensitivity, int amountWhitePixels);

// ���������� �������, ������� ����.
bool isBotRightCorner(IplImage *img, int x, int y, int sensitivity, int amountWhitePixels);

// ��������� �������� �� ����� ���������� ������.
bool isCell(CellCoordinate *cellCoordinate, int sizeCellCoordinate, IplImage *img, int xCoordinate, int yCoordinate, int &width, int &height, int sensitivity, int amountWhitePixels);

// ��������� ��� �� ��� � ��������� ������ ������.
bool checkRepetition(CellCoordinate *cellCoordinate, int x, int y, int width, int height, int sizeCellCoordinate);

// ��������� ����� � ������.
string intToString(int number);

// ������ ��� �����������. (���������� �, �, ������, ������ � ������)
string createImageName(CellCoordinate *cellCoordinate, int i);

//��������� ������� � ������ �����.
void addCellInArray(CellCoordinate *cellCoordinate, int x, int y, int width, int height, int &size);

// ���� ����� ������������ ������ ��� ������������� �����.
bool searchTopCell(CellCoordinate *cellCoordinate, int sizeCellCoordinateint, int xCoordinate, int &width, int &height);