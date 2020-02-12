#include "LayoutHelper.h"
#include <FL/fl_draw.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Input.H>
#include "Label.h"
#include <stdexcept>
#include <string>
#include <cmath>
#include <iostream>

using namespace std;

namespace fluy {

LayoutHelper::LayoutHelper(Fl_Group* pGrp) : _pGrp(pGrp) {
	calculate_stringsize();
}

LayoutHelper::~LayoutHelper() {
	// TODO Auto-generated destructor stub
}

//void LayoutHelper::add(Fl_Group *pGrpParent, Fl_Group *pGrp,
//						int padN, int padE, int padS, int padW,
//						bool isResizable)
//{
//
//}
//
//void LayoutHelper::add(Fl_Group *pGrpParent, Fl_Group *pGrp,
//		 	 	 	 	int padX, int padY,
//						bool isResizable)
//{
//	add(pGrpParent, pGrp, padY, padX, padY, padX, isResizable);
//}

Label* LayoutHelper::addLabel(UINT col, UINT row, char const *pTxt)
{
	ensure_table_size(col, row);
	fl_font(_settings.labelfont, _settings.labelsize);
	Size size = get_size(pTxt);
	size.w += (_paddings.w + _paddings.e);
	size.h += (_paddings.n + _paddings.s);

	Label* pLbl = new Label(get_x(col), get_y(row), size.w, size.h, pTxt);
	//Label* pLbl = new Label(50, 20, size.w, size.h, pTxt);
	pLbl->labelcolor(_settings.labelcolor);
	pLbl->labelfont(_settings.labelfont);
	pLbl->labelsize(_settings.labelsize);
	set(pLbl, col, row);
	make_group_fit();
	return pLbl;
}

void LayoutHelper::addInput(Fl_Input *pI, int chars, UINT col, UINT row) {
	ensure_table_size(col, row);
	Size size = get_size(chars);
	size.w += (_paddings.w + _paddings.e);
	size.h += (_paddings.n + _paddings.s);
	((Fl_Widget*)pI)->position(get_x(col), get_y(row));
	pI->size(size.w, size.h);
	set(pI, col, row);
}



Size LayoutHelper::get_size(char const *pTxt) const {
	Size size;
	fl_measure(pTxt, size.w, size.h);
	return size;
}

Size LayoutHelper::get_size(int chars) const {
	float f = chars / _len_teststring;
	int w = int(ceil(f) * _width10chars);
	Size sz {w, _typical_string_height};
	return sz;
}

void LayoutHelper::ensure_table_size(UINT col, UINT row) {
	//enough columns?
	if (_widgetTable.size() <= col) {
		for (UINT c = _widgetTable.size(); c <= col; c++) {
			WidgetVector* pWv = new WidgetVector;
			_widgetTable.push_back(pWv);
		}
	}
	//enough rows?
	WidgetVector* pWv = _widgetTable.at(col);
	for (UINT r = 0; r <= row; r++) {
		pWv->push_back((Widget*) NULL);
	}
}

int LayoutHelper::get_x(UINT col) const {
	int x = _margins.w;
	if (_widgetTable.size() <= col) {
		string msg = "LayoutHelper::get_x(): WidgetTable has no column ";
		msg.append(to_string(col));
		throw range_error(msg);
	}

	if (col == 0) {
		return x;
	}

	//do we have a widget in this column?
	//If so, return its x().
	WidgetVector* pWv = _widgetTable[col];
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
		return pW->x() + pW->w() + _margins.e + x;
	}

	return x;

}

int LayoutHelper::get_y(UINT row) const {
	int y = _margins.n;
	if (row == 0) {
		return y;
	}

	return y;
}

int LayoutHelper::get_widgets_in_row(int row, WidgetVector *pWv) {
	return 0;
}

void LayoutHelper::set_settings(Font_Settings &settings) {
	_settings.labelcolor = settings.labelcolor;
	_settings.labelfont = settings.labelfont;
	_settings.labelsize = settings.labelsize;
	_settings.textcolor = settings.textcolor;
	_settings.textfont = settings.textfont;
	_settings.textsize = settings.textsize;
	calculate_stringsize();
}

void LayoutHelper::set(Fl_Widget* pW, UINT col, UINT row) {
	WidgetVector* pWv = _widgetTable[col];
	Widget* p = new Widget;
	p->pWidget = pW;
	pWv->at(row) = p;
}

void LayoutHelper::calculate_stringsize() {
	_len_teststring = strlen(_pTeststring);
	fl_font(_settings.textfont, _settings.textsize);
	Size size = get_size(_pTeststring);
	_width10chars = size.w;
	_typical_string_height = size.h;
}

Fl_Widget* LayoutHelper::get_broadest_widget(int col) const {
	Fl_Widget* pW = NULL;
	int maxx = 0;
	WidgetVector* pWv = _widgetTable[col];
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

void LayoutHelper::make_group_fit() const {
	int hmax = 0;
	for (int c = 0, cmax = _widgetTable.size(); c < cmax; c++) {
		int h = get_column_height(c);
		hmax = (h > hmax) ? h : hmax;
	}

	Fl_Widget* pW = get_broadest_widget(_widgetTable.size() - 1);
	int w = pW->x() + pW->w() + _margins.e;
	_pGrp->size(w, hmax);
}

int LayoutHelper::get_column_height(int col) const {
	int h = 0;
	Fl_Widget* pW = get_bottom_widget(col);
	if (pW) {
		h = pW->y() + pW->h() + _margins.s;
	}
	return h;
}

int LayoutHelper::get_row_width(int row) const {
	throw runtime_error("get_row_width: not yet implemented");
}

Fl_Widget* LayoutHelper::get_bottom_widget(int col) const {
	WidgetVector *pCol = _widgetTable[col];
	WidgetVector::reverse_iterator rit = pCol->rbegin();
	for (; rit != pCol->rend(); ++rit) {
		Widget *pW = *rit;
		if (pW && pW->pWidget) {
			return pW->pWidget;
		}
	}
	return NULL;
}

} //namespace fluy
