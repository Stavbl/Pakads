#include "IControl.h"
#include <iostream>
using namespace std;

IControl::IControl() : IControl({ 0, 0 }) {
}

IControl::IControl(COORD position) {
	this->position = position;
}

bool IControl::has_border() {
	return borderType != NONE;
}

void IControl::Show()
{
	hidden = false;
	view_invalidated = true;
}

void IControl::Hide()
{
	hidden = true;
	view_invalidated = true;
}

void IControl::SetBorder(BorderType border) {
	borderType = border;
	view_invalidated = true;
}

void IControl::print_border(HANDLE h, COORD window) {
	if (borderType == NONE) { return; }
	//pieces                              ┌       ┐       └        ┘      │       ─
	const char borderParts[2][6] = { { '\xDA', '\xBF', '\xC0', '\xD9', '\xB3', '\xC4' },
	{ '\xC9', '\xBB', '\xC8', '\xBC', '\xBA', '\xCD' } };

	COORD pos = this->position;
	pos.X += window.X;
	pos.Y += window.Y;
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(h, &info);

	// left top corner
	if (pos.X > 0 && pos.Y > 0) {
		pos.X = this->position.X + window.X - 1;
		pos.Y = this->position.Y + window.Y - 1;
		SetConsoleCursorPosition(h, pos);
		cout << borderParts[borderType][0];
	}

	// top bar
	if (this->position.Y + window.Y > 0) {
		pos.X = this->position.X + window.X;
		pos.Y = this->position.Y + window.Y - 1;
		SetConsoleCursorPosition(h, pos);
		for (int i = 0; i < width(); i++) {
			cout << borderParts[borderType][5];
		}
	}

	// right top corner
	if (position.Y + window.Y > 0 && position.X + window.X + width() < info.dwMaximumWindowSize.X); {
		pos.X = this->position.X + width() + window.X;
		pos.Y = this->position.Y + window.Y - 1;
		SetConsoleCursorPosition(h, pos);
		cout << borderParts[borderType][1];
	}

	// left side
	if (position.X + window.X > 0) {
		pos.X = this->position.X  + window.X - 1;
		pos.Y = this->position.Y + window.Y - 1;
		for (int i = 0; i < height(); i++) {
			pos.Y += 1;
			SetConsoleCursorPosition(h, pos);
			cout << borderParts[borderType][4];
		}
	}

	// right side
	if (position.X + window.X + width() < info.dwMaximumWindowSize.X) {
		pos.X = this->position.X  + window.X + width();
		pos.Y = this->position.Y  + window.Y - 1;
		for (int i = 0; i < height(); i++) {
			pos.Y += 1;
			SetConsoleCursorPosition(h, pos);
			cout << borderParts[borderType][4];
		}
	}

	// bottom left corner
	if (position.X + window.X > 0 && position.Y + window.Y + height() < info.dwMaximumWindowSize.Y) {
		pos.X = this->position.X + window.X - 1;
		pos.Y = this->position.Y + window.Y + height();
		SetConsoleCursorPosition(h, pos);
		cout << borderParts[borderType][2];
	}

	// bottom right corner
	if (position.X + window.X + width() < info.dwMaximumWindowSize.X && position.Y + window.Y + height() < info.dwMaximumWindowSize.Y) {
		pos.X = this->position.X + window.X + width();
		pos.Y = this->position.Y + window.Y + height();
		SetConsoleCursorPosition(h, pos);
		cout << borderParts[borderType][3];
	}

	// bottom line
	if (position.Y + window.Y + height() < info.dwMaximumWindowSize.Y) {
		pos.X = this->position.X + window.X;
		pos.Y = this->position.Y + window.Y + height();
		SetConsoleCursorPosition(h, pos);
		for (int i = 0; i < width(); i++) {
			cout << borderParts[borderType][5];
		}
	}
}

DWORD color_to_rgb(ForegroundColor front, BackgroundColor back) {
	DWORD s = 0;
	switch (front)
	{
	case RED:
		s = s | FOREGROUND_RED;
		break;
	case GREEN:
		s = s | FOREGROUND_GREEN;
		break;
	case BLUE:
		s = s | FOREGROUND_BLUE;
		break;
	case TEAL:
		s = s | FOREGROUND_BLUE | FOREGROUND_GREEN;
		break;
	case WHITE:
		s = s | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
		break;
	case BLACK:
		break;
	case YELLOW:
		s = s | FOREGROUND_GREEN | FOREGROUND_INTENSITY | FOREGROUND_RED;
		break;
	case PURPLE:
		s = s | FOREGROUND_BLUE | FOREGROUND_RED;
		break;
	default:
		break;
	}

	switch (back)
	{
	case RED:
		s = s | BACKGROUND_RED;
		break;
	case GREEN:
		s = s | BACKGROUND_GREEN;
		break;
	case BLUE:
		s = s | BACKGROUND_BLUE;
		break;
	case TEAL:
		s = s | BACKGROUND_BLUE | BACKGROUND_GREEN;
		break;
	case WHITE:
		s = s | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY;
		break;
	case BLACK:
		break;
	case YELLOW:
		s = s | BACKGROUND_GREEN | BACKGROUND_INTENSITY | BACKGROUND_RED;
		break;
	case PURPLE:
		s = s | BACKGROUND_BLUE | BACKGROUND_RED;
		break;
	default:
		break;
	}
	return s;
}

void IControl::draw(HANDLE h, COORD cursor, COORD window) {
	if (hidden) {
		return;
	}
	CONSOLE_SCREEN_BUFFER_INFO bi;
	GetConsoleScreenBufferInfo(h, &bi);
	DWORD old = bi.wAttributes;
	DWORD style = color_to_rgb(this->foreground, this->background);
	SetConsoleTextAttribute(h, style);
	print_border(h, window);
	print(h, cursor, window);
	SetConsoleTextAttribute(h, old);
	view_invalidated = false;
}


void IControl::SetForeground(ForegroundColor color) {
	foreground = color;
	view_invalidated = true;
}

void IControl::SetBackground(ForegroundColor color) {
	background = color;
	view_invalidated = true;
}


bool IControl::needs_redraw()
{
	return view_invalidated;
}

void IControl::set_position(COORD pos)
{
	position.X = pos.X;
	position.Y = pos.Y;
	view->x = pos.X;
	view->y = pos.Y;
	view_invalidated = true;
}

bool IControl::intersects(PCOORD pos, COORD window) {
	if (hidden) { return false; }
	return (pos->Y >= this->pos().Y + window.Y
		&& pos->Y <= this->pos().Y + window.Y + this->height() - 1
		&& pos->X >= this->pos().X + window.X
		&& pos->X <= this->pos().X + window.X + this->width() - 1);
}

vector<TabPosition> IControl::tabPositions() {
	return tabPosArr;
}

COORD IControl::pos() {
	return this->position;
}

const View *IControl::getView() {
	return view;
}

void IControl::setActive(bool active)
{
	this->active = active;
}

void IControl::updateView(COORD cursor) {
	
}

void IControl::updateBorder(int layer)
{
	if (borderType == NONE) {
		view->clearBorder();
		return; 
	}
	//pieces                              ┌       ┐       └        ┘      │       ─
	const char borderParts[2][6] = { { '\xDA', '\xBF', '\xC0', '\xD9', '\xB3', '\xC4' },
	{ '\xC9', '\xBB', '\xC8', '\xBC', '\xBA', '\xCD' } };

	view->update(0, 0, borderParts[borderType][0], foreground, background, layer);
	view->update(width()+1, 0, borderParts[borderType][1], foreground, background, layer);
	view->update(0, height()+1, borderParts[borderType][2], foreground, background, layer);
	view->update(width()+1, height()+1, borderParts[borderType][3], foreground, background, layer);
	for (int i = 0; i < width(); i++) {
		view->update(1+i, 0, borderParts[borderType][5], foreground, background, layer);
		view->update(1 + i, height()+1, borderParts[borderType][5], foreground, background, layer);
	}
	for (int i = 0; i < height(); i++) {
		view->update(0, i+1, borderParts[borderType][4], foreground, background, layer);
		view->update(width()+1, i+1, borderParts[borderType][4], foreground, background, layer);
	}
}
