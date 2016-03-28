#include "ComboBox.h"
#include <iostream>

using namespace std;

ComboBox::ComboBox(int x, int y) {
	position.X = x;
	position.Y = y;
	int i = 0;
	size = 0;
	closedTabPos.push_back(position);
}

void ComboBox::addOption(string str) {
	if (str.size()+2 > size) {
		size = str.size()+2;
	}
	selected = 0;
	buffer.push_back(str);
	COORD p = position;
	p.Y += tabPosArr.size()+2;
	tabPosArr.push_back(p);
}

void ComboBox::print(HANDLE h) {
	COORD tmp;
	tmp.X = this->position.X;
	CONSOLE_SCREEN_BUFFER_INFO bi;
	GetConsoleScreenBufferInfo(h, &bi);
	DWORD old = bi.wAttributes;
	DWORD style = BACKGROUND_RED | BACKGROUND_BLUE  | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(h, style);
	if (buffer.size() <= 0) { return; }
	if (opened) {
		tmp.Y = this->position.Y;
		SetConsoleCursorPosition(h, tmp);
		cout << buffer[selected];
		for (int j = 0; j < size - buffer[selected].size()-1; j++) {
			cout << " ";
		}
		cout << "-";
		tmp.Y = this->position.Y+1;
		SetConsoleCursorPosition(h, tmp);
		for (int j = 0; j < size; j++) {
			cout << "-";
		}

		for (int i = 0; i<buffer.size(); ++i) {
			if (i == selected) {
				SetConsoleTextAttribute(h, style | BACKGROUND_INTENSITY);
			}
			else {
				SetConsoleTextAttribute(h, style);
			}
			tmp.Y = this->position.Y + i + 2;
			SetConsoleCursorPosition(h, tmp);
			cout << buffer[i];                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
			for (int j = 0; j < size - buffer[i].size(); j++) {
				cout << " ";
			}
		}
	}
	else {
		SetConsoleCursorPosition(h, pos());
		if (selected != -1) {
			cout << buffer[selected]; 
			for (int j = 0; j < size - buffer[selected].size() - 1; j++) {
				cout << " ";
			}
		}
		else {
			for (int j = 0; j < size -1; j++) {
				cout << " ";
			}
		}	
		cout << "+";
		SetConsoleTextAttribute(h, 0);
		tmp.Y = this->position.Y + 1;
		SetConsoleCursorPosition(h, tmp);
		for (int j = 0; j < size; j++) {
			cout << " ";
		}

		for (int i = 0; i<buffer.size(); ++i) {
			tmp.Y = this->position.Y + i + 2;
			SetConsoleCursorPosition(h, tmp);
			for (int j = 0; j < size; j++) {
				cout << " ";
			}
		}
	}

	SetConsoleTextAttribute(h, old);

}
bool ComboBox::handle_keys(PCOORD cor, char c, int keycode) {
	if (intersects(cor)) {
		if (keycode >= 37 && keycode <= 40) {
			return false;
		}

		if (c == 0) { return false; }


		if (c == ' ') {
			if (opened) {
				if (cor->Y < pos().Y + 2) {
					
				} else {
					selected = cor->Y - 2 - pos().Y;
				}
				opened = false;
				cor->Y = pos().Y;
				cor->X = pos().X + width() - 1;
			} else {
				opened = true;
			}
		}

		return true;
	}
	return false;
}

int ComboBox::width() {
	return size;
}

int ComboBox::height() {
	return (opened) ? buffer.size()+2 : 1;
}

vector<COORD> &ComboBox::tabPositions() {
	if (opened) {
		return tabPosArr;
	}
	else {
		return closedTabPos;
	}
}



bool ComboBox::handle_clicks(PCOORD mouse, PCOORD cursor) {
	if (intersects(mouse)) {
		if (opened) {
			if (mouse->Y < pos().Y + 2) {

			}
			else {
				selected = mouse->Y - 2 - pos().Y;
			}
			opened = false;
			cursor->Y = pos().Y;
			cursor->X = pos().X + width() - 1;
		}
		else {
			opened = true;
			cursor->X = mouse->X;
			cursor->Y = mouse->Y;
		}
		return true;
	}
	return false;
}

ComboBox::~ComboBox() {
}