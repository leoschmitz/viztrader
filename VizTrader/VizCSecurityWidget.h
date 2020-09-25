//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCSECURITYWIDGET_H__
#define __VIZCSECURITYWIDGET_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include "VizCHorizontalScale.h"
#include "VizCDataFacade.h"
#include <QWidget>
#include <QSplitter>
#include <QWheelEvent>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizIInnerPanel;
class VizCGraphicsPanel;
class VizCOfferBookPanel;
class VizCWorkArea;
//-----------------------------------------------------------------------------
class VizCSecurityWidget : public QWidget
{
	Q_OBJECT

public:
	VizCSecurityWidget(const QString& label, bool isClosable, VizCWorkArea* workArea, QWidget* parent = 0);
	~VizCSecurityWidget();

	bool isClosable() const;
	void updateHorizontalScale();
	VizCHorizontalScale* horizontalScale() const;
	void updateData();

	VizCGraphicsPanel* newGraphicsPanel(bool isClosable);
	VizCOfferBookPanel* newOfferBookPanel(bool isClosable);
	s32 indexOfPanel(VizIInnerPanel* panel) const;
	void closeInnerPanel(int index);
	VizIInnerPanel* innerPanel(int index) const;
	s32 countInnerPanel() const;

	/*VizCGraphicsPanel* newGraphicsPanel(bool isClosable);
	VizCOfferBookPanel* newOfferBookPanel(bool isClosable);
	VizIInnerPanel* cloneInnerPanel(VizIInnerPanel* innerpanel);
	void addInnerPanel(VizIInnerPanel* innerpanel);
	*/

public slots:
	void securityChanged(const QString& securitySymbol, bool input);
	void securityPercentageLoaded(const QString& securitySymbol, int percent);

protected:
	virtual void dragEnterEvent(QDragEnterEvent* event);
	virtual void dropEvent(QDropEvent* event);
	virtual void wheelEvent(QWheelEvent* event);
	
private:
	void showMaximizeInnerPanel(VizIInnerPanel* w);
	void closeInnerPanel(VizIInnerPanel* w);
	void manageTitleButtons();
	void ajustSecurityPeriods();
	void findInnerPanel(const QPoint& p, bool isOutside, bool isCopyAction, s32& currentIndex, s32& nextIndex);
	void manageInnerPanelDropEvent(QDropEvent* event);
	void manageGraphicsItemDropEvent(QDropEvent* event);
	void changeInnerPanel(VizIInnerPanel* ip, int index);
	VizCGraphicsPanel* privateAddGraphicsPanel(bool isClosable, s32 index);
	VizIInnerPanel* addCopyInnerPanel(VizIInnerPanel* innerpanel, s32 index);
	void availableSecuritySymbol(const QString& securitySymbol);
	void unavailableSecuritySymbol(const QString& securitySymbol);

	io::VizCDataFacade* DataFacade;
	VizCWorkArea* WorkArea;
	bool IsClosable;
	VizCHorizontalScale* HorizontalScale;
	QSplitter* Splitter;

	friend class VizIInnerPanel;

private slots:
	void slotHorizontalScaleChange(bool rulerValuesIndexChange, bool rulerValuesChange, bool periodicityChange, bool marginChange);

};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
