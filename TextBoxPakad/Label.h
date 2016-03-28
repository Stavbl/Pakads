#pragma once
#include <Windows.h>
#include "FormElement.h"

class Label : FormElement
{
private:
	char *buffer;
	int size;
public:
	Label(int x = 0, int y = 0, char *txt = NULL);
	int width();
	int height();
	COORD pos();
	void print(HANDLE h);
	bool handle_keys(PCOORD x, char c, int keycode);
	bool handle_clicks(PCOORD mouse, PCOORD cursor);
	~Label();
};
