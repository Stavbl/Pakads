#include "Panel.h"
#include <iostream>

using namespace std;

Panel::Panel(int height, int width, int x, int y)
{
	size = width;
	sizeh = height;
	position.X = x;
	position.Y = y;
}

int Panel::width()
{
	return size;
}

int Panel::height()
{
	return sizeh;
}

COORD Panel::pos()
{
	return position;
}

vector<COORD>& Panel::tabPositions()
{
	vector<COORD> tabs;
	for (int i = 0; i < elements.size(); i++) {
		vector<COORD> tb = elements[i]->tabPositions();
		tabs.insert(tabs.end(), tb.begin(), tb.end());
	}
	return tabs;
}

bool* Panel::calculateElementMap(int screen_width, int screen_height) {
	bool *map = new bool[screen_width*screen_height];
	for (int i = 0; i < screen_width * screen_height; i++) {
		map[i] = true;
	}
	if (elements.size() == 0) { return map; }
	for (int i = 0; i < elements.size(); i++) {
		IControl *element = elements[i];
		COORD cd = element->pos();
		int width = element->width();
		int height = element->height();
		if (element->has_border()) {
			cd.X -= 1;
			cd.Y -= 1;
			height += 2;
			width += 2;
		}
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				int ind = ((y + cd.Y) * screen_width + (x + cd.X));
				if (ind >= 0 && ind < screen_width * screen_height) {
					map[ind] = false;
				}
			}
		}
	}
	return map;
}

void Panel::print(HANDLE h, COORD cursor, COORD window)
{
	window.X += position.X;
	window.Y += position.Y;
	COORD p = window;
	bool *map = calculateElementMap(width(), height());
	/*
	for (int i = 0; i < sizeh; i++) {
		SetConsoleCursorPosition(h, p);
		for (int j = 0; j < size; j++) {
			if (map[i*height() + j]) {
				cout << " ";
				p.X += 1;
			}
			else {
				p.X += 1;
				SetConsoleCursorPosition(h, p);
			}
		}
		p.X = window.X;
		p.Y += 1;
	}*/
	for (int i = 0; i < elements.size(); i++) {
		elements[i]->draw(h, cursor, window);
	}
}

void Panel::AddControl(IControl * elem, int left, int top)
{
	elements.push_back(elem);
	COORD pos = { left, top };
	elem->set_position(pos);
	view_invalidated = true;
}

bool Panel::handle_keys(PCOORD x, COORD window, char c, int keycode)
{
	window.X += position.X;
	window.Y += position.Y;
	for (int i = 0; i < elements.size(); i++) {
		if (elements[i]->handle_keys(x, window, c, keycode)) {
			return true;
		}
	}
	return false;
}

bool Panel::handle_clicks(PCOORD mouse, COORD window, PCOORD cursor)
{
	window.X += position.X;
	window.Y += position.Y;
	for (int i = 0; i < elements.size(); i++) {
		if (elements[i]->handle_clicks(mouse, window, cursor)) {
			return true;
		}
	}
	return false;
}
