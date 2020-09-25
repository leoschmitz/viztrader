//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZIGRAPHICSITEM_H__
#define __VIZIGRAPHICSITEM_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include "VizCVerticalScale.h"
#include "VizCHorizontalScale.h"
#include "VizCSettings.h"
#include "VizObjectUtils.h"
#include <QPainter>
#include <QDebug>
#include <QDateTime>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCWorkArea;
//-----------------------------------------------------------------------------
class VizIGraphicsItem : public QObject
{
	Q_OBJECT
	Q_PROPERTY(int scaleInYOptions READ propertyGetScaleInYOptions WRITE propertySetScaleInYOptions USER true)
	Q_PROPERTY(bool selected READ isSelected WRITE setSelected)
	Q_PROPERTY(bool visible READ isVisible WRITE setVisible)
	Q_PROPERTY(double visibility READ visibility WRITE setVisibility)

public:
	VizIGraphicsItem(VizIGraphicsItem* parent = 0);
	VizIGraphicsItem(const VizIGraphicsItem& other)
	{
		qDebug() << "VizIGraphicsItem copy constructor";
	}
	~VizIGraphicsItem();

	VizIGraphicsItem* parentItem() const;
	bool isRoot() const;
	bool hasChilds() const;
	bool setParentItem(VizIGraphicsItem* parent);
	QList<VizIGraphicsItem*> childItems() const;
	bool isSelected() const;
	void setSelected(bool selected);
	void setIsRemovable(bool isRemovable);
	bool isRemovable() const;
	bool isVisible() const;
	void setVisible(bool visible);
	VizEScaleInYOptions scaleInYOptions() const;
	void setScaleInYOptions(VizEScaleInYOptions scaleInYOptions);
	void setVerticalScale(VizCVerticalScale* scale);
	VizCVerticalScale* verticalScale() const;
	void setHorizontalScale(VizCHorizontalScale* scale);
	VizCHorizontalScale* horizontalScale() const;
	void setWorkArea(VizCWorkArea* workArea);
	VizCWorkArea* workArea() const;
	void setFillRectWithGradient(bool value);
	bool fillRectWithGradient() const;

	//pra bloquear o sinal para as animacoes
	Q_INVOKABLE void blockCreateUndoCommandPropertyChanged(bool block);

	void setVisibility(f64 value);
	f64 visibility() const;


	Q_INVOKABLE virtual void resetToDefaults();
	virtual void build() = 0;
	virtual void paint(QPainter* painter) = 0;
	virtual void ajustToOverlayWithoutScale(f64 itemMinY, f64 itemMaxY) = 0;
	virtual void yRange(bool& isEmpty, f64& minY, f64& maxY) const = 0;
	virtual bool intersects(const QRectF& rect) const = 0;
	virtual QString toolTip(const QPointF& p) const = 0;

protected:
	QRectF graphicsPanelViewPort() const;
	void ajustPointToOverlayWithoutScale(f64 itemMinY, f64 itemMaxY, QPointF& point);
	void ajustLinesToOverlayWithoutScale(f64 itemMinY, f64 itemMaxY, QVector<QPointF>& points);
	void ajustRectsToOverlayWithoutScale(f64 itemMinY, f64 itemMaxY, QVector<QRectF>& rects);
	void addChildItem(VizIGraphicsItem* child);
	void removeChildItem(VizIGraphicsItem* child);
	s32 childItemIndex(VizIGraphicsItem* child);
	void recursiveSetScaleInYOptions(VizIGraphicsItem* item, VizEScaleInYOptions scaleInYOptions);
	bool IsVisible;
	bool IsSelected;
	bool IsRemovable;
	bool UseGradientToFillShapes;
	float Visibility;
	VizEScaleInYOptions ScaleInYOptions;
	VizCVerticalScale* VerticalScale;
	VizCHorizontalScale* HorizontalScale;
	VizIGraphicsItem* ParentItem;
	QList<VizIGraphicsItem*> ChildItems;
	io::VizCSettings* Settings;
	VizCWorkArea* WorkArea;

signals:
	void itemChanged(bool needUpdateVerticalScale = false);
	void createUndoCommandPropertyChanged(QObject* item, const QString& propertyName, const QVariant& oldValue, const QVariant& newValue, bool needUpdateVerticalScale);
	void createUndoCommandPropertiesChanged(QObject* item, const QStringList& propertiesNames, const QVariantList& oldValues, const QVariantList& newValues, bool needUpdateVerticalScale, const QString& commandAliasName);
	void createUndoCommandParentChanged(VizIGraphicsItem* item, VizIGraphicsItem* oldParent, VizIGraphicsItem* newParent);

private:
	//Q_DISABLE_COPY(VizIGraphicsItem)

	int propertyGetScaleInYOptions() const;
	void propertySetScaleInYOptions(int value);
	void isPossibleParent(bool& ok, const VizIGraphicsItem* possibleParent, const VizIGraphicsItem* item) const;
	
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
