//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCMemoryLeakReport.h"
#ifdef WIN32
#include <crtdbg.h>
#endif
//-----------------------------------------------------------------------------
namespace viz
{
namespace system
{
//-----------------------------------------------------------------------------
VizCMemoryLeakReport::VizCMemoryLeakReport()
{
#ifdef WIN32
#ifdef _DEBUG
	/*_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE );
	_CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT );
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE );
	_CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDOUT );
	_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE );
	_CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDOUT );
	*/
	
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_WNDW );
	_CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDERR );

#endif
#endif
}
//-----------------------------------------------------------------------------
VizCMemoryLeakReport::~VizCMemoryLeakReport()
{
#ifdef WIN32
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif
#endif
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
