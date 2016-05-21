#include "ComboBox.h"
#include <iostream>

using namespace std;

ComboBox::ComboBox(int width, vector<string> entries, int x, int y) {
	position.X = x;
	position.Y = y;
	int i = 0;
	size = width;
	closedTabPos.push_back(position);
	for (int i = 0; i < entries.size(); i++) {
		addOption(entries[i]);
	}
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

void ComboBox::print(HANDLE h, COORD cursor, COORD window) {
	CONSOLE_SCREEN_BUFFER_INFO bi;
	GetConsoleScreenBufferInfo(h, &bi);
	DWORD style = bi.wAttributes;
	COORD tmp;
	tmp.X = this->position.X + window.X;
	if (buffer.size() <= 0) { return; }
	if (opened) {
		tmp.Y = this->position.Y + window.Y;
		SetConsoleCursorPosition(h, tmp);
		cout << buffer[selected];
		for (int j = 0; j < size - buffer[selected].size()-1; j++) {
			cout << " ";
		}
		cout << "-";
		tmp.Y = this->position.Y + 1 + window.Y;
		SetConsoleCursorPosition(h, tmp);
		for (int j = 0; j < size; j++) {
			cout << "-";
		}

		for (int i = 0; i<buffer.size(); ++i) {
			//if (i+2+position.Y == bi.dwCursorPosition.Y) {
			//	SetConsoleTextAttribute(h, color_to_rgb(background, foreground));
			//} else
			if (i == selected) {
				SetConsoleTextAttribute(h, style | BACKGROUND_INTENSITY);
			}
			else {
				SetConsoleTextAttribute(h, style);
			}
			tmp.Y = this->position.Y + i + 2 + window.Y;
			SetConsoleCursorPosition(h, tmp);
			cout << buffer[i];                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
			for (int j = 0; j < size - buffer[i].size(); j++) {
				cout << " ";
			}
		}
	}
	else {
		COORD p = pos();
		p.X += window.X;
		p.Y += window.Y;
		SetConsoleCursorPosition(h, p);
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
		if (borderType == NONE) {
			tmp.X = this->position.X - 1 + window.X;
			tmp.Y = this->position.Y + 1 + window.Y;
			SetConsoleCursorPosition(h, tmp);
			for (int j = 0; j < size + 2; j++) {
				cout << " ";
			}
		}

		for (int i = 0; i<buffer.size()+1; ++i) {
			tmp.X = this->position.X - 1 + window.X;
			tmp.Y = this->position.Y + i + 2 + window.Y;
			SetConsoleCursorPosition(h, tmp);
			for (int j = 0; j < size+2; j++) {
				cout << " ";
			}
		}
	}


}
bool ComboBox::handle_keys(PCOORD cor, COORD window, char c, int keycode) {
	if (intersects(cor, window)) {
		if (keycode >= 37 && keycode <= 40) {
			//view_invalidated = true;
			return false;
		}

		if (c == 0) { return false; }


		if (c == ' ') {
			if (opened) {
				if (cor->Y + window.Y < pos().Y + 2) {
					
				} else {
					selected = cor->Y + window.Y - 2 - pos().Y;
				}
				opened = false;
				cor->Y = pos().Y + window.Y;
				cor->X = pos().X + width() - 1 + window.X;
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



bool ComboBox::handle_clicks(PCOORD mouse, COORD window, PCOORD cursor) {
	if (intersects(mouse, window)) {
		if (opened) {
			if (mouse->Y < pos().Y + window.Y + 2) {

			}
			else {
				selected = mouse->Y - window.Y - 2 - pos().Y;
			}
			opened = false;
			cursor->Y = pos().Y + window.Y;
			cursor->X = pos().X + window.X + width() - 1;
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

void ComboBox::set_selected_index(int i)
{
	selected = i;
	view_invalidated = true;
}

int ComboBox::get_selected_index()
{
	return selected;
}

ComboBox::~ComboBox() {
}