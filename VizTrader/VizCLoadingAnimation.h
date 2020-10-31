//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCLOADINGANIMATION_H__
#define __VIZCLOADINGANIMATION_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include <QWidget>
#include <QPaintEvent>
#include <QTimer>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCLoadingAnimation : public QWidget
{
	Q_OBJECT

public:
	VizCLoadingAnimation(QWidget* parent = 0);
	~VizCLoadingAnimation();
	void setPercentageLoaded(s32 percent);
	void startAnimation();
	void stopAnimation();

protected:
	void paintEvent(QPaintEvent *event);

private slots:
	void updateAlpha();

private:
	s32 PercentageLoaded;
	QTimer* Timer;
	f32 Alpha;


};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
