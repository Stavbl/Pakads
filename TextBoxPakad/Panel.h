#pragma once
#include <Windows.h>
#include "FormElement.h"

class Panel : public FormElement
{
private:
	int size;
	int sizeh;
	vector<FormElement *> elements;
public:
	Panel(int width, int height, int x = 0, int y = 0);
	int width();
	int height();
	COORD pos();
	virtual vector<COORD> &tabPositions();
	void print(HANDLE h, COORD cursor, COORD window);
	void addElement(FormElement *elem);
	bool handle_keys(PCOORD x, COORD window, char c, int keycode);
	bool handle_clicks(PCOORD mouse, COORD window, PCOORD cursor);
};
