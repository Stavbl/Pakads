#include "TextBox.h"
#include <iostream>

using namespace std;

TextBox::TextBox(int size, int x, int y) : len(0), size(size) {
	buffer = (char *)malloc(size);
	position.X = x;
	position.Y = y;
	TabPosition tp;
	tp.pos.X = 1;
	tp.pos.Y = 1;
	tp.element = this;
	tabPosArr.push_back(tp);
	int i = 0;
	for (i = 0; i<size; ++i) {
		buffer[i] = ' ';
	}
	view = new View(x, y, size + 2, 3);
}
void TextBox::print(HANDLE h, COORD cursor, COORD window) {
	COORD p = pos();
	p.X += window.X;
	p.Y += window.Y;
	SetConsoleCursorPosition(h, p);
	for (int i = 0; i<size; ++i) {
		cout << buffer[i];
	}
}


bool TextBox::handle_clicks(PCOORD mouse, COORD window, PCOORD cursor) {
	if (intersects(mouse, window)) {
		cursor->X = mouse->X;
		cursor->Y = mouse->Y;
		if (cursor->X - position.X - window.X > len) {
			cursor->X = len + position.X + window.X;
		}
		if (len == 0) {
			cursor->X = position.X + window.X;
		}
		return true;
	}
	return false;
}

bool TextBox::handle_keys(PCOORD cor, COORD window, char c, int keycode) {
	if (c == '\n' || c == '\r') {
		return false;
	}
	if (len <= size) {
		if (intersects(cor, window)) {

			if (keycode == 37) {
				if (cor->X > 0) cor->X--;

				if (cor->X < position.X + window.X) { cor->X = position.X + window.X; }
				cor->X = (cor->X - position.X - window.X) >= (len+1) ? position.X + window.X + len: cor->X;
				return true;
			}
			else if (keycode == 39) {
				cor->X = cor->X - position.X - window.X >= len ? position.X + window.X + len : cor->X+1;
				if (len == 0) {
					cor->X = position.X + window.X;
				}
				return true;
			}
			else if (keycode == 38) {
				return false;
			}
			else if (keycode == 40) {
				return false;
			}
			if (keycode == 46) {
				if (cor->X - position.X - window.X >= len) {
					return false;
				}
				else {
					int index = cor->X - position.X - window.X;
					if (index < 0) {
						index = 0;
					}
					delete_char(index);
					len--;
					if (len < 0) { len = 0; }
					if (cor->X < position.X + window.X) { cor->X = position.X + window.X; }
					return true;
				}
			}
			if (c == 0) { return false; }
			if (cor->X - position.X - window.X > len) {
				cor->X = (len == 0) ? position.X + window.X : len + position.X;
			}
			if (keycode == 8) {
				if (cor->X - position.X - window.X == len) {
					buffer[len-1] = ' ';
					len--;
					cor->X--;
					if (len < 0) { len = 0; }
					if (cor->X < position.X + window.X) { cor->X = position.X + window.X; }
				}
				else if (cor->X - position.X - window.X < len) {
					int index = cor->X - position.X - window.X -1;
					if (index < 0) {
						index = 0;
					}
					delete_char(index);
					len--;
					cor->X--;
					if (len < 0) { len = 0; }
					if (cor->X < position.X + window.X) { cor->X = position.X + window.X; }
				}
			} else if (len < size) {
				if (len == 0) {
					buffer[len] = c;
					len++;
					cor->X++;
				} else if (cor->X - position.X - window.X == len) {
					buffer[len] = c;
					len++;
					if (len < size) {
						cor->X++;
					}
				}
				else {
					shift(cor->X - position.X - window.X, c);
					len++;
					cor->X++;
				}
			}
			if (cor->X - position.X - window.X > len) {
				cor->X = len + position.X + window.X + 1;
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
void TextBox::SetValue(string txt)
{
	for (int i = 0; i < txt.size(); i++) {
		buffer[i] = txt[i];
	}
	len = txt.size();
	view_invalidated = true;
}

void TextBox::updateView(COORD cursor)
{
	int layer = (active) ? 2 : 1;
	view->clearAll(foreground, background);
	updateBorder(layer);
	for (int i = 0; i<size; ++i) {
		if (cursor.X == i+1 && cursor.Y == 1) {
			view->update(i + 1, 1, buffer[i], background, foreground, layer);
		}
		else {
			view->update(i + 1, 1, buffer[i], foreground, background, layer);
		}
	}
}

string TextBox::GetValue()
{
	return buffer;
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
	delete view;
}