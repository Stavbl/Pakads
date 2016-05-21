#include "FormEngine.h"
#include <iostream>


using namespace std;


FormEngine::FormEngine() {
}

//void FormEngine::addTextBox(int x, int y, int len) {
//	TextBox *tb = new TextBox(x, y, len);
//	tb->set_background_color(YELLOW);
//	tb->set_foreground_color(BLACK);
//	elements.push_back((FormElement *)tb);
//}

//void FormEngine::addRadioGroup(vector<string> options, int x, int y) {
//	RadioGroup *rg = new RadioGroup(x, y);
//	for (int i = 0; i < options.size(); i++) {
//		rg->addOption(options[i]);
//	}
//	rg->set_border(SINGLE_LINE);
//	elements.push_back((FormElement *)rg);
//}
//
//void FormEngine::addCheckBoxGroup(vector<string> options, int x, int y) {
//	CheckBoxGroup *rg = new CheckBoxGroup(x, y);
//	for (int i = 0; i < options.size(); i++) {
//		rg->addOption(options[i]);
//	}
//	elements.push_back((FormElement *)rg);
//}
//
//void FormEngine::addComboBox(vector<string> options, int x, int y) {
//	ComboBox *rg = new ComboBox(x, y);
//	for (int i = 0; i < options.size(); i++) {
//		rg->addOption(options[i]);
//	}
//	rg->set_border(DOUBLE_LINE);
//	rg->set_foreground_color(PURPLE);
//	elements.push_back((FormElement *)rg);
//}

void FormEngine::addElement(FormElement *elem) {
	elements.push_back(elem);
}



bool FormEngine::handleKeys(PCOORD cor, char c, int keycode, HANDLE oh) {
	COORD mouse = *cor;
	COORD window;
	window.X = 0;
	window.Y = 0;
	if (c == '\t') {
		focusOnNextElement(cor, oh);
		return true;
	}
	for (int i = 0; i < elements.size(); i++) {
		if (elements[i]->handle_keys(cor, window, c, keycode)) {
			elements[i]->draw(oh, mouse, window);
			return true;
		}
	}
	return false;
}

bool* FormEngine::calculateElementMap(int screen_width, int screen_height) {
	bool *map = new bool[screen_width*screen_height];
	for (int i = 0; i < screen_width * screen_height; i++) {
		map[i] = true;
	}
	for (int i = 0; i < elements.size(); i++) {
		COORD cd = elements[i]->pos();
		int width = elements[i]->width();
		int height = elements[i]->height();
		if (elements[i]->has_border()) {
			cd.X -= 1;
			cd.Y -= 1;
			height += 2;
			width += 2;
		}
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				int ind = ((y+ cd.Y) * screen_width + (x+cd.X));
				if (ind >= 0 && ind < screen_width * screen_height) {
					map[ind] = false;
				}
			}
		}
	}
	return map;
}

void FormEngine::clear_screen(HANDLE oh) {
	SetConsoleTextAttribute(oh, 0);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(oh, &info);
	COORD pos;
	pos.X = 0;
	pos.Y = 0;
	SetConsoleCursorPosition(oh, pos);
	int height = -1;
	for (int i = 0; i < elements.size(); i++) {
		int size = elements[i]->pos().Y + elements[i]->height() + (elements[i]->has_border() ? 1 : 0);
		if (size > height) {
			height = size;
		}
	}
	if (height == -1) {
		height = info.dwMaximumWindowSize.Y;
	}
	bool *map = calculateElementMap(info.dwMaximumWindowSize.X, height);
	for (int y = 0; y < height; y++) {
		SetConsoleCursorPosition(oh, pos);
		for (int x = 0; x < info.dwMaximumWindowSize.X; x++) {
			pos.X += 1;
			if (map[y * info.dwMaximumWindowSize.X + x]) {
				cout << " ";
			}
			else {
				SetConsoleCursorPosition(oh, pos);
			}
		}

		pos.Y += 1;
		pos.X = 0;
	}
	delete[] map;
}

void FormEngine::redraw(HANDLE oh) {
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(oh, &info);
	COORD screen = { 0,0 };
	COORD cursor = info.dwCursorPosition;
	for (int i = 0; i < elements.size(); i++) {
		elements[i]->draw(oh, cursor, screen);
	}
}

void FormEngine::validate_views(HANDLE oh)
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(oh, &info);
	for (int i = 0; i < elements.size(); i++) {
		if (elements[i]->needs_redraw()) {
			clear_screen(oh);
			redraw(oh);
			break;
		}
	}
	SetConsoleCursorPosition(oh, info.dwCursorPosition);
}

bool FormEngine::handleMouse(PCOORD mouse, PCOORD cursor, HANDLE oh) {
	COORD cr = *cursor;
	COORD screen = { 0,0 };
	for (int i = 0; i < elements.size(); i++) {
		if (elements[i]->handle_clicks(mouse, screen, cursor)) {
			elements[i]->draw(oh, cr, screen);
			return true;
		}
	}
	return false;
}

void FormEngine::focusOnNextElement(PCOORD pos, HANDLE oh) {
	if (elements.size() == 0) { return; }
	for (int i = 0; i < elements.size(); i++) {

		if (elements[i]->tabPositions().size() == 0) { continue; }
		if (elements[i]->intersects(pos, { 0,0 })) {
			FormElement *f = elements[i];
			int index = -1;
			int min = -1;
			for (int j = 0; j < f->tabPositions().size(); j++) {
				COORD tb = f->tabPositions()[j];
				int dist = ((tb.Y - pos->Y) * f->width());
				if (dist == 0) {
					dist = tb.X - pos->X;
				} else {
					dist += f->pos().X + f->width() - pos->X;
					dist += tb.X - f->pos().X;
				}

				if (dist > 0 && (dist < min || min == -1)) {
					min = dist;
					index = j;
				}
			}
			if (index == -1) {
				pos->X = f->pos().X + f->width() + 1;
				pos->Y = f->pos().Y;
				break;
			}
			else {
				pos->X = f->tabPositions()[index].X;
				pos->Y = f->tabPositions()[index].Y;
				return;
			}
		}
	}
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(oh, &info);

	int index = 0;
	int distance = -1;
	int offset = pos->X + pos->Y *info.dwMaximumWindowSize.X;
	int num;
	int smallestIndex = 0;
	int smallestNum = -1;
	for (int i = 0; i < elements.size(); i++) {
		if (elements[i]->tabPositions().size() == 0) { continue; }
		num = elements[i]->pos().X + (elements[i]->pos().Y * info.dwMaximumWindowSize.X) - offset;
		if (num > 0 && (num < distance || distance == -1)) {
			distance = num;
			index = i;
		}

		if (num < smallestNum || smallestNum == -1) {
				smallestNum = num;
				smallestIndex = i;
		}
	}
	
	if (distance == -1) { distance = smallestIndex; }

	if (distance != -1) {
		pos->X = elements[index]->tabPositions()[0].X;
		pos->Y = elements[index]->tabPositions()[0].Y;
	}
}

FormEngine::~FormEngine() {
	for (int i = 0; i < elements.size(); i++) {
		delete elements[i];
	}
}
