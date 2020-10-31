//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCWorkAreaManager.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
VizCWorkAreaManager* VizCWorkAreaManager::Instance = 0;
//-----------------------------------------------------------------------------
VizCWorkAreaManager* VizCWorkAreaManager::instance()
{
	if(!Instance)
	{
		Instance = new VizCWorkAreaManager;
	}
	return Instance;
}
//-----------------------------------------------------------------------------
VizCWorkAreaManager::VizCWorkAreaManager() : QObject()
{
}
//-----------------------------------------------------------------------------
VizCWorkAreaManager::~VizCWorkAreaManager()
{
}
//-----------------------------------------------------------------------------
QString VizCWorkAreaManager::generateName()
{

	s32 Count = WorkAreaList.size();

	bool ok = false;
	QString title;
	do
	{
		Count++;
		title = QObject::tr("Área de Trabalho %1").arg(Count);
		ok = true;
		if(containsWorkArea(title))
		{
			ok = false;
		}
	}
	while(!ok);

	return title;
}
//-----------------------------------------------------------------------------
bool VizCWorkAreaManager::containsWorkArea(const QString& name) const
{
	for(s32 i = 0; i < WorkAreaList.size(); i++)
	{
		const QPair<QString, VizCWorkArea*>& pair = WorkAreaList.at(i);
		if(pair.first == name)
		{
			return true;
		}
	}
	return false;
}
//-----------------------------------------------------------------------------
s32 VizCWorkAreaManager::newWorkArea(const QString& name)
{
	s32 index = -1;
	QString title = name;

	if(name.isEmpty())
	{
		title = generateName();
	}

	if(!containsWorkArea(title))
	{
		VizCWorkArea* wa = new VizCWorkArea;
		WorkAreaList.append(QPair<QString, VizCWorkArea*>(title, wa));
		index = WorkAreaList.size() - 1;
		//StackedWidget->insertWidget(index, wa->mdiArea());

		emit workAreaAdded(index);
	}
	return index;
}
//-----------------------------------------------------------------------------
QPair<QString, VizCWorkArea*> VizCWorkAreaManager::workArea(s32 index) const
{
	Q_ASSERT(index >= 0 || index < WorkAreaList.size());
	//StackedWidget->setCurrentIndex(index);
	return WorkAreaList.at(index);
}
//-----------------------------------------------------------------------------
s32 VizCWorkAreaManager::workAreaCount() const
{
	return WorkAreaList.size();
}
//-----------------------------------------------------------------------------
bool VizCWorkAreaManager::removeWorkArea(s32 index)
{
	if(index < 0 || index >= WorkAreaList.size())
		return false;

	QPair<QString, VizCWorkArea*>& pair = WorkAreaList[index];

	pair.second->disconnect();
	pair.second->deleteLater();
	WorkAreaList.removeAt(index);

	emit workAreaRemoved(index);
	
	return true;
}
//-----------------------------------------------------------------------------
bool VizCWorkAreaManager::renameWorkAreaName(s32 index, const QString& newName)
{
	if(index < 0 || index >= WorkAreaList.size() || containsWorkArea(newName))
		return false;

	QPair<QString, VizCWorkArea*>& pair = WorkAreaList[index];
	pair.first = newName;

	emit workAreaRename(index);

	return true;
}
//-----------------------------------------------------------------------------
void VizCWorkAreaManager::moveWorkArea(s32 from, s32 to)
{
	WorkAreaList.move(from, to);

	emit workAreaMove(from, to);
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
