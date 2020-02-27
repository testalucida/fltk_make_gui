#include <iostream>
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Output.H>
#include <FL/Enumerations.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Input.H>
#include "Layout.h"
#include "Label.h"

using namespace std;
using namespace fluy;

int main() {
	Layout lo;
	lo.group_begin(GroupType::DOUBLE_WINDOW, false, 0, 0);
	lo.add_label("Enter your name: ", 0, 0);
	Fl_Input* pI = new Fl_Input(0,0,0,0);
	lo.add_in_out(pI, 30, 1, 1, 0);
	lo.add_label("And your address goes here: ", 0, 1);
	pI = new Fl_Input(0,0,0,0);
	lo.add_in_out(pI, 40, 1, 1, 1);
	Fl_Double_Window* pWin = (Fl_Double_Window*)lo.group_end();

	pWin->show();
	return Fl::run();
}

