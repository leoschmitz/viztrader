//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCClipboard.h"
#include "VizMimeFormat.h"
#include <QDebug>
#include <QMimeData>
//-----------------------------------------------------------------------------
namespace viz
{
//-----------------------------------------------------------------------------
namespace gui
{
//-----------------------------------------------------------------------------
VizCClipboard* VizCClipboard::Instance = 0;
//-----------------------------------------------------------------------------
VizCClipboard::VizCClipboard() : QObject(), GraphicsItem(0)
{
	QObject::connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(slotDataChanged()));
}
//-----------------------------------------------------------------------------
VizCClipboard::~VizCClipboard()
{
}
//-----------------------------------------------------------------------------
void VizCClipboard::clear()
{
	GraphicsItem = 0;
	QApplication::clipboard()->clear();
}
//-----------------------------------------------------------------------------
void VizCClipboard::setGraphicsItem(VizIGraphicsItem* item)
{
	GraphicsItem = item;
	emit dataChanged();
}
//-----------------------------------------------------------------------------
void VizCClipboard::setPixmap(const QPixmap& pixmap)
{
	QMimeData* imagedata = new QMimeData;
	imagedata->setImageData(pixmap);
	QApplication::clipboard()->setMimeData(imagedata);
}
//-----------------------------------------------------------------------------
void VizCClipboard::setText(const QString& text)
{
	QMimeData* textdata = new QMimeData;
	textdata->setText(text);
	QApplication::clipboard()->setMimeData(textdata);
}
//-----------------------------------------------------------------------------
QPixmap VizCClipboard::pixmap() const
{
	return QApplication::clipboard()->pixmap();
}
//-----------------------------------------------------------------------------
QString VizCClipboard::text() const
{
	return QApplication::clipboard()->text();
}
//-----------------------------------------------------------------------------
VizIGraphicsItem* VizCClipboard::graphicsItem() const
{
	return GraphicsItem;
}
//-----------------------------------------------------------------------------
void VizCClipboard::slotDataChanged()
{
	emit dataChanged();
}
//-----------------------------------------------------------------------------
VizCClipboard* VizCClipboard::instance()
{
	if(!Instance)
	{
		Instance = new VizCClipboard;
	}
	return Instance;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------

