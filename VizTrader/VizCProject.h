//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Solu��es em Visualiza��o LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCPROJECT_H__
#define __VIZCPROJECT_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include "VizCWorkArea.h"
#include "VizCWorkAreaManager.h"
#include <QObject>
//-----------------------------------------------------------------------------
namespace viz
{
namespace io
{
//-----------------------------------------------------------------------------
class VizCProject : QObject
{
	Q_OBJECT
public:

	VizCProject(){}
	~VizCProject(){}

};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------