/*
 * LayoutTree.h
 *
 *  Created on: 14.02.2020
 *      Author: martin
 */

#ifndef LAYOUTTREE_H_
#define LAYOUTTREE_H_

#include "internal.h"
#include "WidgetDef.h"

namespace fluy {

class TreeElement {

};

class LayoutTree {
public:
	LayoutTree(WidgetDef* pRoot);
	virtual ~LayoutTree();
private:
	WidgetDef* _pRoot;
};

} //namespace fluy
#endif /* LAYOUTTREE_H_ */
