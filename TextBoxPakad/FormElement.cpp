#include "FormElement.h"



FormElement::FormElement() : FormElement({ 0, 0 }) {
}

FormElement::FormElement(COORD position) {
	this->position = position;
}

bool FormElement::intersects(PCOORD pos) {
	return (pos->Y >= this->pos().Y
		&& pos->Y <= this->pos().Y + this->height() - 1
		&& pos->X >= this->pos().X
		&& pos->X <= this->pos().X + this->width() - 1);
}

vector<COORD> &FormElement::tabPositions() {
	return tabPosArr;
}

COORD FormElement::pos() {
	return this->position;
}