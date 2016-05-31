#include "NumericBox.h"
#include <iostream>

using namespace std;


NumericBox::NumericBox(int height, int width, int min, int max, int x, int y)
{
	size = width + 2;
	minVal = min;
	maxVal = max;
	position.X = x;
	position.Y = y;
}

int NumericBox::width()
{
	return size;
}

int NumericBox::height()
{
	return 1;
}

COORD NumericBox::pos()
{
	return position;
}

void NumericBox::print(HANDLE h, COORD cursor, COORD window)
{
	COORD p = pos();
	p.X += window.X;
	p.Y += window.Y;
	SetConsoleCursorPosition(h, p);
	cout << "-";
	char *buffer = new char[size - 2];
	itoa(val, buffer, 10);
	int sz = size - 2;
	int len = strlen(buffer);
	sz -= len;
	for (int i = (sz - len) / 2; i >= 0; i--) {
		cout << " ";
		sz--;
	}
	cout << buffer;
	for (; sz >= 0; sz--) {
		cout << " ";
	}
	cout << "+";
}

bool NumericBox::handle_keys(PCOORD x, COORD window, char c, int keycode)
{
	return false;
}

bool NumericBox::handle_clicks(PCOORD mouse, COORD window, PCOORD cursor)
{

	if (mouse->X - position.X - window.X == 0) {
		if (val > minVal) {
			val--;
		}
		return true;
	}
	if (mouse->X - position.X - window.X == size) {
		if (val < maxVal) {
			val++;
		}
		return true;
	}
	return false;
}

void NumericBox::SetValue(int val)
{
	this->val = val;
}

int NumericBox::GetValue()
{
	return val;
}
