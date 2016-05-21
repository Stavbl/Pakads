#pragma once
#include <Windows.h>
#include "FormElement.h"

class Label : public FormElement
{
private:
	char *buffer;
	int size;
public:
	Label(int width, int x = 0, int y = 0, char *txt = NULL);
	int width();
	int height();
	COORD pos();
	void print(HANDLE h, COORD cursor, COORD window);
	bool handle_keys(PCOORD x, COORD window, char c, int keycode);
	bool handle_clicks(PCOORD mouse, COORD window, PCOORD cursor);
	void set_text(const char *txt);
	virtual ~Label();
};
