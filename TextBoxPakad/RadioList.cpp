#include "RadioList.h"
#include <iostream>

using namespace std;

RadioList::RadioList(int height, int width, vector<string> entries, int x, int y) : selected(-1) {
	position.X = x;
	position.Y = y;
	int i = 0;
	size = width;
	sizeh = height;
	for (int i = 0; i < entries.size(); i++) {
		addOption(entries[i]);
	}
	view = new View(x, y, this->width() + 2, this->height() + 2);
}

void RadioList::addOption(string str) {
	if (str.size() > size) {
		size = str.size();
	}
	buffer.push_back(str);
	TabPosition tp;
	tp.pos.X = 2;
	tp.pos.Y = tabPosArr.size()+1;
	tp.element = this;
	tabPosArr.push_back(tp);
	if (buffer.size() > sizeh) {
		sizeh = buffer.size();
	}	
}

void RadioList::print(HANDLE h, COORD cursor, COORD window) {
	COORD tmp;
	tmp.X = this->position.X + window.X;
	DWORD style = color_to_rgb(foreground, background);
	for (int i = 0; i<buffer.size(); ++i) {
		if (i + position.Y + window.Y == cursor.Y && intersects(&cursor, window)) {
			SetConsoleTextAttribute(h, color_to_rgb(background, foreground));
		}
		else {
			SetConsoleTextAttribute(h, style);
		}
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
	SetConsoleTextAttribute(h, style);


	for (int j = 0; j < sizeh - buffer.size(); j++) {
		tmp.Y = this->position.Y + j + buffer.size() + window.Y;
		SetConsoleCursorPosition(h, tmp);
		for (int i = 0; i < size; i++) {
			cout << " ";
		}
	}
}

bool RadioList::handle_keys(PCOORD cor, COORD window, char c, int keycode) {
	if (intersects(cor, window)) {
		if (keycode == 38) {
			cor->Y -= 1;
			if (cor->Y == pos().Y+1) {
				cor->Y = pos().Y + sizeh+1;
			}
			return true;
		}
		else if (keycode == 40) {
			cor->Y += 1;
			if (cor->Y == pos().Y + sizeh+2) {
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

int RadioList::width() {
	return size + 4;
}

int RadioList::height() {
	return buffer.size();
}


bool RadioList::handle_clicks(PCOORD mouse, COORD window, PCOORD cursor) {
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

size_t RadioList::GetSelectedIndex()
{
	return selected;
}

void RadioList::SetSelectedIndex(size_t index)
{
	if (index >= buffer.size() || index < 0) {
		return;
	}
	selected = index;
	view_invalidated = true;
}

void RadioList::ClearSelection()
{
	selected = -1;
}

void RadioList::updateView(COORD cursor)
{
	int layer = (active) ? 2 : 1;
	/*if (cursor.X > 0 && cursor.X < width() + 2 && cursor.Y > 0 && cursor.Y < height() + 2) {
		layer = 2;

	}*/
	view->clearAll(foreground, background);
	view->solidifyBackground();
	updateBorder(layer);

	ForegroundColor fc = foreground;
	BackgroundColor bc = background;
	for (int i = 0; i<buffer.size(); ++i) {
		fc = foreground;
		bc = background;
		
		view->update(1, i + 1, '(', fc, bc, layer);
		if (selected == i) {
			view->update(2, i + 1, '*', fc, bc, layer);
		}
		view->update(3, i + 1, ')', fc, bc, layer);
		for (int j = 0; j < buffer[i].size(); j++) {
			view->update(5+j, i + 1, buffer[i][j], fc, bc, layer);
		}
		if (active && 1 + i == cursor.Y && cursor.X > 0 && cursor.X < width() + 2) {
			for (int j = 0; j < width(); j++) {
				view->fcolor[((i + 1)*view->width) + j+1] = bc;
				view->bcolor[((i + 1)*view->width) + j+1] = fc;
			}
		}
	}
}

RadioList::~RadioList() {
	delete view;
}