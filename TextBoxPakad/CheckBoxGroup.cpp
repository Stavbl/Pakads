#include "CheckBoxGroup.h"
#include <iostream>

using namespace std;

CheckBoxGroup::CheckBoxGroup(int _height, int width, vector<string> entries, int x, int y) {
	position.X = x;
	position.Y = y;
	int i = 0;
	sizeh = _height;
	size = width;
	for (int i = 0; i < entries.size(); i++) {
		addOption(entries[i]);
	}
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
	if (buffer.size() > sizeh) {
		sizeh = buffer.size();
	}
}

void CheckBoxGroup::print(HANDLE h, COORD cursor, COORD window) {
	COORD tmp;
	tmp.X = this->position.X + window.X;
	for (int i = 0; i<buffer.size(); ++i) {
		tmp.Y = this->position.Y + i + window.Y;
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
	for (int j = 0; j < sizeh - buffer.size(); j++) {
		tmp.Y = this->position.Y + j + buffer.size() + window.Y;
		SetConsoleCursorPosition(h, tmp);
		for (int i = 0; i < size; i++) {
			cout << " ";
		}
	}
}
bool CheckBoxGroup::handle_keys(PCOORD cor, COORD window, char c, int keycode) {
	if (intersects(cor, window)) {
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

		if (keycode == 0x0D) { c == ' '; }
		if (c == 0) { return false; }


		if (c == ' ') {
			if (selected[cor->Y - position.Y - window.Y]) {
				selected[cor->Y - position.Y - window.Y] = false;
			}
			else {
				selected[cor->Y - position.Y - window.Y] = true;
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
	return sizeh;
}


bool CheckBoxGroup::handle_clicks(PCOORD mouse, COORD window, PCOORD cursor) {
	if (intersects(mouse, window)) {
		if (selected[mouse->Y - position.Y - window.Y]) {
			selected[mouse->Y - position.Y - window.Y] = false;
		}
		else {
			selected[mouse->Y - position.Y - window.Y] = true;
		}
		cursor->X = mouse->X;
		cursor->Y = mouse->Y;
		return true;
	}
	return false;
}

void CheckBoxGroup::selectIndex(size_t i)
{
	if (i < 0 || i >= buffer.size()) {
		return;
	}
	selected[i] = true;
	view_invalidated = true;
}

void CheckBoxGroup::deselectIndex(size_t i)
{
	if (i < 0 || i >= buffer.size()) {
		return;
	}
	selected[i] = false;
	view_invalidated = true;
}

vector<size_t> CheckBoxGroup::getSelectedIndicies()
{
	vector<size_t> indexes;
	for (int i = 0; i < selected.size(); i++) {
		if (selected[i]) {
			indexes.push_back(i);
		}
	}
	return indexes;
}

CheckBoxGroup::~CheckBoxGroup() {
}