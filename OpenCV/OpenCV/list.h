#pragma once
#include "methodsForImage.h"

#include <iostream>

using namespace std;

class List
{
	public:
		List() : first(nullptr), size(0) {}
		~List();

		// Добавляем параментры ячейки в список.
		void addElement(CellCoordinate *cellCoordinate, IplImage *img, int x, int y, int &width, int &height, int &sizeCellCoordinate);

		void List::addValue(int width, int height);

		void printList();

	private:
		struct ListElement
		{
			int width;
			int height;
			int count;
			ListElement *next;	
		};

		int size;

		ListElement *first;
};