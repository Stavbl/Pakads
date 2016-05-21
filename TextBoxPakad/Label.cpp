#include "Label.h"
#include <iostream>

using namespace std;

Label::Label(int width, int x, int y, char* txt) {
	position.X = x;
	position.Y = y;
	size = width;
	if (txt != NULL) {
		buffer = (char *)malloc(strlen(txt) + 1);
		strcpy(buffer, txt);
		if (strlen(buffer) > size) {
			size = strlen(buffer);
		}
	}
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
		cout << buffer;
	}
}

void Label::set_text(const char *txt) {
	buffer = (char *)malloc(strlen(txt) + 1);
	strcpy(buffer, txt); 
	if (strlen(buffer) > size) {
		size = strlen(buffer);
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

COORD Label::pos() {
	return position;
}

Label::~Label() {
	free(buffer);
	cout << "Something";
	int x;
	cin >> x;
}