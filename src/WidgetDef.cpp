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
	children.add(pWiDef, col, row);
}

const WidgetDefVector& GroupDef::get_children(int col) const {
	WidgetDefVector* pCol = children.get(col);
	return *pCol;
}

bool GroupDef::has_grouptype_children() const {
	return children.has_grouptype_children();
}

int GroupDef::get_x(int col) const {
	int x = settings.margins.w;
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
	WidgetDefVector* pWv = children.get(col);
	if (pWv->size() > 0) {
		for (auto w : *pWv) {
			if (w and w->pWidget) {
				return w->pWidget->x();
			}
		}
	}

	//no widgets yet in this column.
	//search for the broadest widget in the column to the left.
	//it's decisive for calculating the x value of this column.
	Fl_Widget* pW = get_broadest_widget(col-1);
	if (pW) {
		return pW->x() + pW->w() + settings.margins.e + x;
	}

	return x;
}

int GroupDef::get_y(int row) const {
	int y = settings.margins.n;
	if (row == 0) {
		return y;
	}

	return y;
}

Fl_Widget* GroupDef::get_broadest_widget(int col) const {
	Fl_Widget* pW = NULL;
	int maxx = 0;
	WidgetDefVector* pWv = children.get(col);
	if (pWv->size() > 0) {
		for (auto w : *pWv) {
			Fl_Widget* p = w->pWidget;
			if (w and p) {
				if (!pW) {
					pW = p;
					maxx = p->x() + p->w();
				} else {
					int right = p->x() + p->w();
					if (right > maxx) {
						maxx = right;
						pW = p;
					}
				}
			}
		}
	}
	return pW;
}

Fl_Group* GroupDef::create_group() {
	//Precondition: there's no groupdef child in this group.
	//
	//Iterate over all widgetDefs, calculate each Fl_Widget's position
	//and size and create a Fl_Group objects according to
	//the Fl_Widget*s positions and sizes.
	//Finally add the Fl_Widget*s to the created Fl_Group.
	if (has_grouptype_children()) {
		throw runtime_error("GroupDef::create_group: no group child allowed.");
	}

	for (int c = 0, cmax = columns(); c < cmax; c++) {
		int x = get_x(c);
		for (int r = 0, rmax = rows(c); r < rmax; r++) {
			WidgetDef& widef = children.get(c, r);
			Fl_Widget* pW = widef.pWidget;
			int y = get_y(r);
			//set position:
			pW->position(x, y);
			set_size_and_font(widef);
		}
	}
	//calculate group's position:

	//calculate group's size:

	//create Fl_Group:

	return NULL;
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

WidgetDef& WidgetDefTable::get(int col, int row) const {
	WidgetDef* p = (*(_table[col]))[row];
	return *p;
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

