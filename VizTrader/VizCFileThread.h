//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCFILETHREAD_H__
#define __VIZCFILETHREAD_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include "VizCCache.h"
#include "VizCFile.h"

#include <QRunnable>
#include <QString>
//-----------------------------------------------------------------------------
namespace viz
{
namespace io
{
//-----------------------------------------------------------------------------
class VizCFileThread: public QObject, public QRunnable
{	
	Q_OBJECT
public:
	VizCFileThread(const QString& securitySymbol, VizCCache* interfaceToCache, VizCFile* interfaceToDisk);
	~VizCFileThread();

protected:
	void run();

private:
	QString SecuritySymbol;
	VizCCache* InterfaceToCache;
	VizCFile* InterfaceToDisk;
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif