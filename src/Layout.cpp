/*
 * Layout.cpp
 *
 *  Created on: 14.02.2020
 *      Author: martin
 */

#include "Layout.h"
#include <FL/fl_draw.H>
#include <FL/Fl_Group.H>

#include <cmath>
#include <stdexcept>

using namespace std;

namespace fluy {

Layout::Layout() {

}

Layout::~Layout() {
	// TODO Auto-generated destructor stub
}

Settings& Layout::group_begin(GroupType type, bool resizable, int col, int row,
		                 int colspan, int rowspan )
{
	GroupDef* pGrpDef = new GroupDef(type, true, colspan, rowspan);
	pGrpDef->resizable = resizable;
	pGrpDef->colspan = colspan;
	pGrpDef->rowspan = rowspan;
	if (!_pGroupdef) {
		_pGroupdef = pGrpDef;
	} else {
		GroupDef* pParent = _groups_in_process.back();
		pGrpDef->pParent = pParent;
		//add the new GroupDef object to its parent's children table:
		pParent->add(pGrpDef, col, row);
	}

	//add the new GroupDef object to groups_in_process:
	_groups_in_process.push_back(pGrpDef);

	return pGrpDef->settings;
}

Fl_Group* Layout::group_end() {
	/**
	 * Get the processed group.
	 * Get its WidgetDef*s and calculate their sizes and positions.
	 * Instantiate the Fl_Group object and add the Fl_Widget objects.
	 */
	GroupDef* pGrpDef = _groups_in_process.back();
	Fl_Group* pGrp = pGrpDef->create_group();

	//remove group from vector, it's not in process any more
	_groups_in_process.pop_back();
	return pGrp;
}

//Fl_Group* Layout::layout_end() {
//	return group_end();
//}

Settings& Layout::add(const char *pTxt, int col, int row,
		         int colspan, int rowspan)
{
	//create WidgetDef for the label:
	Label* pLabel = new Label(0, 0, 0, 0, pTxt);
	WidgetDef* pwd = new WidgetDef(pLabel);
	pwd->type = WidgetType::LABEL;
	return add_widgetdef(pwd, col, row);

}

Settings& Layout::add(Fl_Widget* p, int col, int row, int colspan, int rowspan) {
	WidgetDef* pwd = new WidgetDef(p, colspan, rowspan);
	return add_widgetdef(pwd, col, row);
}

Settings& Layout::add_in_out(Fl_Input* p, int n_chars_wide, int n_chars_high,
		                     int col, int row, int colspan, int rowspan)
{
	//create WidgetDef for Fl_Input p:
	InOutWidgetDef *pwd = new InOutWidgetDef(p, n_chars_wide, n_chars_high,
			                                 colspan, rowspan);
	return add_widgetdef(pwd, col, row);
}

Settings& Layout::add_check_radio_button(Fl_Button* pBtn, int col, int row,
		                                 int colspan, int rowspan)
{
	WidgetDef* pwd = new WidgetDef((Fl_Widget*)pBtn, colspan, rowspan);
	pwd->type = WidgetType::BUTTON_WITH_RIGHTSIDE_TEXT;
	return add_widgetdef(pwd, col, row);
}

Settings& Layout::add_widgetdef(WidgetDef* pwd, int col, int row)
{
	//get innermost group in process:
	GroupDef *pGrp = _groups_in_process.back();
	pwd->pParent = pGrp;
	pGrp->add(pwd, col, row);
	return pwd->settings;
}

//Label* Layout::createLabel(const char *pTxt) {
//	Size size = TextMeasure::inst().
//			 get_size(pTxt, _settings.labelfont, _settings.labelsize);
//	size.w += (_paddings.w + _paddings.e);
//	size.h += (_paddings.n + _paddings.s);
//
//	Label *pLbl = new Label(0, 0, size.w, size.h, pTxt);
//	pLbl->labelcolor(_settings.labelcolor);
//	pLbl->labelfont(_settings.labelfont);
//	pLbl->labelsize(_settings.labelsize);
//	return pLbl;
//}

} //namespace fluy
