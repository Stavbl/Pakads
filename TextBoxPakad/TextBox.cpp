#include "TextBox.h"
#include <iostream>

using namespace std;

TextBox::TextBox(int x, int y, int size) : len(0), size(size) {
	buffer = (char *)malloc(size);
	position.X = x;
	position.Y = y;
	tabPosArr.push_back(position);
	int i = 0;
	for (i = 0; i<size; ++i) {
		buffer[i] = ' ';
	}
}
void TextBox::print(HANDLE h) {
	SetConsoleCursorPosition(h, this->position);
	CONSOLE_SCREEN_BUFFER_INFO bi;
	GetConsoleScreenBufferInfo(h, &bi);
	DWORD old = bi.wAttributes;
	DWORD style = BACKGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(h, style);
	for (int i = 0; i<size; ++i) {
		cout << buffer[i];
	}
	SetConsoleTextAttribute(h, old);

}


bool TextBox::handle_clicks(PCOORD mouse, PCOORD cursor) {
	if (intersects(mouse)) {
		cursor->X = mouse->X;
		cursor->Y = mouse->Y;
		if (cursor->X - position.X > len) {
			cursor->X = len + position.X;
		}
		if (len == 0) {
			cursor->X = position.X;
		}
		return true;
	}
	return false;
}

bool TextBox::handle_keys(PCOORD cor, char c, int keycode) {
	if (len <= size) {
		if (intersects(cor)) {

			if (keycode == 37) {
				if (cor->X > 0) cor->X--;
				cor->X = (cor->X - position.X) >= (len+1) ? position.X + len: cor->X;
				return true;
			}
			else if (keycode == 39) {
				cor->X = cor->X - position.X >= len ? position.X + len : cor->X+1;
				if (len == 0) {
					cor->X = position.X;
				}
				return true;
			}
			else if (keycode == 38) {
				return false;
			}
			else if (keycode == 40) {
				return false;
			}

			if (c == 0) { return false; }
			if (cor->X - position.X > len) {
				cor->X = (len == 0) ? position.X : len + position.X;
			}
			if (keycode == 8) {
				if (cor->X - position.X == len) {
					buffer[len-1] = ' ';
					len--;
					cor->X--;
					if (len < 0) { len = 0; }
					if (cor->X < position.X) { cor->X = position.X; }
				}
				else if (cor->X - position.X  < len) {
					delete_char(cor->X - position.X);
					len--;
					cor->X--;
					if (len < 0) { len = 0; }
					if (cor->X < position.X) { cor->X = position.X; }
				}
			} else if (len < size) {
				if (len == 0) {
					buffer[len] = c;
					len++;
					cor->X++;
				} else if (cor->X - position.X == len) {
					buffer[len] = c;
					len++;
					if (len < size) {
						cor->X++;
					}
				}
				else {
					shift(cor->X - position.X, c);
					len++;
					cor->X++;
				}
			}
			if (cor->X - position.X > len) {
				cor->X = len + position.X + 1;
			}
			return true;
		}
	}
	return false;
}
void TextBox::delete_char(int j) {
	for (int i = j; i< len-1; ++i) {
		buffer[i] = buffer[i + 1];
	}
	if (len > 0) {
		buffer[len - 1] = ' ';
	}
	else {
		buffer[len] = ' ';
	}
}
void TextBox::shift(int j, char c) {
	for (int i = len; i> j; --i) {
		buffer[i] = buffer[i - 1];
	}
	buffer[j] = c;
}

int TextBox::width() {
	return size;
}

int TextBox::height() {
	return 1;
}

COORD TextBox::pos() {
	return position;
}

TextBox::~TextBox() {
	free(buffer);
}