#include <iostream>
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Output.H>
#include <FL/Enumerations.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Input.H>
#include "Layout.h"
#include "LayoutHelper.h"
#include "Label.h"

using namespace std;
using namespace fluy;

int main() {
	Layout lo;
	lo.group_begin(GroupType::GROUP, true, 0, 0);
	Fl_Input* pI = new Fl_Input(0,0,0,0);
	lo.add("Enter your name: ", 0, 0);
	lo.add_in_out(pI, 30, 1, 1, 0);
	lo.group_end();
	Fl_Double_Window* pWin = (Fl_Double_Window*)lo.layout_end();

	pWin->show();
	return Fl::run();
}

int main_() {
	Fl_Double_Window *pW = new Fl_Double_Window (100, 100, 500, 500, "Run fltk_make_gui");
	LayoutHelper lh(pW);
	Label* pLbl = lh.add_label(0, 0, "Enter your name:");
	Fl_Input* pI = new Fl_Input(0,0,0,0);
	lh.add_input(pI, 30, 1, 0);
	lh.add_label(2, 0, "And here goes your christian name:");
	pI = new Fl_Input(0,0,0,0);
	lh.add_input(pI, 20, 3, 0);

	pW->end();
	pW->resizable(pW);
	pW->show();

	return Fl::run();
}
