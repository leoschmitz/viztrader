//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCAPPLICATIONDATA_H__
#define __VIZCAPPLICATIONDATA_H__
//-----------------------------------------------------------------------------
#include <QString>
#include "VizTypes.h"
#include "VizVersionNumber.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace system
{
//-----------------------------------------------------------------------------
class VizCApplicationData
{
public:

	static VizCApplicationData* instance();

	QString appicationName() const;
	VizSVersionNumber applicationVersion() const;
	QString organizationName() const;
	QString organizationDomain() const;
	QString copyrightStatement() const;
	QString licenseText() const;
	QString bugReportEmailAddress() const;
	QString homepageAddress() const;
	QString forgotPasswordURL() const;

private:
	VizCApplicationData();
	QString AppicationName;
	VizSVersionNumber ApplicationVersion;
	QString OrganizationName;
	QString OrganizationDomain;
	QString CopyrightStatement;
	QString LicenseText;
	QString BugReportEmailAddress;
	QString HomepageAddress;
	QString ForgotPasswordURL;

	static VizCApplicationData* Instance;
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
