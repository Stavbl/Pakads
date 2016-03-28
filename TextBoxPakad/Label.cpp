#include "Label.h"
#include <iostream>

using namespace std;

Label::Label(int x, int y, char* txt) {
	buffer = (char *)malloc(strlen(txt)+1);
	strcpy(buffer, txt);
	position.X = x;
	position.Y = y;
	size = strlen(buffer);
}
void Label::print(HANDLE h) {
	SetConsoleCursorPosition(h, this->position);
	CONSOLE_SCREEN_BUFFER_INFO bi;
	GetConsoleScreenBufferInfo(h, &bi);
	DWORD old = bi.wAttributes;
	DWORD style =  FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(h, style);
	cout << buffer;
	SetConsoleTextAttribute(h, old);
}


bool Label::handle_clicks(PCOORD mouse, PCOORD cursor) {
	return false;
}

bool Label::handle_keys(PCOORD cor, char c, int keycode) {
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
}