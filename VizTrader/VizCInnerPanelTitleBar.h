//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCINNERPANELTITLEBAR_H__
#define __VIZCINNERPANELTITLEBAR_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include <QWidget>
#include <QToolButton>
#include <QMouseEvent>
#include <QTimer>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizIInnerPanel;
//-----------------------------------------------------------------------------
class VizCInnerPanelTitleBar : public QWidget
{
	Q_OBJECT

public:
	VizCInnerPanelTitleBar(VizIInnerPanel* parent, bool isClosable = true);
	~VizCInnerPanelTitleBar();

	virtual void setVisible(bool value);
	bool isMaximized() const;
	bool isClosable() const;
	QSize minimumSizeHint() const;
	QSize sizeHint() const;
	void setButtonsState(bool maximized);
	void setButtonsActive(bool actived);
	bool isButtonsActive() const;
	bool buttonsContains(const QPoint& pos) const;
	s32 titleBarHeight() const;

signals:
	void InnerPanelMaximizeButtonClick();
	void InnerPanelCloseButtonClick();

protected:
	virtual void paintEvent(QPaintEvent* event);
	virtual void enterEvent(QEvent* event);
	virtual void leaveEvent(QEvent* event);


private:

	void setupTitleBar();
	f64 viewportWidth() const;

	void ajustButtonsAlphaChannel(f64 alphaValue);
	void ajustTitleBarAlphaChannel(f64 alphaValue);
	void ajustAlphaChannel(QPixmap& pix, f64 alphaValue);


	QColor TitleBackgroundColor1;
	QColor TitleBackgroundColor2;
	QColor TitleTextColor;
	QColor TitleBackgroundColorIsSelected;
	QColor TitleTextColorIsSelected;


	VizIInnerPanel* Parent;
	bool IsClosable;
	bool IsVisible;
	bool IsMaximized;
	bool IsTitleButtonsActive;

	QToolButton* CloseButton;
	QToolButton* MaximizeButton;
	QPixmap MaximizeButtonPixmap[2];
	QPixmap ClosePixmap;

	QTimer* ButtonAutoHideTimer;
	f64 ButtonAutoHideAlpha;
	bool ButtonAutoHideInc;
	
private slots:
	void computeButtonAutoHideAlphaChannel();
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
