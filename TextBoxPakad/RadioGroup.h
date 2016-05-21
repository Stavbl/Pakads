#pragma once
#include <Windows.h>
#include <string>
#include "FormElement.h"
#include <vector>

using namespace std;

class RadioGroup : public FormElement
{
private:
	vector<string> buffer;
	int selected;
	int size;
	int sizeh;
public:
	RadioGroup(int height, int width, vector<string> entries, int x = 0, int y = 0);
	void addOption(string str);
	int width();
	int height();
	void print(HANDLE h, COORD cursor, COORD window);
	bool handle_keys(PCOORD x, COORD window, char c, int keycode);
	bool handle_clicks(PCOORD mouse, COORD window, PCOORD cursor);
	size_t getSelectedIndex();
	bool setSelectedIndex(size_t index);
	bool clearSelection();
	~RadioGroup();
};

