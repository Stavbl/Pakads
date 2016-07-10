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
	Panel ch = Panel(22, 40, 2, 2);
	ch.SetBorder(DOUBLE_LINE);
	ch.SetForeground(GREEN);

	Label lbl = Label(14,"Label", 1, 2);
	//lbl.SetBorder(DOUBLE_LINE);
	vector<string> opts;
	opts.push_back("One");
	opts.push_back("Stav BL");
	opts.push_back("Three");
	RadioList rg = RadioList(3, 20, opts);
	//Panel p2 = Panel(15, 4, 12, 6);
	//p2.set_background_color(WHITE);
	//p2.set_border(SINGLE_LINE);
	//p2.set_foreground_color(BLACK);
	NumericBox nb = NumericBox(1, 10, 1, 5);
	//p2.addElement(&nb);
	//ch.addElement(&nb);
	//PopupClicker pcl = PopupClicker();
	vector<string> v;
	v.push_back("hi");
	v.push_back("hello");
	v.push_back("world");
	v.push_back("pizza");
	v.push_back("message");
	ComboBox cbox = ComboBox(12, v, 2, 2);
	Checklist cgroup = Checklist(3, 18, opts);
	cgroup.SetBackground(BLUE);
	cgroup.SetForeground(WHITE);

	TextBox txb = TextBox(25);
	nb.SetBackground(BLUE);
	lbl.SetBackground(RED);
	nb.SetForeground(WHITE);
	nb.SetBorder(SINGLE_LINE);
	/*Button bt = Button(10);
	bt.SetValue("Close App");
	ExitListener el;
	bt.SetBackground(WHITE);
	bt.SetForeground(BLUE);
	bt.SetBorder(DOUBLE_LINE);
	bt.add_listener(&el);*/
	txb.SetBackground(GREEN);
	txb.SetBorder(SINGLE_LINE);
	cbox.SetBackground(PURPLE);
	cbox.SetForeground(WHITE);
	ch.AddControl(&rg, 1, 13);
	ch.AddControl(&cbox, 1, 10);
	ch.AddControl(&nb, 2, 3);
	ch.AddControl(&txb, 2, 7);
	ch.AddControl(&cgroup, 1, 18);



	//.AddControl(&lbl, 40, 2);
	//ch.AddControl()
	ch.AddControl(&lbl, 1,1);
	//ch.AddControl(&cgroup, 2, 15);
	//ch.AddControl(&bt, 50, 23);
	//engine.Run(&nb);
	engine.Run(&ch);/*
	
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
	m.AddControl(&m, 5, 5);
	cgroup.SetForeground(RED);
	cgroup.SetBackground(WHITE);
	Label tb = Label(10, "Message", 2, 8);
	*/
	
	
	 
	return 0;
}