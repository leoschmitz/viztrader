//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCCLIPBOARD_H__
#define __VIZCCLIPBOARD_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include "VizIGraphicsItem.h"
#include <QObject>
#include <QPixmap>
#include <QString>
#include <QClipboard>
#include <QApplication>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCClipboard : public QObject
{
	Q_OBJECT

public:
	static VizCClipboard* instance();

	void clear();
	void setGraphicsItem(VizIGraphicsItem* item);
	void setPixmap(const QPixmap& pixmap);
	void setText(const QString& text);
	QPixmap pixmap() const;
	QString text() const;
	VizIGraphicsItem* graphicsItem() const;

signals:
	void dataChanged();

private:
	VizCClipboard();
	~VizCClipboard();
	static VizCClipboard* Instance;
	VizIGraphicsItem* GraphicsItem;

private slots:
	void slotDataChanged();

};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
