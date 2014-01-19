#include "EditorSlider.h"

#include "../../model/Context.h"
#include <KSynth/Sequencer.h>
#include "Editor.h"
#include "../../misc/Helper.h"

EditorSlider::EditorSlider(Editor& editor, Context& ctx, QWidget *parent) :
	Grabable(parent), editor(editor), ctx(ctx) {

	setMinimumWidth(16);
	setMaximumWidth(16);
	setMinimumHeight(100);

	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
	setVisible(true);

}

EditorSlider::~EditorSlider() {
	;
}

void EditorSlider::onGrab(int x, int y, int w, int h) {
	setGeometry(x,y,w,h);
}
void EditorSlider::onGrabDone(int, int, int, int) {
	;
}

#include <QPainter>
void EditorSlider::paintEvent(QPaintEvent* e) {

	Q_UNUSED(e);
	QPainter p(this);

	QPen p1; p1.setColor(QColor(220,220,220));
	QPen p2; p2.setColor(QColor(64,64,64));
	QPen p3; p3.setColor(QColor(0,0,0,48)); p3.setWidth(2);

	// slider
	p.setPen(p1);
	p.drawLine(7, 14, 7, height());

	p.setPen(p2);
	p.drawLine(8, 14, 8, height());

	// shadow
	p.setPen(p3);
	p.drawLine(10, 10, 10, height());

	// grab
	static QImage imgTop = Helper::getSkinImage("skin/timeSlider.png", "PNG");
	p.drawImage(0,0,imgTop);

}
