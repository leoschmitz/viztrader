//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizIGraphicsItem.h"
#include "VizCWorkArea.h"
#include "VizCGraphicsPanel.h"
#include "VizGUIUtils.h"
#include <QLineF>
#include <QDebug>
#include <QMetaProperty>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
VizIGraphicsItem::VizIGraphicsItem(VizIGraphicsItem* parent): QObject(), ParentItem(parent), IsSelected(false), IsRemovable(true), IsVisible(true), VerticalScale(0), HorizontalScale(0), Settings(0), WorkArea(0), Visibility(1.0f)
{
	//vizObjectSetName(this, tr(""));
	//vizObjectSetSelected(this, false);

	Settings = io::VizCSettings::instance();

	resetToDefaults();

	if(ParentItem)
	{
		if(ParentItem->childItemIndex(this) == -1)
		{
			ParentItem->addChildItem(this);
		}
	}
}
//-----------------------------------------------------------------------------
VizIGraphicsItem::~VizIGraphicsItem()
{
}
//-----------------------------------------------------------------------------
void VizIGraphicsItem::setVisibility(f64 value)
{
	Visibility = value;
	emit itemChanged();
}
//-----------------------------------------------------------------------------
f64 VizIGraphicsItem::visibility() const
{
	return Visibility;
}
//-----------------------------------------------------------------------------
void VizIGraphicsItem::blockCreateUndoCommandPropertyChanged(bool block)
{
	VizCGraphicsPanel* panel = WorkArea->selectedGraphicsPanel();
	Q_ASSERT(panel);
	panel->blockCreateUndoCommandPropertyChanged(block);
}
//-----------------------------------------------------------------------------
void VizIGraphicsItem::resetToDefaults()
{
	ScaleInYOptions = viz::VESIYO_DEFAULT;
}
//-----------------------------------------------------------------------------
void VizIGraphicsItem::setWorkArea(VizCWorkArea* workArea)
{
	Q_ASSERT(workArea);
	WorkArea = workArea;
}
//-----------------------------------------------------------------------------
VizCWorkArea* VizIGraphicsItem::workArea() const
{
	return WorkArea;
}
//-----------------------------------------------------------------------------
void VizIGraphicsItem::setFillRectWithGradient(bool value)
{
	UseGradientToFillShapes = value;
}
//-----------------------------------------------------------------------------
bool VizIGraphicsItem::fillRectWithGradient() const
{
	return UseGradientToFillShapes;
}
//-----------------------------------------------------------------------------
bool VizIGraphicsItem::isSelected() const
{
	return IsSelected;
}
//-----------------------------------------------------------------------------
void VizIGraphicsItem::setSelected(bool selected)
{
	if(IsSelected != selected)
	{
		IsSelected = selected;
		emit itemChanged();
	}
}
//-----------------------------------------------------------------------------
void VizIGraphicsItem::setIsRemovable(bool isRemovable)
{
	if(IsRemovable != isRemovable)
	{
		IsRemovable = isRemovable;
		emit itemChanged();
	}

	/*
	IsRemovable = isRemovable;
	foreach(VizIGraphicsItem* item, ChildItems)
	{
		item->setIsRemovable(IsRemovable);
	}*/
}
//-----------------------------------------------------------------------------
bool VizIGraphicsItem::isRemovable() const
{
	return IsRemovable;
}
//-----------------------------------------------------------------------------
void VizIGraphicsItem::setVisible(bool visible)
{
	if(IsVisible != visible)
	{
		IsVisible = visible;
		emit itemChanged();
	}
}
//-----------------------------------------------------------------------------
bool VizIGraphicsItem::isVisible() const
{
	return IsVisible;
}
//-----------------------------------------------------------------------------
void VizIGraphicsItem::setVerticalScale(VizCVerticalScale* scale)
{
	VerticalScale = scale;
	foreach(VizIGraphicsItem* item, ChildItems)
	{
		item->setVerticalScale(VerticalScale);
	}
}
//-----------------------------------------------------------------------------
VizCVerticalScale* VizIGraphicsItem::verticalScale() const
{
	return VerticalScale;
}
//-----------------------------------------------------------------------------
void VizIGraphicsItem::setHorizontalScale(VizCHorizontalScale* scale)
{
	HorizontalScale = scale;
	foreach(VizIGraphicsItem* item, ChildItems)
	{
		item->setHorizontalScale(HorizontalScale);
	}
}
//-----------------------------------------------------------------------------
VizCHorizontalScale* VizIGraphicsItem::horizontalScale() const
{
	return HorizontalScale;
}
//-----------------------------------------------------------------------------
QRectF VizIGraphicsItem::graphicsPanelViewPort() const
{
	return gui::graphicsPanelViewPort(VerticalScale, HorizontalScale);
}
//-----------------------------------------------------------------------------
void VizIGraphicsItem::ajustPointToOverlayWithoutScale(f64 itemMinY, f64 itemMaxY, QPointF& point)
{
	f64 minY = VerticalScale->minimum();
	f64 maxY = VerticalScale->maximum();
	f64 border = VerticalScale->border();
	f64 H = (itemMaxY - itemMinY) + (2 * border);
	f64 scaleY = (maxY - minY) / H;
	point.setY(point.y() - (itemMinY - border));
	point.setY(point.y() * scaleY);
	point.setY(point.y() + minY);
}
//-----------------------------------------------------------------------------
void VizIGraphicsItem::ajustLinesToOverlayWithoutScale(f64 itemMinY, f64 itemMaxY, QVector<QPointF>& lines)
{
	f64 minY = VerticalScale->minimum();
	f64 maxY = VerticalScale->maximum();
	f64 border = VerticalScale->border();
	f64 H = (itemMaxY - itemMinY) + (2 * border);
	f64 scaleY = (maxY - minY) / H;
	for(s32 i = 0; i < lines.size(); i++)
	{
		QPointF& p = lines[i];
		p.setY(p.y() - (itemMinY - border));
		p.setY(p.y() * scaleY);
		p.setY(p.y() + minY);
	}
}
//-----------------------------------------------------------------------------
void VizIGraphicsItem::ajustRectsToOverlayWithoutScale(f64 itemMinY, f64 itemMaxY, QVector<QRectF>& rects)
{
	f64 minY = VerticalScale->minimum();
	f64 maxY = VerticalScale->maximum();
	f64 border = VerticalScale->border();
	f64 H = (itemMaxY - itemMinY) + (2 * border);
	f64 scaleY = (maxY - minY) / H;
	for(s32 i = 0; i < rects.size(); i++)
	{
		QRectF& r = rects[i];

		QPointF pos = r.topLeft();
		pos.setY(pos.y() - (itemMinY - border));
		pos.setY(pos.y() * scaleY);
		pos.setY(pos.y() + minY);

		QSizeF size = r.size();
		size.setHeight(size.height() * scaleY);

		r.setTopLeft(pos);
		r.setSize(size);
	}
}
//-----------------------------------------------------------------------------
VizIGraphicsItem* VizIGraphicsItem::parentItem() const
{
	return ParentItem;
}
//-----------------------------------------------------------------------------
bool VizIGraphicsItem::isRoot() const
{
	return !ParentItem;
}
//-----------------------------------------------------------------------------
bool VizIGraphicsItem::hasChilds() const
{
	return !ChildItems.isEmpty();
}
//-----------------------------------------------------------------------------
void VizIGraphicsItem::isPossibleParent(bool& ok, const VizIGraphicsItem* possibleParent, const VizIGraphicsItem* item) const
{

	if(!ok)
		return;

	bool stopRecursion = false;

	if(!item->hasChilds())
	{
		stopRecursion = true;
	}

	if(possibleParent == item)
	{
		ok = false;
		return;
	}

	if(!stopRecursion)
	{
		foreach(VizIGraphicsItem* child, item->childItems())
		{
			isPossibleParent(ok, possibleParent, child);
		}
	}
}
//-----------------------------------------------------------------------------
bool VizIGraphicsItem::setParentItem(VizIGraphicsItem* parent)
{
	bool ok = true;
	isPossibleParent(ok, parent, this);
	if(!ok)
	{
		return false;
	}

	if(ParentItem)
	{
		if(ParentItem->childItemIndex(this) != -1)
		{
			ParentItem->removeChildItem(this);
		}
	}

	VizIGraphicsItem* oldParent = ParentItem;

	ParentItem = parent;
	if(ParentItem)
	{
		if(ParentItem->childItemIndex(this) == -1)
		{
			ParentItem->addChildItem(this);
		}
	}

	if(oldParent != ParentItem)
	{
		emit createUndoCommandParentChanged(this, oldParent, ParentItem);
	}

	return true;
}
//-----------------------------------------------------------------------------
QList<VizIGraphicsItem*> VizIGraphicsItem::childItems() const
{
	return ChildItems;
}
//-----------------------------------------------------------------------------
void VizIGraphicsItem::addChildItem(VizIGraphicsItem* child)
{
	ChildItems.append(child);
}
//-----------------------------------------------------------------------------
void VizIGraphicsItem::removeChildItem(VizIGraphicsItem* child)
{
	s32 index = childItemIndex(child);
	if(index != -1)
	{
		ChildItems.removeAt(index);
	}
}
//-----------------------------------------------------------------------------
s32 VizIGraphicsItem::childItemIndex(VizIGraphicsItem* child)
{
	Q_ASSERT(child);
	for(s32 i = 0; i < ChildItems.size(); i++)
	{
		if(ChildItems[i] == child)
		{
			return i;
		}
	}
	return -1;
}
//-----------------------------------------------------------------------------
VizEScaleInYOptions VizIGraphicsItem::scaleInYOptions() const
{
	return ScaleInYOptions;
}
//-----------------------------------------------------------------------------
void VizIGraphicsItem::setScaleInYOptions(VizEScaleInYOptions scaleInYOptions)
{
	if(ScaleInYOptions != scaleInYOptions)
	{
		int old = propertyGetScaleInYOptions();

		recursiveSetScaleInYOptions(this, scaleInYOptions);

		emit createUndoCommandPropertyChanged(this, "scaleInYOptions", old, propertyGetScaleInYOptions(), true);
		emit itemChanged(true);
	}
}
//-----------------------------------------------------------------------------
void VizIGraphicsItem::recursiveSetScaleInYOptions(VizIGraphicsItem* item, VizEScaleInYOptions scaleInYOptions)
{

	bool stopRecursion = false;

	if(!item->hasChilds())
	{
		stopRecursion = true;
	}

	item->ScaleInYOptions = scaleInYOptions;

	if(!stopRecursion)
	{
		foreach(VizIGraphicsItem* child, item->childItems())
		{
			recursiveSetScaleInYOptions(child, ScaleInYOptions);
		}
	}

}
//-----------------------------------------------------------------------------
int VizIGraphicsItem::propertyGetScaleInYOptions() const
{
	return static_cast<int>(scaleInYOptions());
}
//-----------------------------------------------------------------------------
void VizIGraphicsItem::propertySetScaleInYOptions(int value)
{
	setScaleInYOptions(static_cast<VizEScaleInYOptions>(value));
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
