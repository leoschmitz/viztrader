//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCFile.h"
#include "VizCSecurityPeriodsManipulator.h"

#include <QDebug>
#include <QtAlgorithms>
#include <QString>
#include <QDataStream>
#include <QDate>
#include <QDateTime>
#include <QTextStream>
//-----------------------------------------------------------------------------
namespace viz
{
//-----------------------------------------------------------------------------
namespace io
{
//-----------------------------------------------------------------------------
static const QDir ExecutableDirectory = QDir::current();
const QString RelativePathForSecurities = "data";
const QString RelativePathForDeletedFiles = "dump";
const QString FileVersion = "0.0.6";
//-----------------------------------------------------------------------------
VizCFile::VizCFile() 
{
	checkAndRepairTradeFiles();	
}
//-----------------------------------------------------------------------------
VizCFile::~VizCFile()
{
}
//-----------------------------------------------------------------------------
QList<QString> VizCFile::localSecurityIds()
{
	return AvailableSecurities.keys();
}
//-----------------------------------------------------------------------------
bool VizCFile::loadTrade(const QString& symbol, core::VizCCompany &company, bool intraday)
{
	if (!loadSecurity(symbol, company))
	{
		emit error(QString(tr("%1 não foi carregada").arg(symbol)));
		
		return false;		
	}

	if (!intraday)
	{
		if(!loadSecurityPeriods(symbol, company.Securities[0].SecurityPeriods))
		{
			emit error(QString(tr("%1 não teve períodos carregados").arg(symbol)));

			return false;		
		}

		company.Securities[0].SecurityPeriods 
			= core::VizCSecurityPeriodsManipulator::instance()->adjustSecurityPeriodsSplits(company.Splits, company.Securities[0].SecurityPeriods);
	}	
	else
	{
		/// TOOOOODOOOOOOOO
	}

	return true;	
}
//-----------------------------------------------------------------------------
bool VizCFile::loadCompany(QFileInfo fileInfo, core::VizCCompany& in)
{
	setDataDirectory();

	QFile file(fileInfo.filePath());
	if (!file.open(QIODevice::ReadOnly))
		return false;

	QByteArray fileInMemory = file.readAll();

	u16 crc(0);
	QString fileVersion;			
	QDataStream fileStream(&fileInMemory, QIODevice::ReadOnly);

	fileStream 
		>> fileVersion
		>> in
		>> in.Securities
		>> crc;
 	
	if (fileVersion != FileVersion)
	{
		file.close();
		return false;
	}

	fileStream.device()->seek(0);

	if (crc != qChecksum(fileInMemory.data(), (fileInMemory.size()) - sizeof(u16)))
	{
		file.close();
		return false;
	}	

	file.close();

	return true;
}
//-----------------------------------------------------------------------------
bool VizCFile::loadSecurity(const QString& symbol, core::VizCCompany& in)
{
	core::VizCCompany fullCompany;
		
	if (!loadCompany(securityFileInfo(symbol), fullCompany))
		return false;

	in = fullCompany;

	foreach (const core::VizCSecurity& security, fullCompany.Securities)
	{
		if (symbol == security.Symbol)
		{
			in.Securities.append(security);
			break;
		}
	}

	return true;
	
// 	s32 companySize = companySize(fileInfo);
// 
// 	setDataDirectory();
// 	QFile file(fileInfo.filePath());
// 
// 	if (!file.open(QIODevice::ReadOnly))
// 		return false;
// 
// 	QDataStream fileStream(&file);
// 
// 	fileStream.device()->seek(static_cast<qint64>(companySize));
// 
// 	qint64 currentFilePos(-1);
// 	while (!fileStream.atEnd())
// 	{
// 		currentFilePos = fileStream.device()->pos();
// 		core::VizCSecurity security;
// 		fileStream 
// 			>> security
// 			>> crc;
// 		if (crc != security.crc())
// 			return false;
// 		
// 		in.Securities.append(security);		 
// 
// 		break;
// 	}
// 	
// 	infoSize = static_cast<s32>(fileStream.device()->pos() - currentFilePos);
// 
// 	file.close();
}
//-----------------------------------------------------------------------------
bool VizCFile::updateSecurityFile(const QString& symbol, const QVector<core::VizCSecurityPeriod>& periods)
{
	setDataDirectory();

// 	QFile file;
// 	if (AvailableSecurities.contains(symbol))
// 		file.setFileName(AvailableSecurities[symbol].fileName());
// 	else
// 		return false;
// 
// 	if (!file.open(QIODevice::WriteOnly))
// 		return false;
// 
// 	QDataStream fileStream(&file);
// 
// 	fileStream.device()->seek(fileStream.device()->size());
// 
// 	// Salva o registro
// 	s32 regStart = 1;
// 	s32 regEnd   = 2;
// 	foreach(core::VizCSecurityPeriod period, periods)
// 	{
// 		fileStream << regStart 
// 			<< period 
// 			<< period.crc()
// 			<< regEnd;
// 	}	
// 
// 	file.close();

	return true;
}
//-----------------------------------------------------------------------------
void VizCFile::loadTradeList(QVector<QStringList>& tradeList)
{
	tradeList.clear();

	foreach (const QFileInfo& fileInfo, AvailableSecurities.values())
	{
		core::VizCCompany company;
		loadCompany(fileInfo, company);

		foreach (const core::VizCSecurity& security, company.Securities)
		{
			QStringList listRealTime;
			QStringList listHistoric;

			if (DailySecurities.contains(security.Symbol))
			{
				listHistoric 
					<< security.Symbol 
					<< company.WholeName 
					<< QString(tr("Diária"))
					<< security.NegotiableStart.toString(Qt::ISODate) 
					<< security.NegotiableEnd.toString(Qt::ISODate);
				tradeList.append(listHistoric);
			}

			if (RealTimeSecurities.contains(security.Symbol))
			{
				listRealTime 
					<< security.Symbol 
					<< company.WholeName 
					<< QString(tr("Intraday"))
					// TODO: Isso aqui vai ter que entrar no banco
					<< security.NegotiableStart.toString(Qt::ISODate) 
					<< security.NegotiableEnd.toString(Qt::ISODate);
				tradeList.append(listRealTime);
			}			
		}
	}
	
// 	QString visualitica, fileVersion;
// 	s32 control(-1);
// 	core::VizCCompany company;
// 	
// 
// 	foreach (const QString& symbol, AvailableSecurities.keys())
// 	{
// 		QStringList list;
// 
// 		s32 dummySize;
// 
// 		if (!loadSecurity(symbol, company, dummySize))
// 		{
// 			QFile fileToRename(fileInfo(symbol).filePath());
// 			
// 			if (!deleteInvalidFile(fileToRename))
// 			{
// 				qDebug() << "Impossivel renomear lista de ativos. Lista sera invalida.";
// 			}
// 		}			
// 		else
// 		{
// 			list << symbol 
// 				<< company.WholeName 
// 				<< company.Securities.at(0).NegotiableStart.toString(Qt::ISODate) 
// 				<< company.Securities.at(0).NegotiableEnd.toString(Qt::ISODate);
// 		}
// 
// 		tradeList.append(list);
// 	}
}
//-----------------------------------------------------------------------------
// bool VizCFile::loadSecurity(const QString& symbol, core::VizCCompany& in)
// {
// 	return loadSecurity(AvailableSecurities[symbol], in);	
// }
//-----------------------------------------------------------------------------
void VizCFile::checkAndRepairTradeFiles()
{	
	QStringList filters;

	setDataDirectory();

	QFileInfoList fileBrowseCompanies;
		
	filters << "*.vzc";
	fileBrowseCompanies = QDir::current().entryInfoList(filters, QDir::Files, QDir::Name);
	filters.clear();
	
	foreach (const QFileInfo& companyFile, fileBrowseCompanies)
	{
		core::VizCCompany company;
		if (!loadCompany(companyFile, company))
		{
			deleteInvalidFile(companyFile);
		}
		else
		{
			bool modified(false);
			for (int i = 0; i < company.Securities.size(); i++)
			{
				const QString& symbol = company.Securities.at(i).Symbol;

				QString histoFileName = QString("%1.vzh").arg(symbol);
				bool historicDataAvailable = QFile::exists(histoFileName);
				QString realTimeFileName = QString("%1.vzr").arg(symbol);
				bool realTimeDataAvailable = QFile::exists(realTimeFileName);

				if (!(historicDataAvailable) && !(realTimeDataAvailable))
				{
					company.Securities.remove(i);
					modified = true;
				}
				else
				{
					AvailableSecurities.insert(symbol, companyFile);

					if (historicDataAvailable)
					{
						DailySecurities.insert(symbol);
					}

					if (realTimeDataAvailable)
					{
						RealTimeSecurities.insert(symbol);
					}
				}				
			}

			if (modified)
			{
				saveCompany(company);				
			}
		}
	}



	
// 	for (int i = 0; i < fileBrowse.size(); i++)
// 	{		 
// 		s32 dummySize;
// 		core::VizCCompany dummyCompanyInfo;	
// 
// 		bool loadOk = loadSecurity(fileBrowse.at(i), dummyCompanyInfo, dummySize);
// 
// 		if ((loadOk) && ((!AvailableSecurities.contains(dummyCompanyInfo.Securities.at(0).Symbol))))
// 		{
// 			AvailableSecurities.insert(dummyCompanyInfo.Securities.at(0).Symbol, fileBrowse.at(i));
// 		}
// 		else
// 		{
// 			QFile file(fileBrowse[i].filePath());
// 
// 			if (!deleteInvalidFile(file))
// 			{
// 				qDebug() << "Nao foi possivel renomear: " << file.fileName();				
// 			}
// 		}
// 	}	
}
//-----------------------------------------------------------------------------
bool VizCFile::deleteInvalidFile(const QFileInfo& fileInfo)
{
	setDataDirectory();

	QFile file(fileInfo.fileName());

	if (!file.open(QIODevice::ReadWrite))
		return false;

	if (!deleteInvalidFile(file))
	{
		file.close();
		return false;
	}

	file.close();

	return true;
}
//-----------------------------------------------------------------------------
bool VizCFile::deleteInvalidFile(QFile& file)
{
	setDeletedDirectory();
	int fileLimit(0);			
	while (!file.copy(QString("%1_(%2).bak").arg(file.fileName()).arg(QString::number(fileLimit))) 
		&& (fileLimit < 32768))
	{
		fileLimit++;
	}		

	if (fileLimit == 32768)
	{
		emit error(tr("Arquivo não deletado:").arg(file.fileName()));
		return false;
	}
	else
	{
		setDataDirectory();
		if (!file.remove())
		{
			emit error(tr("Arquivo não deletado:").arg(file.fileName()));
			return false;
		}
	}
	
	return true;
}
//-----------------------------------------------------------------------------
bool VizCFile::saveCompany(const core::VizCCompany& company)
{
	// Testa se o company passado tem o securityId
// 	bool containsSecurityId(false);
// 	const core::VizCSecurity* security;
// 	for (int i = 0; i < company.Securities.size(); i++)
// 	{
// 		if (company.Securities[i].Symbol == symbol)
// 		{
// 			containsSecurityId = true;
// 			security = &(company.Securities[i]);
// 			break;
// 		}
// 	}
// 
// 	if (!containsSecurityId)
// 		return false;

	setDataDirectory();

	QFile file;

	file.setFileName(QString("%1.vzc").arg(company.PartialSecuritySymbol));
// 	if (AvailableSecurities.contains(security->Symbol))
// 		file.setFileName(AvailableSecurities[security->Symbol].fileName());
// 	else file.setFileName(security->Symbol + QString(".vzc"));

	if (!file.open(QIODevice::ReadWrite))
		return false;

	QDataStream fileStream(&file);

	// Salva o header
	fileStream 	
		<< FileVersion
		<< company
		<< company.Securities;

	fileStream.device()->seek(0);

	QByteArray b = file.readAll();

	fileStream << static_cast<u16>(qChecksum(b, b.size()));

	file.close();

	return true;
// 		<< *security
// 		<< security->crc();
}
//-----------------------------------------------------------------------------
bool VizCFile::saveDailyPeriods(const QString& symbol, const QVector<core::VizCSecurityPeriod>& histo)
{
	QFile file;

	file.setFileName(QString("%1.vzh").arg(symbol));

	QIODevice::OpenModeFlag mode = file.exists() ? QIODevice::Append : QIODevice::WriteOnly;

	if (!file.open(mode))
	{
		emit error(QString("%1 invalido").arg(file.fileName()));
		return false;
	}
	QDataStream fileStream(&file);

	// Salva o registro
	foreach(const core::VizCSecurityPeriod& period, histo)
	{
		fileStream 	<< period 
			<< period.crc();
	}

	file.close();

	return true;
}
//-----------------------------------------------------------------------------
bool VizCFile::appendDaily(const QString& symbol, const core::VizCCompany& company)
{
	if (!saveCompany(company))
		return false;

	int index(0);
	if (company.Securities.size() > 1)
	{
		foreach (const core::VizCSecurity& security, company.Securities)
		{
			if (security.Symbol == symbol)
				break;

			index++;
		}
	}

	if (!saveDailyPeriods(symbol, company.Securities.at(index).SecurityPeriods))
		return false;	

	return true;
}
//-----------------------------------------------------------------------------
QFileInfo VizCFile::securityFileInfo(QString symbol) const
{
	return AvailableSecurities.value(symbol);
}
//-----------------------------------------------------------------------------
bool VizCFile::containsSymbol(const QString id) const
{
	return AvailableSecurities.contains(id);
}
//-----------------------------------------------------------------------------
bool VizCFile::loadSecurityPeriods(const QString& symbol, QVector<core::VizCSecurityPeriod>& in)
{
	setDataDirectory();

	QFile securityFile(QString("%1.vzh").arg(symbol));

	if (!securityFile.open(QIODevice::ReadOnly))
		return false;

	QByteArray fileInMemory = securityFile.readAll();

	unsigned int fileSize = fileInMemory.size();

	securityFile.close();

	QDataStream fileStream(&fileInMemory, QIODevice::ReadOnly);

	
	int control = 0;
	core::VizCSecurityPeriod period;
	u16 crc(0);

	//////////////////////////////////////////////////////////////////////////
	// A estrutura segue a mesma ideia do header
	// Basicamente vem o controle 01 registro registro ... registro
	//   e termina com crc e depois 02. Ex 01 BLA1
	// Mais detalhes na explicitacao do header
	//////////////////////////////////////////////////////////////////////////


	// para não ocorrer flood manda-se a porcentagem aos pulos de periods
	int jump(0);
	while (!fileStream.atEnd())
	{

		fileStream 
			>> period
			>> crc
			>> control;


		if ((control != 2) || (crc != period.crc()))
			return false;

		in.push_back(period);	


		if ((jump % 30) == 0)
		{
			emit percentageReady(symbol, static_cast<int>(100.0f - (static_cast<f32>(fileStream.device()->bytesAvailable()) 
				/ static_cast<f32>(fileSize)) * 100.0f));

		}
		jump++;
	}		

	return true;	
}
//-----------------------------------------------------------------------------
void VizCFile::setExecutableDirectory()
{
	if (QDir::current() != ExecutableDirectory)
	{
		QDir::setCurrent(ExecutableDirectory.path());
	}
}
//-----------------------------------------------------------------------------
void VizCFile::setDataDirectory()
{
	QDir checkForDataDir(RelativePathForSecurities);

	if (QDir::current() != checkForDataDir)
	{
		setExecutableDirectory();

		if (!checkForDataDir.exists())
		{
			QDir createDirectory;
			createDirectory.mkdir(RelativePathForSecurities);
		}

		QDir::setCurrent(RelativePathForSecurities);
	}
}
//-----------------------------------------------------------------------------
void VizCFile::setDeletedDirectory()
{
	QDir checkForDeletedDir(RelativePathForDeletedFiles);

	if (QDir::current() != checkForDeletedDir)
	{
		setExecutableDirectory();

		if (!checkForDeletedDir.exists())
		{
			QDir createDirectory;
			createDirectory.mkdir(RelativePathForDeletedFiles);
		}

		QDir::setCurrent(RelativePathForDeletedFiles);
	}
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
