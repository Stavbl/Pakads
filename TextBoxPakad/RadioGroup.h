#pragma once
#include <Windows.h>
#include <string>
#include "FormElement.h"
#include <vector>

using namespace std;

class RadioGroup : FormElement
{
private:
	vector<string> buffer;
	int selected;
	int size;
public:
	RadioGroup(int x = 0, int y = 0);
	void addOption(string str);
	int width();
	int height();
	void print(HANDLE h);
	bool handle_keys(PCOORD x, char c, int keycode);
	bool handle_clicks(PCOORD mouse, PCOORD cursor);
	~RadioGroup();
};

