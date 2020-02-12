#ifndef LABEL_H_
#define LABEL_H_

#include <FL/Fl_Widget.H>
#include <string>

namespace fluy {

class Label: public Fl_Widget {
public:
	Label(int x, int y, int w, int h, const char* pTxt);
	virtual ~Label();

	void draw();
private:

};

} //namespace fluy

#endif /* LABEL_H_ */
