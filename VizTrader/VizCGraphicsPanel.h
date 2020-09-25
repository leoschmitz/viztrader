//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCGRAPHICSPANEL_H__
#define __VIZCGRAPHICSPANEL_H__
//-----------------------------------------------------------------------------
#include "VizIInnerPanel.h"
#include "VizIGraphicsItemBasedOnSecurity.h"
#include "VizIGraphicsItemBasedOnShape.h"
#include "VizIChart.h"
#include "VizCVerticalScale.h"
#include "VizCLoadingAnimation.h"
#include <QTransform>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
enum VizEDragType
{
	VEDT_NONE = 0,
	VEDT_CONTROL_POINT,
	VEDT_SHAPE,
	VEDT_SECURITY,
	VEDT_PANEL,
	VECT_PAN,
	VEDT_ZOOM_BOX
};
//-----------------------------------------------------------------------------
class VizCWorkArea;
//-----------------------------------------------------------------------------
class VizCGraphicsPanel : public VizIInnerPanel
{
	Q_OBJECT

	Q_PROPERTY(QColor backGroundColor READ backGroundColor WRITE setBackGroundColor USER true)
	Q_PROPERTY(bool showValuesInTheTitleBar READ showValuesInTheTitleBar WRITE setShowValuesInTheTitleBar USER true)
	Q_PROPERTY(bool useColorOfPlotInValuesOfTitleBar READ useColorOfPlotInValuesOfTitleBar WRITE setUseColorOfPlotInValuesOfTitleBar USER true)

public:
	VizCGraphicsPanel(VizCSecurityWidget* parent, bool isClosable, VizCWorkArea* workArea);
	~VizCGraphicsPanel();

	//pra bloquear o sinal para as animacoes
	Q_INVOKABLE void blockCreateUndoCommandPropertyChanged(bool block);

	virtual void resetToDefaults();
	virtual QString titleBarName() const;

	void setLoadingState(bool value);
	bool isLoadingState() const;
	void setPercentageLoaded(int percent);
		
	bool showValuesInTheTitleBar() const;
	void setShowValuesInTheTitleBar(bool value);
	bool useColorOfPlotInValuesOfTitleBar() const;
	void setUseColorOfPlotInValuesOfTitleBar(bool value);

	void setBackGroundColor(const QColor& color);
	QColor backGroundColor() const;
	void updateVerticalScale();
	VizCVerticalScale* verticalScale();
	void cancelAddItem();
	
	bool containItem(const QString& name) const;
	bool containItem(const VizIGraphicsItem* item) const;
	bool isPossibleRemoveItem(VizIGraphicsItem* item);
	bool isPossibleAddItem(VizIGraphicsItem* item);
	const QList<VizIGraphicsItem*>& items() const;
	VizIGraphicsItemBasedOnSecurity* newGraphicsItemBasedOnSecurity(const QString& name, const QString& securitySymbol, viz::VizEScaleInYOptions scaleInYOptions = viz::VESIYO_DEFAULT, bool isRemovable = true);
	VizIGraphicsItemBasedOnShape* newVizIGraphicsItemBasedOnShape(const QString& name, viz::VizEScaleInYOptions scaleInYOptions = viz::VESIYO_DEFAULT, bool isRemovable = true);
	VizIChart* changeChartType(VizIChart* chart, const QString& newChartTypeName);
	void addItem(VizIGraphicsItem* item, bool isRemovable = true);
	void removeItem(VizIGraphicsItem* item);
	VizIGraphicsItem* cloneItem(VizIGraphicsItem* item, bool preserveIsClosableProperty);

signals:
	void createUndoCommandAddItem(VizIGraphicsItem* item, VizIGraphicsItem* parent);
	void createUndoCommandRemoveItem(VizIGraphicsItem* item, VizIGraphicsItem* parent);

protected:
	virtual bool event(QEvent* event);
	virtual void paintEvent(QPaintEvent* event);
	virtual void mouseDoubleClickEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void keyPressEvent(QKeyEvent* event);
	virtual void leaveEvent(QEvent* event);
	virtual void focusInEvent(QFocusEvent* event);
	virtual void updateData();
	virtual bool isAnyThingSelected() const;

protected slots:
	void slotItemChanged(bool needUpdateVerticalScale);
	void slotCreateUndoCommandPropertyChanged(QObject* item, const QString& propertyName, const QVariant& oldValue, const QVariant& newValue, bool needUpdateVerticalScale);
	void slotCreateUndoCommandPropertiesChanged(QObject* item, const QStringList& propertiesNames, const QVariantList& oldValues, const QVariantList& newValues, bool needUpdateVerticalScale, const QString& commandAliasName);
	void slotCreateUndoCommandParentChanged(VizIGraphicsItem* item, VizIGraphicsItem* oldParent, VizIGraphicsItem* newParent);
	void slotCreateUndoCommandAddItem(VizIGraphicsItem* item, VizIGraphicsItem* parent);
	void slotCreateUndoCommandRemoveItem(VizIGraphicsItem* item, VizIGraphicsItem* parent);
	void slotAplicationSettingsChange();
	void slotMultipleSelectMenu(QAction* action);
	virtual void slotCloseInnerPanelClick();
	
private:
	void createFakeItem();
	void removeFakeItem();
	void addItemBasedOnFakeItem();
	VizIGraphicsItemBasedOnSecurity* findNearestParentItem(const QPointF& mousepos);
	VizIGraphicsItemBasedOnSecurity* privateNewGraphicsItemBasedOnSecurity(const QString& name, const QString& securitySymbol, viz::VizEScaleInYOptions scaleInYOptions = viz::VESIYO_DEFAULT, bool isRemovable = true);
	VizIGraphicsItemBasedOnShape* privateNewVizIGraphicsItemBasedOnShape(const QString& name, viz::VizEScaleInYOptions scaleInYOptions = viz::VESIYO_DEFAULT, bool isRemovable = true);
	void connectItemsFromChangedEvent();
	void disconnectItemsFromChangedEvent();
	void connectItemFromChangedEvent(VizIGraphicsItem* item);
	void disconnectItemFromChangedEvent(VizIGraphicsItem* item);
	s32 controlPointsContains(const QList<QPointF>& points, const QPointF& point);
	void drawControlPoints(QPainter* painter, const QList<QPointF>& points);
	void drawCrossHairs(QPainter* painter);
	void drawZoomBox(QPainter* painter);
	void recursivePaint(QPainter* painter, VizIGraphicsItem* item);
	void recursiveYRange(VizIGraphicsItem* item, bool& isEmpty, f64& minY, f64& maxY);
	void recursiveAjustToOverlayWithoutScale(VizIGraphicsItem* item, f64 itemMinY, f64 itemMaxY);
	void recursiveBuild(VizIGraphicsItem* item);
	void recursiveRemoveItem(VizIGraphicsItem* item);
	void recursiveIsPossibleRemoveItem(VizIGraphicsItem* item, bool& ok);
	void recursiveAddItem(VizIGraphicsItem* item);
	VizIGraphicsItem* recursiveCloneItem(VizIGraphicsItem* parent, VizIGraphicsItem* item, bool preserveIsClosableProperty);

	bool isAnyItemSelected() const;
	QList<VizIGraphicsItem*> itemsAt(const QPointF& pos);
	VizIGraphicsItem* findSelectedItem(const QList<VizIGraphicsItem*>& list) const;
	QTransform transformWindowToViewPort() const;
	QTransform transformViewPortToWindow() const;
	QPointF mapToViewPort(const QPoint& pos, bool discrete = false);
	QSizeF mapToViewPort(const QSize& size);

	s32 itemIndex(VizIGraphicsItem* item);
	s32 graphicsItemIndex(VizIGraphicsItem* gi) const;

	VizIGraphicsItemBasedOnSecurity* baseGraphicsItem() const;

	
	QMenu* MultipleSelectMenu;
	VizCVerticalScale* VerticalScale;
	VizCHorizontalScale* HorizontalScale;
	QColor BackGroundColor;
	QList<VizIGraphicsItem*> Items;
	QWidget* Dummy;
	f64 MousePan;
	QPointF MousePressPositionMappedInWindowContinuum;
	QPointF MousePressPositionMappedInWindowDiscrete;
	QPointF MousePositionMappedInWindowContinuum;
	QPointF MousePositionMappedInWindowDiscrete;
	VizEDragType CurrentDragType;
	VizSShapeTransform CurrentShapeTransform;
	VizIGraphicsItemBasedOnShape* FakeItem;
	bool Antialiasing;
	bool UseGradientToFillShapes;
	bool ShowValuesInTheTitleBar;
	bool UseColorOfPlotInValuesOfTitleBar;

	bool BlockCreateUndoCommandPropertyChanged;

	VizCLoadingAnimation* LoadingAnimation;
	bool LoadingState;

	static VizCGraphicsPanel* ZoomBoxCurrentGraphicsPanel;
	static VizCGraphicsPanel* CrossHairsCurrentGraphicsPanel;

};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
