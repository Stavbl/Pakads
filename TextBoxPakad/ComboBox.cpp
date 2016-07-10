#include "ComboBox.h"
#include <iostream>

using namespace std;

ComboBox::ComboBox(int width, vector<string> entries, int x, int y) {
	position.X = x;
	position.Y = y;
	int i = 0;
	size = width;
	TabPosition tp;
	tp.pos.X = size;
	tp.pos.Y = 1;
	tp.element = this;
	closedTabPos.push_back(tp);
	tabPosArr.push_back(tp);
	for (int i = 0; i < entries.size(); i++) {
		addOption(entries[i]);
	}
	view = new View(x, y, width + 2, entries.size() + 4);
}

void ComboBox::addOption(string str) {
	if (str.size()+2 > size) {
		size = str.size()+2;
	}
	selected = 0;
	buffer.push_back(str);
	TabPosition tp;
	tp.pos.X = 1;
	tp.pos.Y = tabPosArr.size()+2;
	tp.element = this;
	tabPosArr.push_back(tp);
	tp.active = false;
	closedTabPos.push_back(tp);
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
			if (i+2+position.Y + window.Y == cursor.Y && intersects(&cursor, window)) {
				SetConsoleTextAttribute(h, color_to_rgb(background, foreground));
			} else
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
		if (keycode == 38) {
			cor->Y -= 1;
			cor->X = pos().X + width() + 1;
			if (cor->Y == pos().Y + 1) {
				cor->Y = pos().Y + height() + 1;
			}
			if (cor->Y == pos().Y + 3) {
				cor->Y = pos().Y + 2;
			}
			return true;
		}
		else if (keycode == 40) {
			cor->Y += 1; 
			if (cor->Y == pos().Y + 3) {
				cor->Y = pos().Y + 4;
			}
			if (cor->Y == pos().Y + height() + 2) {
				cor->Y = pos().Y + 2;
			}
			return true;
		}
		if (keycode >= 37 && keycode <= 40) {
			return true;
		}

		if (keycode == 0x0D) { c == ' '; }
		if (c == 0) { return true; }


		if (c == ' ') {
			if (opened) {
				if (cor->Y - window.Y < pos().Y + 2) {
					
				} else {
					selected = cor->Y - window.Y - 2 - pos().Y;
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

void ComboBox::setActive(bool active)
{
	this->active = active;
	if (this->opened && !active) {
		this->opened = false;
	}
}

vector<TabPosition> ComboBox::tabPositions() {
	if (opened) {
		return tabPosArr;
	}
	else {
		return closedTabPos;
	}
}

void ComboBox::updateView(COORD cursor)
{
	int layer = (active) ? 2 : 1;
	/*int layer = 1;
	if (cursor.X > 0 && cursor.X < width() + 2 && cursor.Y > 0 && cursor.Y < height() + 2) {
		layer = 2;
	}*/
	view->clearAll(foreground, background);
	updateBorder(layer);
	if (buffer.size() <= 0) { return; }
	if (opened) {
		if (selected != -1) {
			for (int i = 0; i < buffer[selected].size(); i++) {
				view->update(1 + i, 1, buffer[selected][i], foreground, background, layer);
			}
		}
		if (cursor.Y != 1 || cursor.X != width()) {
			view->update(width(), 1, '-', foreground, background, layer);
		}
		else {
			view->update(width(), 1, '-', background, foreground, layer);
		}
		for (int i = 0; i < width(); i++) {
			view->update(i + 1, 2, '-', foreground, background, layer);
		}
		

		for (int i = 0; i<buffer.size(); ++i) {
			if (cursor.X > 0 && cursor.X < width()+2 && cursor.Y == i+2) {
				for (int j = 0; j < buffer[i].size(); j++) {
					view->update(1 + j, i+3, buffer[i][j], foreground, background, layer);
				}
			}
			else {
				for (int j = 0; j < buffer[i].size(); j++) {
					view->update(1 + j, i + 3, buffer[i][j], foreground, background, layer);
				}
			}
			if (active && 3 + i == cursor.Y && cursor.X > 0 && cursor.X < width() + 2) {
				for (int j = 0; j < width(); j++) {
					view->fcolor[((i + 3)*view->width) + j + 1] = background;
					view->bcolor[((i + 3)*view->width) + j + 1] = foreground;
				}
			}
		}
		
		view->solidifyBackground(layer);
		view->clearBorder();
	}
	else {

		if (selected != -1) {
			for (int i = 0; i < buffer[selected].size(); i++) {
				view->update(1 + i, 1, buffer[selected][i], foreground, background, layer);
			}
		}
		if (cursor.Y != 1 || cursor.X != width()) {
			view->update(width(), 1, '+', foreground, background, layer);
		}
		else {
			view->update(width(), 1, '+', background, foreground, layer);
		}
		for (int i = 0; i < width(); i++) {
			view->layer[width() + 2 + i + 1] = 1;
		}
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

void ComboBox::SetSelectedIndex(size_t i)
{
	selected = i;
	view_invalidated = true;
}

size_t ComboBox::GetSelectedIndex()
{
	return selected;
}

ComboBox::~ComboBox() {
	delete view;
}