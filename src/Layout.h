/*
 * Layout.h
 *
 *  Created on: 14.02.2020
 *      Author: martin
 */

#ifndef LAYOUT_H_
#define LAYOUT_H_

#include "internal.h"
#include "WidgetDef.h"
#include "Label.h"
#include "LayoutTree.h"

class Fl_Widget;
class Fl_Group;
class Fl_Input;
class Fl_Button;

namespace fluy {

/**
 * Layout help creating a Fl_Group with all its contents.
 * By instantiating a Layout object with the hierarchical topmost group
 * (typically a Group::DOUBLE_WINDOW) a tree will be build
 * by group_begin(), add(...), group_end().
 * By finally calling build_layout() all groups will be calculated and
 * instantiated.
 */
class Layout {
public:
	Layout();
	virtual ~Layout();
	/**
	 * Starts a group of the given kind (Group::GROUP, Group::TILE,...)
	 * Widgets add()'ed will be placed in that group.
	 * Call group_end after having add()'ed the desired widgets.
	 * Please note that counting cols and rows starts with 0 for each group.
	 * Returns the settings of the group.
	 */
	Settings& group_begin(GroupType, bool resizable,
			        int col, int row,
			        int colspan = 1, int rowspan = 1);
	/**
	 * Adds a widget with "built in" label like Button or
	 * widgets with its label to its right like
	 * Round_Button, Check_Button.
	 * Widgets may be initialized with positon and size set to 0.
	 */
	Settings& add(Fl_Widget*, int col, int row,
			      int colspan = 1, int rowspan = 1);
	/**
	 * Adds a widget with base class Fl_Input.
	 * If Fl_Input* is Fl_Input (or one of its derivates) or Fl_Output
	 * then n_chars_high must be 1.
	 * If it is about the multiline equivalents the widget's height will
	 * be calculated based on n_chars_high and the set font.
	 */
	Settings& add_in_out(Fl_Input*, int n_chars_wide, int n_chars_high,
			             int col, int row, int colspan = 1, int rowspan = 1);

	/**
	 * Adds a button with its label to its right.
	 * "Normal" buttons have to be added by using add(Fl_Widget*,...)
	 */
	Settings& add_check_radio_button(Fl_Button*, int col, int row,
			                         int colspan = 1, int rowspan = 1);
	/**
	 * Adds a label.
	 * This is for input- and output- widgets.
	 * Instead of creating those widgets by passing their label
	 * (e.g. Fl_Input(1, 2, 50, 25, "Enter your name:")) you create them
	 * without label and make a separate call to add(const char* pLbl).
	 */
	Settings& add(const char* pLbl, int col, int row,
			      int colspan = 1, int rowspan = 1);
	/**
	 * Indicates that adding widgets to a group is terminated.
	 */
	Fl_Group* group_end();
	/**
	 * Indicates that adding widgets to this layout is terminated.
	 * Returns a pointer to the hierarchical topmost group (typically a
	 * Fl_Double_Window).
	 */
	//Fl_Group* layout_end();
private:
	Settings& add_widgetdef(WidgetDef*, int col, int row);
private:
	/**
	 * typically the groups and widgets of the Fl_Double_Window
	 */
	GroupDef* _pGroupdef = NULL;
	/**
	 * Each call to group_begin() results in creating a
	 * GroupDef object and storing it in a vector.
	 * Each call to group_end() results in removing it
	 * from that vector:
	 */
	std::vector<GroupDef*> _groups_in_process;
	Paddings _paddings;
	Margins _margins;
	Fonts _settings;
};

} //namespace fluy

#endif /* LAYOUT_H_ */
