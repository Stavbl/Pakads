#include "CheckBoxGroup.h"
#include <iostream>

using namespace std;

CheckBoxGroup::CheckBoxGroup(int x, int y) {
	position.X = x;
	position.Y = y;
	int i = 0;
	size = 0;
}

void CheckBoxGroup::addOption(string str) {
	if (str.size() > size) {
		size = str.size();
	}
	buffer.push_back(str);
	selected.push_back(false);	
	COORD p = position;
	p.X += 1;
	p.Y += tabPosArr.size();
	tabPosArr.push_back(p);
}

void CheckBoxGroup::print(HANDLE h) {
	COORD tmp;
	tmp.X = this->position.X;
	CONSOLE_SCREEN_BUFFER_INFO bi;
	GetConsoleScreenBufferInfo(h, &bi);
	DWORD old = bi.wAttributes;
	DWORD style =  BACKGROUND_BLUE | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY;
	SetConsoleTextAttribute(h, style);
	for (int i = 0; i<buffer.size(); ++i) {
		tmp.Y = this->position.Y + i;
		SetConsoleCursorPosition(h, tmp);
		if (selected[i]) {
			cout << "[x] ";
		}
		else {
			cout << "[ ] ";
		}
		cout << buffer[i];
		for (int j = 0; j < size - buffer[i].size(); j++) {
			cout << " ";
		}
	}

	SetConsoleTextAttribute(h, old);

}
bool CheckBoxGroup::handle_keys(PCOORD cor, char c, int keycode) {
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
			if (selected[cor->Y - position.Y]) {
				selected[cor->Y - position.Y] = false;
			}
			else {
				selected[cor->Y - position.Y] = true;
			}
		}

		return true;
	}
	return false;
}

int CheckBoxGroup::width() {
	return size + 4;
}

int CheckBoxGroup::height() {
	return buffer.size();
}


bool CheckBoxGroup::handle_clicks(PCOORD mouse, PCOORD cursor) {
	if (intersects(mouse)) {
		if (selected[mouse->Y - position.Y]) {
			selected[mouse->Y - position.Y] = false;
		}
		else {
			selected[mouse->Y - position.Y] = true;
		}
		cursor->X = mouse->X;
		cursor->Y = mouse->Y;
		return true;
	}
	return false;
}

CheckBoxGroup::~CheckBoxGroup() {
}