//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZVERSIONNUMBER_H__
#define __VIZVERSIONNUMBER_H__
//-----------------------------------------------------------------------------
#include <QString>
#include "VizTypes.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace system
{
//-----------------------------------------------------------------------------
enum VizEStageCodes
{
	VESC_DEVELOPMENT_STAGE = 1,
	VESC_ALPHA_STAGE = 2,
	VESC_BETA_STAGE = 3,
	VESC_RELEASE_CANDIDATE_STAGE = 4,
	VESC_RELEASED = 5
};
//-----------------------------------------------------------------------------
struct VizSVersionNumber
{
	VizSVersionNumber()
	{
		Major = 1;
		Minor = 0;
		Patch = 0;
		DevelopmentRevision = 1;
		Stage = VESC_DEVELOPMENT_STAGE;
	}

	VizSVersionNumber(u8 major, u8 minor, u8 patch, u32 developmentRevision, VizEStageCodes stage)
	{
		Major = major;
		Minor = minor;
		Patch = patch;
		DevelopmentRevision = developmentRevision;
		Stage = stage;
	}
	QString toShortString()
	{
		QString stage;
		if(Stage == VESC_DEVELOPMENT_STAGE)
			stage = "d";
		if(Stage == VESC_ALPHA_STAGE)
			stage = "a";
		if(Stage == VESC_BETA_STAGE)
			stage = "b";
		if(Stage == VESC_RELEASE_CANDIDATE_STAGE)
			stage = "rc";
		if(Stage == VESC_RELEASED)
			stage = "";
		
		QString devRevision = Stage == VESC_RELEASED  ? "" : QString::number(DevelopmentRevision);
		QString patch = Patch == 0 ? "" : QString(".%1").arg(Patch);

		return QString("%1.%2%3%4%5").arg(Major).arg(Minor).arg(patch).arg(stage).arg(devRevision);
	}

	QString toLongString()
	{
		QString stage;
		if(Stage == VESC_DEVELOPMENT_STAGE)
			stage = "- development";
		if(Stage == VESC_ALPHA_STAGE)
			stage = "- alpha";
		if(Stage == VESC_BETA_STAGE)
			stage = "- beta";
		if(Stage == VESC_RELEASE_CANDIDATE_STAGE)
			stage = "- release candidate";
		if(Stage == VESC_RELEASED)
			stage = "";

		QString devRevision = Stage == VESC_RELEASED  ? "" : QString("- revision %1 [%6 - %7]").arg(DevelopmentRevision).arg(__DATE__).arg(__TIME__);
		QString patch = Patch == 0 ? "" : QString(".%1").arg(Patch);

		return QString("%1.%2%3 %4 %5").arg(Major).arg(Minor).arg(patch).arg(stage).arg(devRevision);
	}

	static s32 compareVersions(const VizSVersionNumber& version1, const VizSVersionNumber& version2)
	{
		if(version1.Major > version2.Major) return 1;
		if(version1.Major < version2.Major) return -1;
		if(version1.Minor > version2.Minor) return 1;
		if(version1.Minor < version2.Minor) return -1;
		if(version1.Patch > version2.Patch) return 1;
		if(version1.Patch < version2.Patch) return -1;
		if(version1.Stage > version2.Stage) return 1;
		if(version1.Stage < version2.Stage) return -1;

		u8 DevelopmentRevision1 = version1.DevelopmentRevision;
		u8 DevelopmentRevision2 = version2.DevelopmentRevision;

		if(version1.Stage == VESC_RELEASED)
		{
			if(version1.DevelopmentRevision == 0) DevelopmentRevision1 = 0xFFFF;
			if(version2.DevelopmentRevision == 0) DevelopmentRevision2 = 0xFFFF;
		}

		if (DevelopmentRevision1 > DevelopmentRevision2) return 1;
		if (DevelopmentRevision1 < DevelopmentRevision2) return -1;

		return 0;
	}

	u8 Major;
	u8 Minor;
	u8 Patch;
	u32 DevelopmentRevision;
	VizEStageCodes Stage;
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
