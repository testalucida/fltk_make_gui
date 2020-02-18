#ifndef LAYOUTHELPER_H_
#define LAYOUTHELPER_H_

#include "constants.h"
#include "internal.h"
#include <FL/Enumerations.H>
#include <vector>

class Fl_Widget;
class Fl_Group;
class Fl_Input;

namespace fluy {

class Label;

typedef unsigned int UINT;



class LayoutHelper {
public:
	LayoutHelper(Fl_Group*);
	virtual ~LayoutHelper();
	Fonts get_settings() const {return _settings;}
	void set_settings(Fonts& settings);
	/**
	 * Adds a group to a group.
	 */
//	void add(Fl_Group*, Fl_Group*,
//			int padN = PAD_N, int padE = PAD_E,
//			int padS = PAD_S, int padW = PAD_W,
//			bool isResizable = true);
//	void add(Fl_Group*, Fl_Group*,
//				int padX, int padY, bool isResizable = true);

	fluy::Label* add_label(UINT col, UINT row, const char* pTxt);
	void add_input(Fl_Input* pI, int chars, UINT col, UINT row);
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
	Fonts _settings;
	int _lbl_dist = 3; //space between label and widget
	//used to calculate input fields width and height:
	const char* _pTeststring = "wegfeiaulm";
	int _len_teststring;
	int _width10chars;
	int _typical_string_height;

};

}//namespace fluy

#endif /* LAYOUTHELPER_H_ */
