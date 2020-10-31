//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZLIB_H__
#define __VIZLIB_H__
//-----------------------------------------------------------------------------
#include <QDebug>
#include <QtGlobal>
#include <QVariant>
#include "VizVersionNumber.h"
#include "VizCApplicationData.h"
#include "VizTypes.h"
#include "VizMath.h"
#include "VizRange.h"
#include "VizMimeFormat.h"
#include "VizCCompany.h"
#include "VizCSecurity.h"
#include "VizCSecurityPeriod.h"
#include "VizCTradePeriod.h"
#include "VizCOpenSecurityWidget.h"
#include "VizCOpenIndicatorWidget.h"
#include "VizCSecurityWidget.h"
#include "VizIInnerPanel.h"
#include "VizCGraphicsPanel.h"
#include "VizCOfferBookPanel.h"
#include "VizCMdiSubWindow.h"
//#include "VizCPreviewGraphics.h"
#include "VizCDataInspector.h"
#include "VizCPropertyEditor.h"
#include "VizCSceneInspector.h"
#include "VizCTradePeriod.h"
#include "VizCTechnicalAnalysisFunctions.h"
#include "VizIGraphicsItem.h"
#include "VizIGraphicsItemBasedOnSecurity.h"
#include "VizIGraphicsItemBasedOnShape.h"
#include "VizIGraphicsItemBasedOnShape.h"
#include "VizIIndicator.h"
#include "VizIChart.h"
#include "VizCToolManager.h"
#include "VizCApplicationSettings.h"
#include "VizCSettings.h"
#include "VizCDataFacade.h"
#include "VizCMemoryUtils.h"
#include "VizCUndoManager.h"
#include "VizCUndoCommands.h"
#include "VizCUndoView.h"
#include "VizCClipboard.h"
#include "VizCEditActions.h"
#include "VizCCertificateInformation.h"
#include "VizCWorkArea.h"
#include "VizCWorkAreaManager.h"
#include "VizObjectUtils.h"
#include "VizCPropertiesAliasNames.h"
#include "VizCWorkAreaManagerWidget.h"
//indicators
#include "VizCMovingAverageIndicator.h"
#include "VizCVolumeIndicator.h"
#include "VizCMinusDirectionalIndicator.h"
#include "VizCPlusDirectionalIndicator.h"
#include "VizCAverageDirectionalMovementIndexIndicator.h"
#include "VizCRelativeStrengthIndexIndicator.h"
#include "VizCBollingerBandsIndicator.h"
#include "VizCBollingerLowerBandIndicator.h"
#include "VizCBollingerUpperBandIndicator.h"
//charts
#include "VizCLineChart.h"
#include "VizCMountainChart.h"
#include "VizCCandlesticksChart.h"
#include "VizCBarsChart.h"
#include "VizCHeikinAshiBarsChart.h"
#include "VizCHeikinAshiCandlesticksChart.h"
//shapes
#include "VizCHorizontalLine.h"
#include "VizCTrendLine.h"
#include "VizCVerticalLine.h"
#include "VizCSemiLogTrendLine.h"
//io
#include "VizCTP.h"
#include "VizCDataFacade.h"
//-----------------------------------------------------------------------------
namespace viz
{
	namespace core
	{
	}
	namespace io
	{
	}
	namespace gui
	{
	}
	namespace system
	{
	}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
