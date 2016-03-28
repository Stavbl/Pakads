#include "RadioGroup.h"
#include <iostream>

using namespace std;

RadioGroup::RadioGroup(int x, int y) : selected(-1) {
	position.X = x;
	position.Y = y;
	int i = 0;
	size = 0;
}

void RadioGroup::addOption(string str) {
	if (str.size() > size) {
		size = str.size();
	}
	buffer.push_back(str);
	COORD p = position;
	p.X += 1;
	p.Y += tabPosArr.size();
	tabPosArr.push_back(p);
}

void RadioGroup::print(HANDLE h) {
	COORD tmp;
	tmp.X = this->position.X;
	CONSOLE_SCREEN_BUFFER_INFO bi;
	GetConsoleScreenBufferInfo(h, &bi);
	DWORD old = bi.wAttributes;
	DWORD style = BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY;
	SetConsoleTextAttribute(h, style);
	for (int i = 0; i<buffer.size(); ++i) {
		tmp.Y = this->position.Y + i;
		SetConsoleCursorPosition(h, tmp);
		if (selected == i) {
			cout << "(*) ";
		} else {
			cout << "( ) ";
		}
		cout << buffer[i];
		for (int j = 0; j < size - buffer[i].size(); j++) {
			cout << " ";
		}
	}

	SetConsoleTextAttribute(h, old);

}
bool RadioGroup::handle_keys(PCOORD cor, char c, int keycode) {
	if (intersects(cor)) {
			if (keycode == 37) {
				return false;
			}
			else if (keycode == 39) {
				return false;
			}
			else if (keycode == 38) {
				return false;
			}
			else if (keycode == 40) {
				return false;
			}

			if (c == 0) { return false; }

			
			if (c == ' ') {
				if (selected == cor->Y - position.Y) {
					selected = -1;
				} else {
					selected = cor->Y - position.Y;
				}
			}
			
			return true;
	}
	return false;
}

int RadioGroup::width() {
	return size + 4;
}

int RadioGroup::height() {
	return buffer.size();
}


bool RadioGroup::handle_clicks(PCOORD mouse, PCOORD cursor) {
	if (intersects(mouse)) {
		if (selected == mouse->Y - position.Y) {
			selected = -1;
		}
		else {
			selected = mouse->Y - position.Y;
		}
		cursor->X = mouse->X;
		cursor->Y = mouse->Y;
		return true;
	}
	return false;
}

RadioGroup::~RadioGroup() {
}