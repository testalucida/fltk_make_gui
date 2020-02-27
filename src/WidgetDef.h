/*
 * Group.h
 *
 *  Created on: 14.02.2020
 *      Author: martin
 */

#ifndef WIDGETDEF_H_
#define WIDGETDEF_H_

#include "internal.h"
#include <vector>

class Fl_Widget;
class Fl_Group;
class Fl_Input;

namespace fluy {

enum GroupType {
	GROUP,
	DOUBLE_WINDOW,
	TILE,
	TAB,
	SCROLL,
	UNK
};

enum WidgetType {
	ANY_GROUP,
	SINGLE_IN_OUT,
	MULTI_IN_OUT,
	LABEL,
	BUTTON_WITH_RIGHTSIDE_TEXT,
	OTHER
};

/////////////////////////////////////////////////////////////////
class TextMeasure {
public:
	~TextMeasure() {}
	static TextMeasure& inst();
	Size get_size(char const *pTxt, Fl_Font, Fl_Fontsize) const;
	Size get_size(int chars, Fl_Font, Fl_Fontsize) const;
private:
	TextMeasure();
	const char* _pTeststring = "wegfeiaulm";
	int _len_teststring = 0;
	static TextMeasure* _pThis;
};
/////////////////////////////////////////////////////////////////

class WidgetDef {
public:
	WidgetDef(Fl_Widget*, int colspan=1, int rowspan=1);
	virtual ~WidgetDef() {}
	void set_label_attributes();
public:
	//static int id;
	WidgetDef* pParent = NULL;
	//char type = 'W';
	WidgetType type = WidgetType::OTHER;
	Fl_Widget* pWidget = NULL;
	int colspan = 1;
	int rowspan = 1;
	Settings settings;
};
/////////////////////////////////////////////////////////////////

class InOutWidgetDef : public WidgetDef {
public:
	InOutWidgetDef(Fl_Input*, int n_chars_wide, int n_chars_high,
			       int colspan = 1, int rowspan = 1);

	int n_chars_wide = 0;
	int n_chars_high = 0;
};
/////////////////////////////////////////////////////////////////

typedef std::vector<WidgetDef*> WidgetDefVector;
//typedef std::vector<WidgetDefVector*> WidgetDefTable;

/////////////////////////////////////////////////////////////////
/**
 * WidgetDefTable represents all columns and rows of a Fl_Group.
 * The table is described by a vector containing columns of WidgetDef objects.
 * A column containing WidgetDef objects is realized by typedef WidgetDefVector:
 * a vector containing WidgetDef (GroupDef) objects.
 */
class WidgetDefTable {
public:
	WidgetDefTable();
	~WidgetDefTable();
	void add(WidgetDef*, int col, int row);
	//WidgetDef& add(Fl_Widget*, int col, int row, int colspan=1, int rowspan=1);
	WidgetDef* get(int col, int row) const;
	int columns() const {return _table.size();}
	int rows() const;
	WidgetDefVector* get(int col) const {return _table[col];}
	void get(int row, WidgetDefVector*) const;
	bool has_grouptype_children() const;
	//WidgetDef* get(int col, int row) const;
public:
	Paddings _paddings;
	Margins _margins;
	Fonts _settings;
private:
	void ensure_table_size(int col, int row);
private:
	std::vector<WidgetDefVector*> _table;
};
/////////////////////////////////////////////////////////////////

/**
 * GroupDef represents a Fl_Group object with its widgets and groups.
 * It owns a vector containing children widges and, by inheriting WidgetDef,
 * it has a link to the parent.
 */
class GroupDef : public WidgetDef {
public:
	GroupDef(GroupType type = GroupType::UNK, bool resizable = true,
			 int colspan=1, int rowspan=1);
	~GroupDef();
	void add(WidgetDef*, int col, int row);
	int columns() const {return children.columns();}
	int rows() const {return children.rows();}
	const WidgetDefVector& get_children(int col) const;
	bool has_grouptype_children() const;
	int get_x(int col) const;
	/**
	 * Provides the y value for the given row.
	 * Note:
	 * margins.s of row-1 is included, margins.n of row is not.
	 */
	int get_y(int row) const;
	WidgetDef* get_broadest_widget(int col) const;
	WidgetDef* get_highest_widget(int row) const;
	Fl_Group* create_group();
	void set_widgets_position();
private:
	void set_size_and_font(WidgetDef&);
public:
	GroupType grouptype = GroupType::UNK;
	bool resizable = false;
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
	WidgetDefTable children;
};
/////////////////////////////////////////////////////////////////

} //namespace fluy

#endif /* WIDGETDEF_H_ */
