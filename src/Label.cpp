#include "Label.h"
#include <FL/fl_draw.H>
#include <FL/Enumerations.H>
#include <iostream>

using namespace std;

namespace fluy {

Label::Label(int x, int y, int w, int h, const char* pTxt)
: Fl_Widget(x, y, w, h, pTxt)
{
	box(FL_FLAT_BOX);
}

Label::~Label() {
}

void Label::resize(int x, int y, int w, int h) {
//	Fl_Widget::resize(x, y, this->w(), this->h());
	Fl_Widget::resize(x, y, w, h);
}

void Label::draw() {
	Fl_Color colr = fl_color();
	Fl_Font font = fl_font();
	Fl_Fontsize size = fl_size();

	fl_color(labelcolor());
	fl_font(labelfont(), labelsize());

	fl_draw(label(), x(), y(), w(), h(), FL_ALIGN_LEFT, 0, 0);
	fl_color(colr);
	fl_font(font, size);
}

} //namespace fluy
