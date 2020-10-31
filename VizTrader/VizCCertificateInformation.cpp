//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCCertificateInformation.h"
#include <QTextCodec>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
VizCCertificateInformation* VizCCertificateInformation::Instance = 0;
//-----------------------------------------------------------------------------
VizCCertificateInformation::VizCCertificateInformation(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    connect(certificationPathView, SIGNAL(currentRowChanged(int)), this, SLOT(updateCertificateInfo(int)));
}
//-----------------------------------------------------------------------------
VizCCertificateInformation::~VizCCertificateInformation()
{
}
//-----------------------------------------------------------------------------
VizCCertificateInformation* VizCCertificateInformation::instance()
{
	if(!Instance)
	{
		Instance = new VizCCertificateInformation();
	}
	return Instance;
}
//-----------------------------------------------------------------------------
void VizCCertificateInformation::setCertificateChainAndCiphers(const QList<QSslCertificate> &chain, const QList<QSslCipher>& ciphers)
{
    Chain = chain;
	Ciphers = ciphers;

	
	QSslCipher ciph = Ciphers.first();
	QString cipherstr = QString("%1, %2 (%3/%4) - %5").arg(ciph.authenticationMethod())
		.arg(ciph.name()).arg(ciph.usedBits()).arg(ciph.supportedBits()).arg(ciph.protocolString());

		cipherLabel->setText(cipherstr);

	/*
	cryptographicCiphersView->clear();
	for(int i = 0; i < Ciphers.size(); ++i)
	{
		const QSslCipher& cipher = Ciphers.at(i);


		cryptographicCiphersView->addItem(tr("%1, %2 (%3/%4) %5 %6 %7").arg(ciph.authenticationMethod())
			.arg(ciph.name()).arg(ciph.usedBits()).arg(ciph.supportedBits())
			.arg(ciph.encryptionMethod()).arg(ciph.keyExchangeMethod()).arg(ciph.protocolString()));

	}
	*/


    certificationPathView->clear();
    for(int i = 0; i < Chain.size(); ++i)
	{
        const QSslCertificate &cert = Chain.at(i);
        certificationPathView->addItem(tr("%1%2 (%3)").arg(!i ? QString() : tr("Emitido por: "))
                                             .arg(cert.subjectInfo(QSslCertificate::Organization).join(' '))
                                             .arg(cert.subjectInfo(QSslCertificate::CommonName).join(' ')));
    }

    certificationPathView->setCurrentRow(0);
}
//-----------------------------------------------------------------------------
void VizCCertificateInformation::updateCertificateInfo(int index)
{
    certificateInfoView->clear();
    if (index >= 0 && index < Chain.size())
	{

        const QSslCertificate &cert = Chain.at(index);

		/*QTextCodec* codec = QTextCodec::codecForName("UTF-8");
		QString cu = cert.subjectInfo(QSslCertificate::Organization);
		*/

        QStringList lines;
        lines << tr("Organization: %1").arg(cert.subjectInfo(QSslCertificate::Organization).join(' '))
              << tr("Subunit: %1").arg(cert.subjectInfo(QSslCertificate::OrganizationalUnitName).join(' '))
              << tr("Country: %1").arg(cert.subjectInfo(QSslCertificate::CountryName).join(' '))
              << tr("Locality: %1").arg(cert.subjectInfo(QSslCertificate::LocalityName).join(' '))
              << tr("State/Province: %1").arg(cert.subjectInfo(QSslCertificate::StateOrProvinceName).join(' '))
              << tr("Common Name: %1").arg(cert.subjectInfo(QSslCertificate::CommonName).join(' '))
              << QString()
              << tr("Issuer Organization: %1").arg(cert.issuerInfo(QSslCertificate::Organization).join(' '))
              << tr("Issuer Unit Name: %1").arg(cert.issuerInfo(QSslCertificate::OrganizationalUnitName).join(' '))
              << tr("Issuer Country: %1").arg(cert.issuerInfo(QSslCertificate::CountryName).join(' '))
              << tr("Issuer Locality: %1").arg(cert.issuerInfo(QSslCertificate::LocalityName).join(' '))
              << tr("Issuer State/Province: %1").arg(cert.issuerInfo(QSslCertificate::StateOrProvinceName).join(' '))
              << tr("Issuer Common Name: %1").arg(cert.issuerInfo(QSslCertificate::CommonName).join(' '));
		foreach (QString line, lines)
		{
			//QString l = codec->fromUnicode(line);

            certificateInfoView->addItem(line);
		}
    }
	else
	{
        certificateInfoView->clear();
    }
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
