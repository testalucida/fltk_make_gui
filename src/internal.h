/*
 * internal.h
 *
 *  Created on: 14.02.2020
 *      Author: martin
 */

#ifndef INTERNAL_H_
#define INTERNAL_H_

#include "constants.h"
#include <FL/Enumerations.H>
#include <vector>

class Fl_Widget;

namespace fluy {

struct Widget {
	Fl_Widget* pWidget = NULL;

};

typedef std::vector<Widget*> WidgetVector;
typedef std::vector<WidgetVector*> WidgetTable;

/**
 * Paddings define the space between content and border
 */
struct Paddings {
	int n = PAD_N;
	//int e = PAD_E;
	int s = PAD_S;
	//int w = PAD_W;
};

/**
 * Margins define the space outside the border
 */
struct Margins {
	int n = MARGIN_N;
	int e = MARGIN_E;
	int s = MARGIN_S;
	int w = MARGIN_W;
};

struct Fonts {
	Fl_Font labelfont = 1; //FL_HELVETICA_BOLD
	Fl_Fontsize labelsize = 14;
	Fl_Color labelcolor = FL_DARK_BLUE;
	Fl_Font textfont = 0; //FL_HELVETICA
	Fl_Fontsize textsize = 14;
	Fl_Color textcolor = FL_BLACK;
};

struct Settings {
	Margins margins;
	Fonts fonts;
};

struct Size {
	int w = 0;
	int h = 0;
};

} //namespace fluy

#endif /* INTERNAL_H_ */
