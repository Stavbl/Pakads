#include "Label.h"
#include <iostream>

using namespace std;

Label::Label(int width, string txt, int x, int y) {
	position.X = x;
	position.Y = y;
	size = width;

	buffer = (char *)malloc(txt.size() + 1);
	strcpy(buffer, txt.c_str());
	if (strlen(buffer) > size) {
		size = strlen(buffer);
	}
	view = new View(x, y, this->width() + 2, this->height() + 2);
}
void Label::print(HANDLE h, COORD cursor, COORD window) {
	COORD p = this->pos();
	p.X += window.X;
	p.Y += window.Y;
	SetConsoleCursorPosition(h, p);
	for (int i = 0; i < width(); i++) {
		cout << " ";
	}
	SetConsoleCursorPosition(h, p);
	if (buffer != NULL) {
		int w = strlen(buffer);
		for (int i = 0; i < (width()-w) / 2; i++) {
			cout << " ";
		}
		cout << buffer;
	}
}

void Label::SetValue(string txt) {
	buffer = (char *)malloc(txt.size() + 1);
	strcpy(buffer, txt.c_str()); 
	if (txt.size() > size) {
		size = txt.size();
	}
}

bool Label::handle_clicks(PCOORD mouse, COORD window, PCOORD cursor) {
	return false;
}

bool Label::handle_keys(PCOORD cor, COORD window, char c, int keycode) {
	return false;
}

int Label::width() {
	return size;
}

int Label::height() {
	return 1;
}

void Label::updateView(COORD cursor)
{
	int layer = (active) ? 2 : 1;
	/*if (cursor.X > 0 && cursor.X < width() + 2 && cursor.Y > 0 && cursor.Y < height() + 2) {
	layer = 2;

	}*/
	view->clearAll(foreground, background);
	view->solidifyBackground();
	updateBorder(layer);

	if (buffer != NULL) {
		int w = strlen(buffer);
		for (int i = 0; i < w; i++) {
			view->update(1+((width() - w) / 2)+i, 1, buffer[i], foreground, background, layer);
		}
	}

}

COORD Label::pos() {
	return position;
}

Label::~Label() {
	free(buffer);
	delete view;
}