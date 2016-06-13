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

struct ExitListener : public MouseListener {
	void MousePressed(IControl &element, int x, int y, bool is_left) {
		exit(1);
	}
};


int main() {


	EventEngine engine;
	Panel ch = Panel(25, 80, 5, 3);
	ch.SetBorder(DOUBLE_LINE);
	ch.SetForeground(GREEN);

	Label lbl = Label(8,"Label");
	vector<string> opts;
	opts.push_back("One");
	opts.push_back("Stav Blyman");
	opts.push_back("Three");
	RadioList rg = RadioList(3, 20, opts);
	//Panel p2 = Panel(15, 4, 12, 6);
	//p2.set_background_color(WHITE);
	//p2.set_border(SINGLE_LINE);
	//p2.set_foreground_color(BLACK);
	NumericBox nb = NumericBox(1, 10, 1, 99);
	//p2.addElement(&nb);
	//ch.addElement(&nb);
	//PopupClicker pcl = PopupClicker();
	vector<string> v;
	v.push_back("hi");
	v.push_back("hello");
	v.push_back("world");
	ComboBox cbox = ComboBox(8, v, 2, 2);
	Checklist cgroup = Checklist(3, 8, v, 12, 2);
	RadioList r = RadioList(8, 7, v, 26, 2);
	TextBox txb = TextBox(30);

	Button bt = Button(10);
	bt.SetValue("Close App");
	ExitListener el;
	bt.SetBackground(WHITE);
	bt.SetForeground(BLUE);
	bt.SetBorder(DOUBLE_LINE);
	bt.add_listener(&el);
	txb.SetBackground(GREEN);
	ch.AddControl(&nb, 2, 2);
	ch.AddControl(&lbl, 20, 2);
	ch.AddControl(&rg, 30, 15);
	ch.AddControl(&cbox, 2, 4);
	ch.AddControl(&cgroup, 2, 15);
	ch.AddControl(&r, 30, 2);
	ch.AddControl(&txb, 6, 12);
	ch.AddControl(&bt, 50, 23);

	
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

	engine.Run(&ch);
	cgroup.SetForeground(RED);
	cgroup.SetBackground(WHITE);
	//engine.Run(&r);
	//engine.Run(&cgroup);
	//engine.Run(&cbox);
	Label tb = Label(10, "Message", 2, 8);

	//engine.Run(&tb);
	//engine.showPopup("Are you sure you want to close this application !", &pcl);
	
	
	 
	return 0;
}