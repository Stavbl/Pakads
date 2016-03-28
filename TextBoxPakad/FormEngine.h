#pragma once
#include <Windows.h>
#include <vector>
#include "FormElement.h"
#include "RadioGroup.h"
#include "TextBox.h"
#include "CheckBoxGroup.h"
#include "ComboBox.h"

class FormEngine
{
	vector<FormElement *> elements;
public:
	FormEngine();
	void addTextBox(int x = 0, int y = 0, int len = 30);
	void addRadioGroup(vector<string> options, int x, int y);
	void addCheckBoxGroup(vector<string> options, int x, int y);
	void addComboBox(vector<string> options, int x, int y);
	void addElement(FormElement *);
	void focusOnNextElement(PCOORD pos, HANDLE oh);
	bool handleKeys(PCOORD cor, char c, int keycode, HANDLE oh);
	bool handleMouse(PCOORD mouse, PCOORD cursor, HANDLE oh);
	void redraw(HANDLE oh);
	~FormEngine();
};

