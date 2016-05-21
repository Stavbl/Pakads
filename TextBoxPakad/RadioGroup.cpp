#include "RadioGroup.h"
#include <iostream>

using namespace std;

RadioGroup::RadioGroup(int height, int width, vector<string> entries, int x, int y) : selected(-1) {
	position.X = x;
	position.Y = y;
	int i = 0;
	size = width;
	sizeh = height;
	for (int i = 0; i < entries.size(); i++) {
		addOption(entries[i]);
	}
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
	if (buffer.size() > sizeh) {
		sizeh = buffer.size();
	}	
}

void RadioGroup::print(HANDLE h, COORD cursor, COORD window) {
	COORD tmp;
	tmp.X = this->position.X + window.X;

	for (int i = 0; i<buffer.size(); ++i) {
		
		tmp.Y = this->position.Y + i + window.Y;
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

	for (int j = 0; j < sizeh - buffer.size(); j++) {
		tmp.Y = this->position.Y + j + buffer.size() + window.Y;
		SetConsoleCursorPosition(h, tmp);
		for (int i = 0; i < size; i++) {
			cout << " ";
		}
	}
}

bool RadioGroup::handle_keys(PCOORD cor, COORD window, char c, int keycode) {
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
				if (selected == cor->Y - position.Y - window.Y) {
					selected = -1;
				} else {
					selected = cor->Y - position.Y - window.Y;
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


bool RadioGroup::handle_clicks(PCOORD mouse, COORD window, PCOORD cursor) {
	if (intersects(mouse, window)) {
		if (selected == mouse->Y - position.Y - window.Y) {
			selected = -1;
		}
		else {
			selected = mouse->Y - position.Y - window.Y;
		}
		cursor->X = mouse->X;
		cursor->Y = mouse->Y;
		return true;
	}
	return false;
}

size_t RadioGroup::getSelectedIndex()
{
	return selected;
}

bool RadioGroup::setSelectedIndex(size_t index)
{
	if (index >= buffer.size() || index < 0) {
		return false;
	}
	selected = index;
	view_invalidated = true;
	return true;
}

bool RadioGroup::clearSelection()
{
	if (selected == -1) { return false; }
	selected = -1;
	return true;
}

RadioGroup::~RadioGroup() {
}