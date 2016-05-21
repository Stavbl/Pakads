#pragma once
#include <Windows.h>
#include <string>
#include "FormElement.h"
#include <vector>

using namespace std;

class CheckBoxGroup : public FormElement
{
private:
	vector<string> buffer;
	vector<bool> selected;
	int size;
	int sizeh;
public:
	CheckBoxGroup(int height, int width, vector<string> entries, int x = 0, int y = 0);
	void addOption(string str);
	int width();
	int height();
	void print(HANDLE h, COORD cursor, COORD window);
	bool handle_keys(PCOORD x, COORD window, char c, int keycode);
	bool handle_clicks(PCOORD mouse, COORD window, PCOORD cursor);
	void selectIndex(size_t i);
	void deselectIndex(size_t i);
	vector<size_t> getSelectedIndicies();
	~CheckBoxGroup();
};

