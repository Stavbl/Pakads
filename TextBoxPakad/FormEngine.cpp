#include "FormEngine.h"



FormEngine::FormEngine() {
}

void FormEngine::addTextBox(int x, int y, int len) {
	elements.push_back((FormElement *)new TextBox(x, y, len));
}

void FormEngine::addRadioGroup(vector<string> options, int x, int y) {
	RadioGroup *rg = new RadioGroup(x, y);
	for (int i = 0; i < options.size(); i++) {
		rg->addOption(options[i]);
	}
	elements.push_back((FormElement *)rg);
}

void FormEngine::addCheckBoxGroup(vector<string> options, int x, int y) {
	CheckBoxGroup *rg = new CheckBoxGroup(x, y);
	for (int i = 0; i < options.size(); i++) {
		rg->addOption(options[i]);
	}
	elements.push_back((FormElement *)rg);
}

void FormEngine::addComboBox(vector<string> options, int x, int y) {
	ComboBox *rg = new ComboBox(x, y);
	for (int i = 0; i < options.size(); i++) {
		rg->addOption(options[i]);
	}
	elements.push_back((FormElement *)rg);
}

void FormEngine::addElement(FormElement *elem) {
	elements.push_back(elem);
}



bool FormEngine::handleKeys(PCOORD cor, char c, int keycode, HANDLE oh) {
	if (c == '\t') {
		focusOnNextElement(cor, oh);
		return true;
	}
	for (int i = 0; i < elements.size(); i++) {
		if (elements[i]->handle_keys(cor, c, keycode)) {
			elements[i]->print(oh);
			return true;
		}
	}
	return false;
}

void FormEngine::redraw(HANDLE oh) {
	for (int i = 0; i < elements.size(); i++) {
		elements[i]->print(oh);
	}
}

bool FormEngine::handleMouse(PCOORD mouse, PCOORD cursor, HANDLE oh) {
	for (int i = 0; i < elements.size(); i++) {
		if (elements[i]->handle_clicks(mouse, cursor)) {
			elements[i]->print(oh);
			return true;
		}
	}
	return false;
}

void FormEngine::focusOnNextElement(PCOORD pos, HANDLE oh) {
	if (elements.size() == 0) { return; }
	for (int i = 0; i < elements.size(); i++) {
		if (elements[i]->intersects(pos)) {
			FormElement *f = elements[i];
			int index = -1;
			int min = -1;
			for (int j = 0; j < f->tabPositions().size(); j++) {
				COORD tb = f->tabPositions()[j];
				int dist = ((tb.Y - pos->Y) * f->width());
				if (dist == 0) {
					dist = tb.X - pos->X;
				} else {
					dist += f->pos().X + f->width() - pos->X;
					dist += tb.X - f->pos().X;
				}

				if (dist > 0 && (dist < min || min == -1)) {
					min = dist;
					index = j;
				}
			}
			if (index == -1) {
				pos->X = f->pos().X + f->width() + 1;
				pos->Y = f->pos().Y;
				break;
			}
			else {
				pos->X = f->tabPositions()[index].X;
				pos->Y = f->tabPositions()[index].Y;
				return;
			}
		}
	}
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(oh, &info);

	int index = 0;
	int distance = -1;
	int offset = pos->X + pos->Y *info.dwMaximumWindowSize.X;
	int num;
	int smallestIndex = 0;
	int smallestNum = -1;
	for (int i = 0; i < elements.size(); i++) {
		num = elements[i]->pos().X + (elements[i]->pos().Y * info.dwMaximumWindowSize.X) - offset;
		if (num > 0 && (num < distance || distance == -1)) {
			distance = num;
			index = i;
		}

		if (num < smallestNum || smallestNum == -1) {
				smallestNum = num;
				smallestIndex = i;
		}
	}
	
	if (distance == -1) { distance = smallestIndex; }

	if (distance != -1) {
		pos->X = elements[index]->tabPositions()[0].X;
		pos->Y = elements[index]->tabPositions()[0].Y;
	}
}

FormEngine::~FormEngine() {
	for (int i = 0; i < elements.size(); i++) {
		delete elements[i];
	}
}
