#pragma once
#include <string>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;

// Структура для хранения всех необходимых данных о ячейках.
struct CellCoordinate
{
	int xCoordinate;
	int yCoordinate;
	int width;
	int height;
};

// Определяем является ли пиксель серым или чёрным.
inline bool isGreyOrBlack(int a, int b, int c, int sensitivity);

// Определяем является ли пиксель серым или чёрным для датчика.
inline bool isGreyOrBlackForSensor(IplImage *img, int x, int y, int sensitivity);

// Определяем является ли пиксель белым.
inline bool isWhite(int a, int b, int c);

// Определяем является ли точка основанием левого-верхнего угла.
bool isTopLeftCorner(IplImage *img, int x, int y, int sensitivity, int amountWhitePixels);

// Определяем является ли точка основанием правого-верхнего угла.
bool isTopRightCorner(IplImage *img, int x, int y, int sensitivity, int amountWhitePixels);

// Аналогично функциии, которая выше.
bool isBotLeftCorner(IplImage *img, int x, int y, int sensitivity, int amountWhitePixels);

// Аналогично функции, которая выше.
bool isBotRightCorner(IplImage *img, int x, int y, int sensitivity, int amountWhitePixels);

// Проверяем является ли точка основанием ячейки.
bool isCell(CellCoordinate *cellCoordinate, int sizeCellCoordinate, IplImage *img, int xCoordinate, int yCoordinate, int &width, int &height, int sensitivity, int amountWhitePixels);

// Проверяем нет ли уже в структуре данной ячейки.
bool checkRepetition(CellCoordinate *cellCoordinate, int x, int y, int width, int height, int sizeCellCoordinate);

// Переводим число в строку.
string intToString(int number);

// Создаём имя изображения. (координаты х, у, ширина, высота и формат)
string createImageName(CellCoordinate *cellCoordinate, int i);

//Добавляем элемент в массив ячеек.
void addCellInArray(CellCoordinate *cellCoordinate, int x, int y, int width, int height, int &size);

// Ищем ранее обнаруженную ячейку над потенциальной нашей.
bool searchTopCell(CellCoordinate *cellCoordinate, int sizeCellCoordinateint, int xCoordinate, int &width, int &height);