#include <iostream>
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Output.H>
#include <FL/Enumerations.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Input.H>

#include "LayoutHelper.h"
#include "Label.h"

using namespace std;
using namespace fluy;


int main() {
	Fl_Double_Window *pW = new Fl_Double_Window (100, 100, 500, 500, "Run fltk_make_gui");
	LayoutHelper lh(pW);
	Label* pLbl = lh.addLabel(0, 0, "Enter your name: ");
	Fl_Input* pI = new Fl_Input(0,0,0,0);
	lh.addInput(pI, 30, 1, 0);

	//Label* pLbl = new Label(0, 0, 100, 25, "This is a Label.");

	/*
	Label* pLbl = new Label(50, 20, 100, 25, "This is a Label."); // @suppress("Un- or ill-initialized variables")
	pLbl->labelfont(FL_HELVETICA_BOLD);
	pLbl->labelcolor(FL_DARK_BLUE);
	*/
	pW->end();
	pW->show();

	return Fl::run();
}
