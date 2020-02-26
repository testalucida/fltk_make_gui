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
	if (!_pGroupdef) {
		if (type != GroupType::DOUBLE_WINDOW) {
			throw runtime_error("Layout::group_begin(): "
					"first group has to be of type DOUBLE_WINDOW");
		}
		_pGroupdef = pGrpDef;
	} else {
		pGrpDef->colspan = colspan;
		pGrpDef->rowspan = rowspan;
		GroupDef* pParent = _groups_in_process.back();
		pGrpDef->pParent = pParent;
		//get x:
		pGrpDef->x = pParent->get_x(col);
		//get y:
		pGrpDef->y = pParent->get_y(row);

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

Settings& Layout::add_label(const char *pTxt, int col, int row,
		                    int colspan, int rowspan)
{
	//create WidgetDef for the label:
	Label* pLabel = new Label(0, 0, 0, 0, pTxt);
	WidgetDef* pwd = new WidgetDef(pLabel);
	pwd->type = WidgetType::LABEL;

	//set size:
	Fl_Font font = pwd->settings.fonts.labelfont;
	Fl_Fontsize fontsize = pwd->settings.fonts.labelsize;
	Size size = TextMeasure::inst().get_size(pTxt, font, fontsize);
	pLabel->size(size.w, size.h + PAD_N + PAD_S);

	return add_widgetdef(pwd, col, row);
}

Settings& Layout::add(Fl_Widget* p, int col, int row, int colspan, int rowspan)
{
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


} //namespace fluy
