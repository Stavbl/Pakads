#include "Checklist.h"
#include <iostream>

using namespace std;

Checklist::Checklist(int _height, int width, vector<string> entries, int x, int y) {
	position.X = x;
	position.Y = y;
	int i = 0;
	sizeh = _height;
	size = width;
	for (int i = 0; i < entries.size(); i++) {
		addOption(entries[i]);
	}
}

void Checklist::addOption(string str) {
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

void Checklist::print(HANDLE h, COORD cursor, COORD window) {
	COORD tmp;
	tmp.X = this->position.X + window.X;
	DWORD style = color_to_rgb(foreground, background);
	for (int i = 0; i<buffer.size(); ++i) {
		tmp.Y = this->position.Y + i + window.Y;
		SetConsoleCursorPosition(h, tmp);
		if (i + position.Y + window.Y == cursor.Y && intersects(&cursor, window)) {
			SetConsoleTextAttribute(h, color_to_rgb(background, foreground));
		}
		else {
			SetConsoleTextAttribute(h, style);
		}
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
	SetConsoleTextAttribute(h, style);
	for (int j = 0; j < sizeh - buffer.size(); j++) {
		tmp.Y = this->position.Y + j + buffer.size() + window.Y;
		SetConsoleCursorPosition(h, tmp);
		for (int i = 0; i < size; i++) {
			cout << " ";
		}
	}
}
bool Checklist::handle_keys(PCOORD cor, COORD window, char c, int keycode) {
	if (intersects(cor, window)) {
		if (keycode >= 37 && keycode <= 40) {
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

int Checklist::width() {
	return size + 4;
}

int Checklist::height() {
	return sizeh;
}


bool Checklist::handle_clicks(PCOORD mouse, COORD window, PCOORD cursor) {
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

void Checklist::SelectIndex(size_t i)
{
	if (i < 0 || i >= buffer.size()) {
		return;
	}
	selected[i] = true;
	view_invalidated = true;
}

void Checklist::DeselectIndex(size_t i)
{
	if (i < 0 || i >= buffer.size()) {
		return;
	}
	selected[i] = false;
	view_invalidated = true;
}

vector<size_t> Checklist::GetSelectedIndicies()
{
	vector<size_t> indexes;
	for (int i = 0; i < selected.size(); i++) {
		if (selected[i]) {
			indexes.push_back(i);
		}
	}
	return indexes;
}

Checklist::~Checklist() {
}