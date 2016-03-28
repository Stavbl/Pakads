#pragma once
#include <Windows.h>
#include "FormElement.h"

class TextBox : FormElement
{
private:
	char *buffer;
	int len;
	int size;
public:
	TextBox(int x = 0, int y = 0, int size =  30);
	int width();
	int height();
	COORD pos();
	void print(HANDLE h);
	bool handle_keys(PCOORD x, char c, int keycode);
	bool handle_clicks(PCOORD mouse, PCOORD cursor);
	void shift(int j, char c);
	void delete_char(int j);
	~TextBox();
};