#pragma once
#include <Windows.h>
#include <string>
#include "FormElement.h"
#include <vector>

using namespace std;

class CheckBoxGroup : FormElement
{
private:
	vector<string> buffer;
	vector<bool> selected;
	int size;
public:
	CheckBoxGroup(int x = 0, int y = 0);
	void addOption(string str);
	int width();
	int height();
	void print(HANDLE h);
	bool handle_keys(PCOORD x, char c, int keycode);
	bool handle_clicks(PCOORD mouse, PCOORD cursor);
	~CheckBoxGroup();
};

