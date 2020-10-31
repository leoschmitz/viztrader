//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCOFFERBOOKPANEL_H__
#define __VIZCOFFERBOOKPANEL_H__
//-----------------------------------------------------------------------------
#include "VizIInnerPanel.h"
#include <QLabel>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCWorkArea;
//-----------------------------------------------------------------------------
class VizCOfferBookPanel : public VizIInnerPanel
{
	Q_OBJECT

public:
	VizCOfferBookPanel(VizCSecurityWidget* parent, bool isClosable, VizCWorkArea* workArea);
	~VizCOfferBookPanel();

	virtual void resetToDefaults();
	virtual QString titleBarName() const;

protected:
	virtual bool event(QEvent* event);
	virtual void mouseDoubleClickEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void updateData();

private:
	
	QLabel* Dummy;
	friend class VizCSecurityWidget;
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
