#include "NumericBox.h"
#include <iostream>

using namespace std;


NumericBox::NumericBox(int height, int width, int min, int max, int x, int y)
{
	size = width + 2;
	minVal = min;
	maxVal = max;
	val = min;
	position.X = x;
	position.Y = y;
	view = new View(x, y, size+2, height+2);
	TabPosition tp;
	tp.pos.X = 1;
	tp.pos.Y = 1;
	tp.element = this;

	tabPosArr.push_back(tp);
	tp.pos.X = size;

	tabPosArr.push_back(tp);
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
	if (this->intersects(x, window)) {
		if (x->X == pos().X + window.X) {
			if (keycode == 39 || keycode == 39) {
				x->X = pos().X + window.X + width() - 1;
			}
			else if (c == ' ') {
				if (val > minVal) {
					val--;
				}
			}
			return true;
		}
		else if (x->X == pos().X + window.X + width()-1) {
			if (keycode == 37 || keycode == 39) {
				x->X = pos().X + window.X;
			}
			else if (c == ' ') {
				if (val < maxVal) {
					val++;
				}
			}
			return true;
		}
	}
	return false;
}

bool NumericBox::handle_clicks(PCOORD mouse, COORD window, PCOORD cursor)
{

	if (mouse->X - position.X - window.X == 0) {
		*cursor = *mouse;
		if (val > minVal) {
			val--;
		}
		return true;
	}
	if (mouse->X - position.X - window.X == size-1) {
		*cursor = *mouse;
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

void NumericBox::updateView(COORD cursor)
{
	int layer = 1;
	if (cursor.X > 0 && cursor.X < width()+2 && cursor.Y > 0 && cursor.Y < height()+2) {
		layer = 2;
	}
	view->clearAll(foreground, background);
	view->solidifyBackground();
	updateBorder(layer);

	

	char *buffer = new char[size - 2];
	itoa(val, buffer, 10);
	int len = strlen(buffer);
	int sz = (size - 2 - len)/2;
	
	for (int i = 0; i < len; i++) {
		view->update(sz+2, 1, buffer[i], foreground, background, layer);
	}
	if (cursor.X == 1 && cursor.Y == 1) {
		view->update(1, 1, '-', background, foreground, layer);
	}
	else {
		view->update(1, 1, '-', foreground, background, layer);
	}
	if (cursor.X == width() && cursor.Y == 1) {
		view->update(size, 1, '+', background, foreground, layer);
	}
	else {
		view->update(size, 1, '+', foreground, background, layer);
	}

}

NumericBox::~NumericBox()
{
	delete view;
}
