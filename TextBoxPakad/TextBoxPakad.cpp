#include <iostream>
#include <Windows.h>
#include "TextBox.h"
#include "FormEngine.h"
#include "RadioList.h"
#include "ComboBox.h"
#include "TextBoxPakad.h"
#include "Label.h"
#include "Button.h"
#include "NumericBox.h"
#include "Panel.h"
#include "Popup.h"

using namespace std;

struct PrintListener : public MouseListener {
	int times = 0;
	COORD pos;
	TextBox tb;
	PrintListener() : tb(20, 20, 10) {
		tb.SetValue("Button");
		pos.X = 0;
		pos.Y = 0;
		tb.SetBackground(PURPLE);
	}
	void MousePressed(IControl &element, int x, int y, bool is_left) {
		pos.X += 1;
		pos.Y += 1;
		tb.set_position(pos);
		element.SetBackground(times % 2 == 0 ? PURPLE : BLUE);
	}
};

struct PopupClicker : public PopupEvent {
	void PopupClicked(PopupResult res) {

	}
};

int main() {


	EventEngine engine;
	//Panel ch = Panel(30, 12, 10, 3);
	//ch.set_border(DOUBLE_LINE);
	//ch.set_foreground_color(GREEN);
	//engine.addElement(&ch);

	//Label lbl = Label(8, 2, 2, "Label");
	/*vector<string> opts;
	opts.push_back("One");
	opts.push_back("Stav Blyman");
	opts.push_back("Three");
	RadioGroup rg = RadioGroup(3, 20, opts, 2, 6);*/
	//Panel p2 = Panel(15, 4, 12, 6);
	//p2.set_background_color(WHITE);
	//p2.set_border(SINGLE_LINE);
	//p2.set_foreground_color(BLACK);
	//NumericBox nb = NumericBox(6, 0, 100, 4, 3);
	//p2.addElement(&nb);
	////ch.addElement(&p2);
	//ch.addElement(&lbl);
	//ch.addElement(&rg);
	//ch.addElement(&nb);
	//PopupClicker pcl = PopupClicker();
	vector<string> v;
	v.push_back("hi");
	v.push_back("hello");
	v.push_back("world");
	ComboBox cbox = ComboBox(8, v, 2, 2);
	Checklist cgroup = Checklist(3, 8, v, 12, 2);
	RadioList r = RadioList(8, 7, v, 26, 2);
	class popevent : public PopupEvent {
		virtual void PopupClicked(PopupResult res) {
			if (res == OK_CLICKED) {
			}
			else {
			
			}
		}

	};
	popevent pe;
	MessageBox m = MessageBox(6, 34,  &pe);
	m.SetTitle("Popup");
	m.SetText("Are you sure you want to cancel ?");
	m.SetBorder(SINGLE_LINE);
	engine.Run(&m);
	cgroup.SetForeground(RED);
	cgroup.SetBackground(WHITE);
	engine.Run(&r);
	engine.Run(&cgroup);
	engine.Run(&cbox);
	Label tb = Label(10, "Message", 2, 8);

	engine.Run(&tb);
	//engine.showPopup("Are you sure you want to close this application !", &pcl);
	
	
	 
	return 0;
}