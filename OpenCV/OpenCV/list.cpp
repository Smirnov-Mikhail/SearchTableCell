#include "list.h"
#include <iostream>
#include "methodsForImage.h"

using namespace std;

List::~List()
{
	while (first != nullptr)
	{
		ListElement *temp = first->next;

		delete first;

		first = temp;
	}
}

void List::addValue(int width, int height)
{
	ListElement *temp = new ListElement;
	temp->width = width;
	temp->height = height;
	temp->count = 1;

	temp->next = first;
	first = temp;

	size++;
}

void List::addElement(CellCoordinate *cellCoordinate, IplImage *img, int x, int y, int &width, int &height, int &sizeCellCoordinate)
{
	if (first == nullptr)
	{
		int tempWidth = width;
		int tempHeight = height;
		if (isCell(cellCoordinate, sizeCellCoordinate, img, x, y, width, height, 200, 15))
		{
			if (abs(width - tempWidth) > 22 || abs(height - tempHeight) > 22)
			{
				addValue(width, height);
				addCellInArray(cellCoordinate, x, y, width, height, sizeCellCoordinate);

				return;
			}
			else
			{
				addValue(width, height);
				addCellInArray(cellCoordinate, x, y, width, height, sizeCellCoordinate);

				return;
			}
			
		}
		else
		{
			width = tempWidth;
			height = tempHeight;

			addValue(width, height);
			addCellInArray(cellCoordinate, x, y, width, height, sizeCellCoordinate);

			return;
		}

		return;
	}

	ListElement *current = first;

	while (((abs(width - current->width) > 22) || (abs(height - current->height) > 22)) && (current->next != nullptr))
		current = current->next;

	if (abs(width - current->width) <= 22 && abs(height - current->height) <= 22)
	{
		if (current->count < 7)
		{
			int tempWidth = width;
			int tempHeight = height;
			if (isCell(cellCoordinate, sizeCellCoordinate, img, x, y, width, height, 200, 15))
			{
				if (abs(width - tempWidth) > 22 || abs(height - tempHeight) > 22)
				{
					addElement(cellCoordinate, img, x, y, width, height, sizeCellCoordinate);

					return;
				}
				else
				{
					current->count++;
					addCellInArray(cellCoordinate, x, y, width, height, sizeCellCoordinate);

					return;
				}
			}
			else if (abs(width - tempWidth) > 22 || abs(height - tempHeight) > 22)
			{
				width = tempWidth;
				height = tempHeight;
			}
		}

		current->count++;
		addCellInArray(cellCoordinate, x, y, width, height, sizeCellCoordinate);

		return;
	}
	else if (current->next == nullptr)
	{
		int tempWidth = width;
		int tempHeight = height;
		if (isCell(cellCoordinate, sizeCellCoordinate, img, x, y, width, height, 200, 15))
		{
			if (abs(width - tempWidth) > 22 || abs(height - tempHeight) > 22)
			{
				addElement(cellCoordinate, img, x, y, width, height, sizeCellCoordinate);

				return;
			}
			else
			{
				addValue(width, height);

				addCellInArray(cellCoordinate, x, y, width, height, sizeCellCoordinate);

				return;
			}
		}
		else if (abs(width - tempWidth) > 22 || abs(height - tempHeight) > 22)
		{
			width = tempWidth;
			height = tempHeight;
		}

		addValue(width, height);

		addCellInArray(cellCoordinate, x, y, width, height, sizeCellCoordinate);
	}
}

void List::printList()
{
	ListElement *temp = first;

	while (temp != nullptr)
	{
		cout << temp->width << ' ' << temp->height << ' ' << temp->count << endl;

		temp = temp->next;
	}
}