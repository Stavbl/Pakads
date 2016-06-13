#include "FormEngine.h"
#include <iostream>


using namespace std;


EventEngine::EventEngine() {
}

void EventEngine::Run(IControl *elem) {
	element = elem;
	_INPUT_RECORD  c;
	CONSOLE_SCREEN_BUFFER_INFO  ci;
	DWORD read = 0;
	HANDLE ih = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode = 0;
	GetConsoleMode(ih, &mode);
	SetConsoleMode(ih, mode & (~ENABLE_ECHO_INPUT) | (ENABLE_MOUSE_INPUT)& (~ENABLE_LINE_INPUT) & (~ENABLE_PROCESSED_OUTPUT) & (~ENABLE_QUICK_EDIT_MODE) | (ENABLE_EXTENDED_FLAGS)& (~ENABLE_INSERT_MODE) | ENABLE_WINDOW_INPUT);
	HANDLE oh = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD style = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(ih, style);

	validate_views(oh);
	int keyCode;
	while (1) {
		ReadConsoleInput(ih, &c, 1, &read);
		if (read == 1) {
			if (c.EventType == KEY_EVENT && c.Event.KeyEvent.bKeyDown) {
				keyCode = c.Event.KeyEvent.wVirtualKeyCode;
				GetConsoleScreenBufferInfo(oh, &ci);
				int width = ci.srWindow.Bottom - ci.srWindow.Top;
				if (handleKeys(&ci.dwCursorPosition, c.Event.KeyEvent.uChar.AsciiChar, keyCode, oh)) {
				}
				else if (keyCode == 37) {
					ci.dwCursorPosition.X = (ci.dwCursorPosition.X <= 0) ? 0 : ci.dwCursorPosition.X - 1;
				}
				else if (keyCode == 39) {
					ci.dwCursorPosition.X = (ci.dwCursorPosition.X >= ci.dwMaximumWindowSize.X) ? 0 : ci.dwCursorPosition.X + 1;
				}
				else if (keyCode == 38) {
					ci.dwCursorPosition.Y = (ci.dwCursorPosition.Y <= 0) ? 0 : ci.dwCursorPosition.Y - 1;
				}
				else if (keyCode == 40) {
					ci.dwCursorPosition.Y = (ci.dwCursorPosition.Y >= ci.dwMaximumWindowSize.Y) ? 0 : ci.dwCursorPosition.Y + 1;
				}
				SetConsoleCursorPosition(oh, ci.dwCursorPosition);
				redraw(oh, ci);
				SetConsoleCursorPosition(oh, ci.dwCursorPosition);
			}
			else if (c.EventType == MOUSE_EVENT && c.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
				GetConsoleScreenBufferInfo(oh, &ci);
				if (handleMouse(&c.Event.MouseEvent.dwMousePosition, &ci.dwCursorPosition, oh)) {
				}
				SetConsoleCursorPosition(oh, ci.dwCursorPosition);
			}
		}
		validate_views(oh);
	}


}



bool EventEngine::handleKeys(PCOORD cor, char c, int keycode, HANDLE oh) {

	if (element == NULL) { return false; }
	COORD mouse = *cor;
	COORD window;
	window.X = 0;
	window.Y = 0;
	if (c == '\t') {
		focusOnNextElement(cor, oh);
		return true;
	}
		if (element->handle_keys(cor, window, c, keycode)) {
			element->draw(oh, mouse, window);
			return true;
		}
	return false;
}

bool* EventEngine::calculateElementMap(int screen_width, int screen_height) {
	bool *map = new bool[screen_width*screen_height];
	for (int i = 0; i < screen_width * screen_height; i++) {
		map[i] = true;
	}
	if (element == NULL) { return map; }
	//for (int i = 0; i < elements.size(); i++) {
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
				int ind = ((y+ cd.Y) * screen_width + (x+cd.X));
				if (ind >= 0 && ind < screen_width * screen_height) {
					map[ind] = false;
				}
			}
		}
	//}
	return map;
}

void EventEngine::clear_screen(HANDLE oh) {

	SetConsoleTextAttribute(oh, 0);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(oh, &info);
	COORD pos;
	pos.X = 0;
	pos.Y = 0;
	SetConsoleCursorPosition(oh, pos);
	int height = info.srWindow.Bottom - info.srWindow.Top;
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

void EventEngine::redraw(HANDLE oh, CONSOLE_SCREEN_BUFFER_INFO info) {
	if (element == NULL) { return; }
	COORD screen = { 0,0 };
	COORD cursor = info.dwCursorPosition;
//	for (int i = 0; i < elements.size(); i++) {
		element->draw(oh, cursor, screen);
	//}
	//if (popup != NULL) {
	//	popup->draw(oh, cursor, screen);
	//}
}

void EventEngine::validate_views(HANDLE oh)
{
	if (element == NULL) { return; }
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(oh, &info);
	if (needsRefresh) {
		needsRefresh = false;
		clear_screen(oh);
		redraw(oh, info);
		return;
	}

	//for (int i = 0; i < elements.size(); i++) {
		if (element->needs_redraw()) {
			clear_screen(oh);
			redraw(oh, info);
		}
	//}
	SetConsoleCursorPosition(oh, info.dwCursorPosition);
}

bool EventEngine::handleMouse(PCOORD mouse, PCOORD cursor, HANDLE oh) {
	if (element == NULL) { return false; }
	COORD cr = *cursor;
	COORD screen = { 0,0 };
	/*if (popup != NULL) {
		if (popup->handle_clicks(mouse, screen, cursor)) {
			popup->draw(oh, cr, screen);
			return true;
		}
		return false;
	}*/
	//for (int i = 0; i < elements.size(); i++) {
		if (element->handle_clicks(mouse, screen, cursor)) {
			element->draw(oh, cr, screen);
			return true;
		}
	//}
	return false;
}

void EventEngine::focusOnNextElement(PCOORD pos, HANDLE oh) {
	if (element == NULL) { return; }
	//for (int i = 0; i < elements.size(); i++) {

		if (element->tabPositions().size() == 0) { return; }
		if (element->intersects(pos, { 0,0 })) {
			IControl *f = element;
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
			}
			else {
				pos->X = f->tabPositions()[index].X;
				pos->Y = f->tabPositions()[index].Y;
				return;
			}
		}
	//}
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(oh, &info);

	int index = 0;
	int distance = -1;
	int offset = pos->X + pos->Y *info.dwMaximumWindowSize.X;
	int num;
	int smallestIndex = 0;
	int smallestNum = -1;
//	for (int i = 0; i < elements.size(); i++) {
		if (element->tabPositions().size() == 0) { return; }
		num = element->pos().X + (element->pos().Y * info.dwMaximumWindowSize.X) - offset;
		if (num > 0 && (num < distance || distance == -1)) {
			distance = num;
			index = 0;
		}

		if (num < smallestNum || smallestNum == -1) {
				smallestNum = num;
				smallestIndex = 0;
		}
	//}
	
	if (distance == -1) { distance = smallestIndex; }

	if (distance != -1 && element->tabPositions().size() > 0) {
		pos->X = element->tabPositions()[0].X;
		pos->Y = element->tabPositions()[0].Y;
	}
}

EventEngine::~EventEngine() {

}
