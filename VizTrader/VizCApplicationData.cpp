//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCApplicationData.h"
#include <QObject>
#include <QFile>
#include <QTextStream>
//-----------------------------------------------------------------------------
namespace viz
{
namespace system
{
//-----------------------------------------------------------------------------
VizCApplicationData* VizCApplicationData::Instance = 0;
//-----------------------------------------------------------------------------
VizCApplicationData* VizCApplicationData::instance()
{
	if(!Instance)
	{
		Instance = new VizCApplicationData;
	}

	return Instance;
}
//-----------------------------------------------------------------------------
VizCApplicationData::VizCApplicationData()
{
	QFile licenseFile(":/VizTrader/Resources/license.txt");
	licenseFile.open(QIODevice::ReadOnly);
	QTextStream inLicense(&licenseFile);
	QString licenseString = inLicense.readAll();
	licenseFile.close();

	AppicationName = QObject::tr("VizTrader");
	ApplicationVersion = VizSVersionNumber(1, 0, 0, 14, VESC_DEVELOPMENT_STAGE);
	OrganizationName = QObject::tr("Visualítica");
	OrganizationDomain = QObject::tr("visualitica.com");
	CopyrightStatement = QObject::tr("(C) 2008-2009 Copyright Visualítica Soluções em Visualização LTDA. Todos direitos reservados.");
	LicenseText = licenseString;
	BugReportEmailAddress = QObject::tr("viztraderbugs@visualitica.com");
	HomepageAddress = QObject::tr("http://visualitica.com/viztrader");
	ForgotPasswordURL = QObject::tr("https://www.visualitica.com/viztrader/accounts/ForgotPassword");

}
//-----------------------------------------------------------------------------
QString VizCApplicationData::appicationName() const
{
	return AppicationName;
}
//-----------------------------------------------------------------------------
VizSVersionNumber VizCApplicationData::applicationVersion() const
{
	return ApplicationVersion;
}
//-----------------------------------------------------------------------------
QString VizCApplicationData::organizationName() const
{
	return OrganizationName;
}
//-----------------------------------------------------------------------------
QString VizCApplicationData::organizationDomain() const
{
	return OrganizationDomain;
}
//-----------------------------------------------------------------------------
QString VizCApplicationData::copyrightStatement() const
{
	return CopyrightStatement;
}
//-----------------------------------------------------------------------------
QString VizCApplicationData::licenseText() const
{
	return LicenseText;
}
//-----------------------------------------------------------------------------
QString VizCApplicationData::bugReportEmailAddress() const
{
	return BugReportEmailAddress;
}
//-----------------------------------------------------------------------------
QString VizCApplicationData::homepageAddress() const
{
	return HomepageAddress;
}
//-----------------------------------------------------------------------------
QString VizCApplicationData::forgotPasswordURL() const
{
	return ForgotPasswordURL;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
