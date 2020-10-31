//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCFileThread.h"
#include "VizCCompany.h"
//-----------------------------------------------------------------------------
namespace viz
{
//-----------------------------------------------------------------------------
namespace io
{
//-----------------------------------------------------------------------------
VizCFileThread::VizCFileThread(const QString& securitySymbol, VizCCache* interfaceToCache, VizCFile* interfaceToDisk) : SecuritySymbol(securitySymbol)
{
	InterfaceToCache = interfaceToCache;
	InterfaceToDisk = interfaceToDisk;	
}
//-----------------------------------------------------------------------------
VizCFileThread::~VizCFileThread()
{

}
//-----------------------------------------------------------------------------
void VizCFileThread::run()
{
	s32 fileSize(-1);
	core::VizCCompany company;

	if (InterfaceToDisk->loadTrade(SecuritySymbol, company))
	{
		InterfaceToCache->insertSecurity(company, fileSize);	
	}
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
