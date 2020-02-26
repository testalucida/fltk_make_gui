/*
 * WidgetDef.cpp
 *
 *  Created on: 15.02.2020
 *      Author: martin
 */
#include "WidgetDef.h"
#include <FL/fl_draw.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Tile.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Scroll.H>
#include "Label.h"
#include <string>
#include <cmath>
#include <stdexcept>

using namespace std;

namespace fluy {

///////////////////////////////////////////////////////////////////////////////
///////////////////////    TextMeasure   //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TextMeasure* TextMeasure::_pThis = NULL;

TextMeasure::TextMeasure() {
	_len_teststring = strlen(_pTeststring);
}

TextMeasure& TextMeasure::inst() {
	if (!_pThis) {
		_pThis = new TextMeasure();
	}
	return *_pThis;
}

Size TextMeasure::get_size(char const *pTxt,
		                   Fl_Font font, Fl_Fontsize fontsize) const
{
	fl_font(font, fontsize);
	Size size;
	fl_measure(pTxt, size.w, size.h);
	return size;
}

Size TextMeasure::get_size(int chars,
						   Fl_Font font, Fl_Fontsize fontsize) const
{
	float f = chars / _len_teststring;
	Size teststring_size = get_size(_pTeststring, font, fontsize);
	int w = int(ceil(f) * teststring_size.w);
	Size sz {w, teststring_size.h};
	return sz;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////    WidgetDef     //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

WidgetDef::WidgetDef(Fl_Widget* pWidget, int colspan, int rowspan) {
	this->pWidget = pWidget;
	this->colspan = colspan;
	this->rowspan = rowspan;
	set_label_attributes();
}

void WidgetDef::set_label_attributes() {
	if (pWidget) {
		pWidget->labelfont(settings.fonts.labelfont);
		pWidget->labelsize(settings.fonts.labelsize);
		pWidget->labelcolor(settings.fonts.labelcolor);
	}
}

///////////////////////////////////////////////////////////////////////////////
/////////////////////   InOutWidgetDef     ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

InOutWidgetDef::InOutWidgetDef(Fl_Input* p, int n_chars_wide, int n_chars_high,
	       	   	   	   	   	   int colspan, int rowspan)
: WidgetDef((Fl_Widget*)p, colspan, rowspan)
{
	this->n_chars_wide = n_chars_wide;
	this->n_chars_high = n_chars_high;
	type = (n_chars_high == 1) ? WidgetType::SINGLE_IN_OUT :
							     WidgetType::MULTI_IN_OUT;
	set_label_attributes();
	TextMeasure& tm = TextMeasure::inst();
	Fl_Font font = settings.fonts.textfont;
	Fl_Fontsize fontsize = settings.fonts.textsize;
	Size size = tm.get_size(n_chars_wide, font, fontsize);
	p->size(size.w, size.h + PAD_N + PAD_S);
	p->textfont(font);
	p->textsize(fontsize);
	p->textcolor(settings.fonts.textcolor);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////   GroupDef   //////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

GroupDef::GroupDef(GroupType groupType, bool resizable,
		           int colspan, int rowspan)
: WidgetDef((Fl_Widget*)NULL, colspan, rowspan)
{
	type = WidgetType::ANY_GROUP;
	grouptype = groupType;
	this->resizable = resizable;
}

GroupDef::~GroupDef() {
	//TODO: delete all children
}


void GroupDef::add(WidgetDef* pWiDef, int col, int row) {
	//put it in the children widget table:
	children.add(pWiDef, col, row);
	//set position of widget:
	Fl_Widget* p = pWiDef->pWidget;
	int x = get_x(col) + pWiDef->settings.margins.w;
	int y = get_y(row) + pWiDef->settings.margins.n;
	p->position(x, y);
}

const WidgetDefVector& GroupDef::get_children(int col) const {
	WidgetDefVector* pCol = children.get(col);
	return *pCol;
}

bool GroupDef::has_grouptype_children() const {
	return children.has_grouptype_children();
}

int GroupDef::get_x(int col) const {
	int x = this->x;
	if (columns() <= col) {
		string msg = "GroupDef::get_x(): Group has no column ";
		msg.append(to_string(col));
		throw range_error(msg);
	}

	if (col == 0) {
		return x;
	}

	//do we have a widget in this column?
	//If so, return its x().
//	WidgetDefVector* pWv = children.get(col);
//	if (pWv->size() > 0) {
//		for (auto w : *pWv) {
//			if (w && w->pWidget) {
//				return w->pWidget->x();
//			}
//		}
//	}

	//no widgets yet in this column.
	//search for the broadest widget in the column to the left.
	//it's decisive for calculating the x value of this column.
	WidgetDef* pWiDef = get_broadest_widget(col-1);
	if (pWiDef) {
		Fl_Widget* pW = pWiDef->pWidget;
		if (pW) {
			return pW->x() + pW->w() + pWiDef->settings.margins.e;
		}
	}

	return x;
}

int GroupDef::get_y(int row) const {
	if (rows() <= row) {
		string msg = "GroupDef::get_y(): Group has no row ";
		msg.append(to_string(row));
		throw range_error(msg);
	}

	int y = this->y;

	for(int r = row-1; r > -1; r--) {
		WidgetDef* pWiDef = get_highest_widget(r);
		if (pWiDef) {
			Fl_Widget* p = pWiDef->pWidget;
			y += (p->y() + p->h() + pWiDef->settings.margins.s);
			break;
		}
	}

	return y;
}

WidgetDef* GroupDef::get_broadest_widget(int col) const {
	WidgetDef* pWiDef = NULL;
	int maxx = 0;
	WidgetDefVector* pCol = children.get(col);
	if (pCol->size() > 0) {
		for (auto wd : *pCol) {
			Fl_Widget* p = wd->pWidget;
			if (wd and p) {
				if (!pWiDef) {
					pWiDef = wd;
					maxx = p->x() + p->w();
				} else {
					int right = p->x() + p->w();
					if (right > maxx) {
						maxx = right;
						pWiDef = wd;
					}
				}
			}
		}
	}
	return pWiDef;
}

WidgetDef* GroupDef::get_highest_widget(int row) const {
	WidgetDefVector col;
	children.get(row, &col);
	int H = 0;
	WidgetDef* pWiDef = NULL;
	for (auto wd : col) {
		Fl_Widget* p = wd->pWidget;
		int y_bot = p->y() + p->h() + wd->settings.margins.s;
		if (y_bot > H) {
			H = y_bot;
			pWiDef = wd;
		}
	}
	return pWiDef;
}

Fl_Group* GroupDef::create_group() {
	//Get width of group which is defined by the broadest widget
	//in the mostright column.
	//Get height of group which is defined by the hihgest widget
	//in the bottommost row.
	//Instantiate a Fl_Group object accordingly.
	//Add all Fl_Widget objects to the new Fl_Group.
	//Make group resizable as appropriate.

	if (columns() == 0 || rows() == 0) {
		return new Fl_Group(this->x, this->y, 0, 0);
	}

	int grp_w = 0, grp_h = 0;
	//get broadest widget of the last column:
	WidgetDef* pWiDef = get_broadest_widget(columns()-1);
	if (pWiDef) {
		Fl_Widget* p = pWiDef->pWidget;
		if (p) {
			grp_w = p->x() + p->w() + pWiDef->settings.margins.e;
		}
	}
	//get highest widget of the last row:
	pWiDef = get_highest_widget(rows()-1);
	if (pWiDef) {
		Fl_Widget* p = pWiDef->pWidget;
		if (p) {
			grp_h = p->y() + p->h() + pWiDef->settings.margins.s;
		}
	}

	//create Fl_Group:
	Fl_Group* pGrp = NULL;
	switch (this->grouptype) {
	case GroupType::DOUBLE_WINDOW:
		pGrp = new Fl_Double_Window(this->x, this->y, grp_w, grp_h);
		break;
	case GroupType::GROUP:
		pGrp = new Fl_Group(this->x, this->y, grp_w, grp_h);
		break;
	case GroupType::SCROLL:
		pGrp = new Fl_Scroll(this->x, this->y, grp_w, grp_h);
		break;
	case GroupType::TAB:
		pGrp = new Fl_Tabs(this->x, this->y, grp_w, grp_h);
		break;
	case GroupType::TILE:
		pGrp = new Fl_Tile(this->x, this->y, grp_w, grp_h);
		break;
	default:
		throw runtime_error("GroupDef::create_group: unknown GroupType");
	}

	//add widgets:
	pGrp->end();
	for (int c = 0, cmax = columns(); c < cmax; c++) {
		WidgetDefVector* pCol = children.get(c);
		for(auto wd : *pCol) {
			pGrp->add(wd->pWidget);
		}
	}

	if (resizable) {
		pGrp->resizable(pGrp);
	}

	return pGrp;
}

void GroupDef::set_size_and_font(WidgetDef& widef) {
	Fl_Widget* pW = widef.pWidget;
	TextMeasure& tm = TextMeasure::inst();
	Fonts fonts = widef.settings.fonts;
	switch(widef.type) {
	case WidgetType::LABEL:
	{
		Size size = tm.get_size(pW->label(), fonts.labelfont, fonts.labelsize);
		pW->size(size.w, size.h);
	}
	break;
	case WidgetType::SINGLE_IN_OUT:
	case WidgetType::MULTI_IN_OUT:
	{
		InOutWidgetDef& inout = (InOutWidgetDef&)widef;
		Size size =
			tm.get_size(inout.n_chars_wide, fonts.textfont, fonts.textsize);
		size.h = size.h * inout.n_chars_high;
		pW->size(size.w, size.h);
		Fl_Input* pI = (Fl_Input*)pW;
		pI->textfont(fonts.textfont);
		pI->textsize(fonts.textsize);
		pI->textcolor(fonts.textcolor);
	}
	break;
	case WidgetType::BUTTON_WITH_RIGHTSIDE_TEXT:
	break;
	case WidgetType::OTHER:
	break;
	default:
		throw runtime_error("GroupDef::get_size(): unknown WidgetType");
	} //switch
	pW->labelfont(fonts.labelfont);
	pW->labelsize(fonts.labelsize);
	pW->labelcolor(fonts.labelcolor);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////  WidgetDefTable  //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

WidgetDefTable::WidgetDefTable() {
}

WidgetDefTable::~WidgetDefTable() {
}

void WidgetDefTable::ensure_table_size(int col, int row) {
	//enough columns?
	if ((int)_table.size() <= col) {
		for (int c = _table.size(); c <= col; c++) {
			WidgetDefVector* pWv = new WidgetDefVector;
			_table.push_back(pWv);
		}
	}
	//enough rows?
	WidgetDefVector* pWv = _table.at(col);
	for (int r = 0; r <= row; r++) {
		pWv->push_back((WidgetDef*) NULL);
	}
}

void WidgetDefTable::add(WidgetDef *pWiDef, int col, int row) {
	ensure_table_size(col, row);
	WidgetDefVector* pCol = _table[col];
	(*pCol)[row] = pWiDef;
}

int WidgetDefTable::rows() const {
	if (_table.size() == 0) {
		return 0;
	}
	WidgetDefVector* pWiVec = _table[0];
	return pWiVec->size();
}

WidgetDef* WidgetDefTable::get(int col, int row) const {
	WidgetDef* p = (*(_table[col]))[row];
	return p;
}

void WidgetDefTable::get(int row, WidgetDefVector* pRowWidgets) const {
	for (auto wvec : _table) {
		pRowWidgets->push_back(wvec->at(row));
	}
}

bool WidgetDefTable::has_grouptype_children() const {
	for (auto wicol : _table) {
		for (auto widef : *wicol) {
			if (widef->type == WidgetType::ANY_GROUP) {
				return true;
			}
		}
	}
	return false;
}


} //namespace fluy

