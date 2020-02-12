#ifndef LAYOUTHELPER_H_
#define LAYOUTHELPER_H_

#include "constants.h"
#include <FL/Enumerations.H>
#include <vector>

class Fl_Widget;
class Fl_Group;
class Fl_Input;

namespace fluy {

class Label;

typedef unsigned int UINT;

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
	int e = PAD_E;
	int s = PAD_S;
	int w = PAD_W;
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

struct Font_Settings {
	Fl_Font labelfont = 1; //FL_HELVETICA_BOLD
	Fl_Fontsize labelsize = 14;
	Fl_Color labelcolor = FL_DARK_BLUE;
	Fl_Font textfont = 0; //FL_HELVETICA
	Fl_Fontsize textsize = 14;
	Fl_Color textcolor = FL_BLACK;
};

struct Size {
	int w = 0;
	int h = 0;
};

class LayoutHelper {
public:
	LayoutHelper(Fl_Group*);
	virtual ~LayoutHelper();
	Font_Settings get_settings() const {return _settings;}
	void set_settings(Font_Settings& settings);
	/**
	 * Adds a group to a group.
	 */
//	void add(Fl_Group*, Fl_Group*,
//			int padN = PAD_N, int padE = PAD_E,
//			int padS = PAD_S, int padW = PAD_W,
//			bool isResizable = true);
//	void add(Fl_Group*, Fl_Group*,
//				int padX, int padY, bool isResizable = true);

	fluy::Label* addLabel(UINT col, UINT row, const char* pTxt);
	void addInput(Fl_Input* pI, int chars, UINT col, UINT row);
	void add(Fl_Group*, Fl_Widget*, int col, int row);
	Size get_size(const char* pTxt) const;
	Size get_size(int chars) const;

private:
	void ensure_table_size(UINT col, UINT row);
	void set(Fl_Widget*, UINT col, UINT row);
	int get_x(UINT col) const;
	int get_y(UINT row) const;
	int get_widgets_in_row(int row, WidgetVector* pWv);
	Fl_Widget* get_broadest_widget(int col) const;
	void calculate_stringsize();
	void make_group_fit() const;
	int get_column_height(int col) const;
	int get_row_width(int row) const;
	Fl_Widget* get_bottom_widget(int col) const;
private:
	Fl_Group* _pGrp;
	WidgetTable _widgetTable;
	Paddings _paddings;
	Margins _margins;
	Font_Settings _settings;
	int _lbl_dist = 3; //space between label and widget
	//used to calculate input fields width and height:
	const char* _pTeststring = "wegfeiaulm";
	int _len_teststring;
	int _width10chars;
	int _typical_string_height;

};

}//namespace fluy

#endif /* LAYOUTHELPER_H_ */
