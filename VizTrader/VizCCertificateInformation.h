//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCCERTIFICATEINFORMATION_H__
#define __VIZCCERTIFICATEINFORMATION_H__
//-----------------------------------------------------------------------------
#include <QtGui/QDialog>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslCipher>
#include "ui_VizCCertificateInformation.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCCertificateInformation : public QDialog, private Ui::UIVizCCertificateInformation
{
    Q_OBJECT
public:
	static VizCCertificateInformation* instance();
	void setCertificateChainAndCiphers(const QList<QSslCertificate>& chain, const QList<QSslCipher>& ciphers);

private slots:
    void updateCertificateInfo(int index);

private:
	VizCCertificateInformation(QWidget* parent = 0);
	~VizCCertificateInformation();
	static VizCCertificateInformation* Instance;

	QList<QSslCertificate> Chain;
	QList<QSslCipher> Ciphers;
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
