//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCWORKAREAMANAGER_H__
#define __VIZCWORKAREAMANAGER_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include "VizCWorkArea.h"
#include <QStackedWidget>
#include <QObject>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCWorkAreaManager : public QObject
{
	Q_OBJECT

public:
	static VizCWorkAreaManager* instance();

	s32 newWorkArea(const QString& name = QString(""));
	QPair<QString, VizCWorkArea*> workArea(s32 index) const;
	s32 workAreaCount() const;
	bool removeWorkArea(s32 index);
	bool renameWorkAreaName(s32 index, const QString& newName);
	void moveWorkArea(s32 from, s32 to);

private:
	VizCWorkAreaManager();
	~VizCWorkAreaManager();
	QString generateName();
	bool containsWorkArea(const QString& name) const;
	
	QList< QPair<QString, VizCWorkArea*> > WorkAreaList;
	static VizCWorkAreaManager* Instance;

signals:
	void workAreaAdded(int index);
	void workAreaRemoved(int index);
	void workAreaRename(int index);
	void workAreaMove(int from, int to);
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
