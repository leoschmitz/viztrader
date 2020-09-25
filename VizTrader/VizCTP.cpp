//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCTP.h"
//-----------------------------------------------------------------------------
#define LAN_MODE 
//-----------------------------------------------------------------------------
namespace viz
{
namespace net
{
//-----------------------------------------------------------------------------
// Quaisquer mudanças nessas portas devem ser reescritas também no servidor
const int SslListeningPort = 12000;	
const int TcpListeningPort = 12001;
const QString VizTPProtocolVersion = "0.0.3";
#ifdef LAN_MODE
//const QString HostNameForSecurities = "192.168.0.100";
const QString HostNameForSecurities = "localhost";
#else
const QString HostNameForSecurities = "null-away.no-ip.biz";
#endif 
const int SslTimeoutMilliseconds = 10000;
//-----------------------------------------------------------------------------
VizCTP* VizCTP::Instance = 0;
//-----------------------------------------------------------------------------
VizCTP* VizCTP::instance()
{
	if(!Instance)
	{
		Instance = new VizCTP;
	} 
	return Instance;
}
//-----------------------------------------------------------------------------
VizCTP::VizCTP(QObject* parent) : QObject(parent), SslBlockSize32(0), 
	TcpBlockSize32(0), SessionId('9', 32), TcpAuthenticationPassed(false), User(""), 
	Md5Password(""), CertificateValid(true), CurrentState(UNCONNECTED_STATE), 
	OpenSockets(0), TcpReceivingPacketType(VETPCP_INVALID)
{
	QObject::connect(this, SIGNAL(error(VizCTP::VizTPError)), SLOT(manageLocalError(VizCTP::VizTPError)));

	TcpSocket = new QTcpSocket(this);

	QObject::connect(TcpSocket, SIGNAL(readyRead()), 
		this, SLOT(tcpReceiveData()));
	QObject::connect(TcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), 
		this, SLOT(manageSocketError(QAbstractSocket::SocketError)));
	QObject::connect(TcpSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
		this, SLOT(tcpSocketStateChanged(QAbstractSocket::SocketState)));
	
	SslSocket = new QSslSocket(this);	
	
	QObject::connect(SslSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
		this, SLOT(sslSocketStateChanged(QAbstractSocket::SocketState)));
	QObject::connect(SslSocket, SIGNAL(disconnected()),
		this, SLOT(sslDisconnected()));
	QObject::connect(SslSocket, SIGNAL(encrypted()),
		this, SLOT(sslSocketEncrypted()));
	QObject::connect(SslSocket, SIGNAL(sslErrors(QList<QSslError>)),
		this, SLOT(sslErrors(QList<QSslError>)));
	QObject::connect(SslSocket, SIGNAL(readyRead()),
		this, SLOT(sslReceiveData()));	
	QObject::connect(SslSocket, SIGNAL(error(QAbstractSocket::SocketError)), 
		this, SLOT(manageSocketError(QAbstractSocket::SocketError)));	
}
//-----------------------------------------------------------------------------
VizCTP::~VizCTP()
{
	if (TcpSocket)
		TcpSocket->disconnectFromHost();

	if (SslSocket)
		SslSocket->disconnectFromHost();

	setState(UNKNOWN_STATE);
}
//-----------------------------------------------------------------------------
void VizCTP::getCertificate()
{
	sslConnect();
}
//-----------------------------------------------------------------------------
void VizCTP::connectToVizServer(const QString& user, const QString& md5Password)
{
	if (!isCertificateValid())
	{
		emit error(CERTIFICATE_INVALID_ERROR);

		return;
	}
	
	User = user;

	Md5Password = md5Password;		

	authenticateUser();		
}
//-----------------------------------------------------------------------------
void VizCTP::sslDisconnected()
{
	emit disconnected();
}
//-----------------------------------------------------------------------------
void VizCTP::disconnectFromVizServer()
{	
	closeConnection();
}
//-----------------------------------------------------------------------------
void VizCTP::authenticateTcpConnection()
{
	setState(AUTHENTICATING_SESSION_STATE);

	VizCTPPacket packet(VETPSP_UNENCRYPTED_SESSIONID);

	packet << SessionId;

	TcpSocket->write(packet.getPacketReady());
}
//-----------------------------------------------------------------------------
void VizCTP::manageLocalError(VizCTP::VizTPError localError)
{
	disconnectFromVizServer();
}
//-----------------------------------------------------------------------------
void VizCTP::manageSocketError(QAbstractSocket::SocketError socketError)
{
	switch (socketError)
	{
	case QAbstractSocket::UnfinishedSocketOperationError:
	case QAbstractSocket::UnsupportedSocketOperationError:
	case QAbstractSocket::SocketAddressNotAvailableError:
	case QAbstractSocket::AddressInUseError:
	case QAbstractSocket::SocketAccessError:
	case QAbstractSocket::DatagramTooLargeError:
	case QAbstractSocket::RemoteHostClosedError:
	case QAbstractSocket::SocketResourceError:
		emit error(SOCKET_ERROR);
		break;
	case QAbstractSocket::UnknownSocketError:
		emit error(UNKNOWN_ERROR);		
	case QAbstractSocket::ConnectionRefusedError: 
		emit error(CONNECTION_REFUSED_ERROR);
		break;
	case QAbstractSocket::HostNotFoundError:
		emit error(HOST_NOT_FOUND_ERROR);
		break;
	case QAbstractSocket::SocketTimeoutError:
		emit error(SOCKET_TIMEOUT_ERROR);
		break;
	case QAbstractSocket::NetworkError:
		emit error(NETWORK_ERROR);
		break;
	case QAbstractSocket::ProxyAuthenticationRequiredError:
		emit error(PROXY_AUTHENTICATION_REQUIRED_ERROR);
		break;
	case QAbstractSocket::SslHandshakeFailedError:
		emit error(SSL_HANDSHAKE_ERROR);
		break;
	case QAbstractSocket::ProxyConnectionRefusedError:
		emit error(PROXY_CONNECTION_REFUSED_ERROR);
		break;
	case QAbstractSocket::ProxyConnectionClosedError:
		emit error(PROXY_CONNECTION_CLOSED_ERROR);
		break;
	case QAbstractSocket::ProxyConnectionTimeoutError:
		emit error(PROXY_CONNECTION_TIMEOUT_ERROR);
		break;
	case QAbstractSocket::ProxyNotFoundError:
		emit error(PROXY_NOT_FOUND_ERROR);
		break;
	case QAbstractSocket::ProxyProtocolError:
		emit error(PROXY_PROTOCOL_ERROR);
		break;
	}	
}
//-----------------------------------------------------------------------------
bool VizCTP::setProxy(const QNetworkProxy& networkProxy)
{
	if (SslSocket->proxy() == networkProxy)
		return false;

	SslSocket->setProxy(networkProxy);
	TcpSocket->setProxy(networkProxy);

	return true;
}
//-----------------------------------------------------------------------------
void VizCTP::sslConnect()
{	
	SslSocket->abort();

	SslSocket->connectToHostEncrypted(HostNameForSecurities, SslListeningPort);		
}
//-----------------------------------------------------------------------------
QList<QSslCipher> VizCTP::ciphers()
{
	return SslSocket->ciphers();
}
//-----------------------------------------------------------------------------
QList<QSslCertificate> VizCTP::chainOfCertificates()
{
	return SslSocket->peerCertificateChain();
}
//-----------------------------------------------------------------------------
void VizCTP::closeConnection() 
{
	CertificateValid = true;
	TcpAuthenticationPassed = false;

	setState(CLOSING_STATE);

	if (SslSocket->isValid())
		SslSocket->disconnectFromHost();
	

	if (TcpSocket->isValid())
		TcpSocket->disconnectFromHost();	
}
//-----------------------------------------------------------------------------
void VizCTP::tcpSocketStateChanged(QAbstractSocket::SocketState state)
{
	switch (state)
	{
	case QAbstractSocket::HostLookupState:
		{
			setState(WAIT_TCP_CONNECTION);
		}
		break;
	case QAbstractSocket::ConnectingState:
		{		
			++OpenSockets;						
		}
		break;
	case QAbstractSocket::ConnectedState:
		{		
		}		
		break;
	case QAbstractSocket::UnconnectedState:
		{
			setState(UNCONNECTED_STATE);
			closeConnection();
		}		
		break;
	case QAbstractSocket::ClosingState:	
		{		
			--OpenSockets;
			setState(CLOSING_STATE);
			closeConnection();
		}
		break;
	}
}
//-----------------------------------------------------------------------------
void VizCTP::sslSocketStateChanged(QAbstractSocket::SocketState state)
{
	switch (state)
	{
	case QAbstractSocket::HostLookupState:
		{
			setState(HOST_LOOKUP_STATE);
		}
		break;
	case QAbstractSocket::ConnectingState:
		{		
			++OpenSockets;
			
			setState(CONNECTING_STATE);				
		}
		break;
	case QAbstractSocket::ConnectedState:
		{			
			setState(CERTIFIED_STATE);
		}		
		break;
	case QAbstractSocket::UnconnectedState:
		{
			setState(UNCONNECTED_STATE);
			closeConnection();
		}		
		break;
	case QAbstractSocket::ClosingState:	
		{		
			--OpenSockets;

			setState(CLOSING_STATE);

			closeConnection();
		}
		break;
	}	
}
//-----------------------------------------------------------------------------
void VizCTP::setState(VizTPState newState)
{
	if (newState == state())
		return;

	switch (newState) 
	{
	case UNKNOWN_STATE:
		break;
	case HOST_LOOKUP_STATE:
		break;
	case CONNECTING_STATE:
		break;
	case CERTIFIED_STATE:
		break;
	case AUTHENTICATING_USER_STATE:
		break;	
	case CLOSING_STATE:		
		break;
	case UNCONNECTED_STATE:		
		{
			if (OpenSockets > 0)
				return;
		}
		break;		
	}

	CurrentState = newState;

	qDebug() << newState;

	emit stateChanged(state());
}
//-----------------------------------------------------------------------------
VizCTP::VizTPState VizCTP::state()
{
	return CurrentState;
}
//-----------------------------------------------------------------------------
bool VizCTP::onlineAndAuthenticate()
{
	return TcpAuthenticationPassed;
}
//-----------------------------------------------------------------------------
void VizCTP::sslSocketEncrypted()
{
	if (!SslSocket)
	{
		closeConnection();

		return;     
	}	

	emit certificateReady(isCertificateValid());
}
//-----------------------------------------------------------------------------
void VizCTP::tcpConnect()
{
	TcpSocket->connectToHost(HostNameForSecurities, TcpListeningPort);
}
//-----------------------------------------------------------------------------
void VizCTP::sslReceiveSessionId(QDataStream& in)
{
	in >> SessionId;

	tcpConnect();
}
//-----------------------------------------------------------------------------
void VizCTP::requestNegotiableList(const QDate& lastDate)
{
	if (!onlineAndAuthenticate())
		return;

	emit syncronizingNegotiableList();

	VizCTPPacket packet;

	// requisita FULL
	if (lastDate.isNull())
	{
		packet.setPacketType(VETPSP_REQUEST_FULL_NEGOTIABLE_LIST);
	}
	else
	{
		packet.setPacketType(VETPSP_REQUEST_PARTIAL_NEGOTIABLE_LIST);
		packet << lastDate;		
	}

	if (SslSocket->isEncrypted())
		SslSocket->write(packet.getPacketReady());
}
//-----------------------------------------------------------------------------
void VizCTP::requestSessionId()
{
	setState(STARTING_SESSION_STATE);

	VizCTPPacket packet(VETPSP_REQUEST_SESSIONID);
 	
	if (SslSocket->isEncrypted())
		SslSocket->write(packet.getPacketReady());
}
//-----------------------------------------------------------------------------
void VizCTP::requestSecurity(const QString& securityId, const QDate& lastDate)
{
	VizCTPPacket packet;
	if (lastDate.isNull())
	{
		packet.setPacketType(VETPSP_REQUEST_FULL_HISTO);
		packet << securityId;
	}
	else
	{
		packet.setPacketType(VETPSP_REQUEST_PARTIAL_HISTO);
		packet << securityId;
		packet << lastDate;
	}
	
	if (SslSocket->isEncrypted())
		SslSocket->write(packet.getPacketReady());
	
}
//-----------------------------------------------------------------------------
void VizCTP::tcpReceivePartialSecurityList(QDataStream& in)
{
	in >> NegotiableListElementsToAdd;
	in >> NegotiableListElementsToRemove;
	emit negotiableListReady();
}
//-----------------------------------------------------------------------------
void VizCTP::tcpReceiveFullSecurityList(QDataStream& in)
{
	in >> NegotiableListElementsToAdd;
	emit negotiableListReady();	
}
//-----------------------------------------------------------------------------
void VizCTP::tcpReceiveSecurity(QDataStream& in, const s32& blockSize)
{
	core::VizCCompany company;
	core::VizCSecurity security;
	
	s32 numberOfPeriods(0);

	in >> company;
	in >> security;
	in >> security.SecurityPeriods;

	company.Securities.push_back(security);

	RequestedCompanies.append(QPair<core::VizCCompany, s32>(company, blockSize));		
}
//-----------------------------------------------------------------------------
void VizCTP::manageUnavailableSecurity(QDataStream& in)
{
	QString unavailableSecurity;
	in >> unavailableSecurity;
	emit securityUnavailable(unavailableSecurity);
}
//-----------------------------------------------------------------------------
void VizCTP::tcpReceiveFullRealTime(QDataStream& in)
{
	core::VizCCompany company;
	core::VizCSecurity security; 

	in >> company;
	in >> security;
	in >> security.RealTimePeriods;

	company.Securities.append(security);
}
//-----------------------------------------------------------------------------
void VizCTP::tcpReceiveTick(QDataStream& in)
{
	QString symbol;

	in >> symbol;

	core::VizCRealTimePeriod rtPeriod;

	in >> rtPeriod;

	qDebug() << rtPeriod.TradeDateTime << rtPeriod.TradePrice;
}
//-----------------------------------------------------------------------------
void VizCTP::tcpManagePacket(QDataStream& in, bool& receivingNextPacket)
{
	receivingNextPacket = false;

	if (TcpBlockSize32 == 0)
	{
		if (TcpSocket->bytesAvailable() < (int) sizeof(quint32))
			return;

		in >> TcpBlockSize32;
	}	

	if (TcpReceivingPacketType == VETPCP_INVALID)
	{
		if (TcpSocket->bytesAvailable() < (int) sizeof(VizETPClientPacket))
			return;

		in >> TcpReceivingPacketType;
	}

	if ((TcpReceivingPacketType == VETPCP_TCP_FULL_HISTO) || (TcpReceivingPacketType == VETPCP_TCP_FULL_NEGOTIABLE_LIST))
	{
		emit percentageReady(TcpReceivingPacketType, static_cast<f32>(TcpSocket->bytesAvailable() + sizeof(VizETPClientPacket))  / static_cast<f32>(TcpBlockSize32));		
	}

	if ((TcpSocket->bytesAvailable() + sizeof(VizETPClientPacket)) < TcpBlockSize32)
		return;

	switch (TcpReceivingPacketType)
	{
	case VETPCP_INVALID:
		break;
	case VETPCP_TCP_FULL_HISTO:
		tcpReceiveSecurity(in, TcpBlockSize32);
		emit tcpFullSecurityReady();				
		break;
	case VETPCP_TCP_CURRENTLY_UNAVAILABLE_HISTO:
		manageUnavailableSecurity(in);
		break;
	case VETPCP_TCP_PARTIAL_HISTO:
		tcpReceiveSecurity(in, TcpBlockSize32);	
		emit tcpPartialSecurityReady();
		break;
	case VETPCP_TCP_HISTO_UP_TO_DATE:
		break;
	case VETPCP_TCP_UPDATE_COMPANY_INFO:
		break;
	case VETPCP_TCP_UPDATE_SECURITY_INFO:
		break;
	case VETPCP_TCP_REMOVE_COMPANY_HISTO:
		break;
	case VETPCP_TCP_REMOVE_SECURITY_HISTO:
		break;
	case VETPCP_TCP_HISTO_EMPTY:
		tcpReceiveSecurity(in, TcpBlockSize32);	
		emit tcpPartialSecurityReady();
		break;
	case VETPCP_TCP_FULL_REALTIME:
		tcpReceiveFullRealTime(in);
		break;
	case VETPCP_TCP_CURRENTLY_UNAVAILABLE_REALTIME:
		break;
	case VETPCP_TCP_PARTIAL_REALTIME:
		break;
	case VETPCP_TCP_REAL_TIME_TICK:
		tcpReceiveTick(in);
		break;	
	case VETPCP_TCP_FULL_NEGOTIABLE_LIST:
		tcpReceiveFullSecurityList(in);
		break;
	case VETPCP_TCP_PARTIAL_NEGOTIABLE_LIST:
		tcpReceivePartialSecurityList(in);
		break;
	case VETPCP_TCP_NEGOTIABLE_UP_TO_DATE:
		emit negotiableListReady();		
		break;
	}

	TcpReceivingPacketType = VETPCP_INVALID;

	if (TcpSocket->bytesAvailable() > 0)
		receivingNextPacket = true;

	TcpBlockSize32 = 0;
}
//-----------------------------------------------------------------------------
void VizCTP::tcpReceiveData()
{
	bool receivingNextPacket = true;
	QDataStream in(TcpSocket);
	in.setVersion(QDataStream::Qt_4_0);
	
	while (receivingNextPacket)
	{
		tcpManagePacket(in, receivingNextPacket);
	}		
}
//-----------------------------------------------------------------------------
void VizCTP::requestFullRealTimePeriod(const QString& symbol)
{
	if (!onlineAndAuthenticate())
		return;

	VizCTPPacket packet(VETPSP_REQUEST_FULL_REALTIME);

	packet << symbol;

	if (SslSocket->isEncrypted())
		SslSocket->write(packet.getPacketReady());	
}
//-----------------------------------------------------------------------------
void VizCTP::requestPartialSecurity(const QString& securityId, const QDate& date)
{
	if (!onlineAndAuthenticate())
		return;

	VizCTPPacket packet(VETPSP_REQUEST_PARTIAL_HISTO);
	
	packet << securityId;
	packet << date;
	
	if (SslSocket->isEncrypted())
		SslSocket->write(packet.getPacketReady());	
}
//-----------------------------------------------------------------------------
void VizCTP::sslReceiveProtocolVersion(QDataStream& in)
{
	QString protocolVersion;
	in >> protocolVersion;

	if (protocolVersion != VizTPProtocolVersion)
	{
		emit error(DIFFERENT_PROTOCOL_VERSION_ERROR);

		disconnectFromVizServer();
	}
}
//-----------------------------------------------------------------------------
void VizCTP::sslManagePacket(QDataStream& in, bool& receivingNextPacket)
{
	receivingNextPacket = false;

	if (SslSocket->isEncrypted())
	{	
		if (SslBlockSize32 == 0)
		{
			if (SslSocket->bytesAvailable() < (int)sizeof(quint32))
				return;

			in >> SslBlockSize32;
		}	

		if (SslSocket->bytesAvailable() < SslBlockSize32)
			return;

		VizETPClientPacket packetType;
		
		in >> packetType;

		switch (packetType)
		{
		case VETPCP_INVALID:
			break;
		case VETPCP_SSL_AUTHENTICATION_OK:
			requestSessionId();			
			break;
		case VETPCP_SSL_ERROR_AUTHENTICATION:
			setState(CERTIFIED_STATE);
			emit error(AUTHENTICATION_ERROR);
			break;
		case VETPCP_SSL_SESSIONID:
			sslReceiveSessionId(in);
			break;
		case VETPCP_SSL_ERROR_UNENCRYPTED_SESSIONID:
			requestSessionId();
			break;
		case VETPCP_SSL_UNENCRYPTED_SESSIONID_OK:
			TcpAuthenticationPassed = true;			
			setState(CONNECTED_STATE);
			emit connected();	
			
			//////////////////////////////////////////////////////////////////////////
			// TEMP
			//////////////////////////////////////////////////////////////////////////
			requestFullRealTimePeriod(QString("PETR3"));

			break;
		case VETPCP_SSL_READY_FOR_SESSIONID:
			authenticateTcpConnection();
			break;
		case VETPCP_SSL_PROTOCOL_VERSION:
			sslReceiveProtocolVersion(in);
			break;
		}

		if (SslSocket->bytesAvailable() > 0)
			receivingNextPacket = true;

		SslBlockSize32 = 0;
	}
}
//-----------------------------------------------------------------------------
void VizCTP::sslReceiveData()
{
	bool receivingNextPacket = true;

	QDataStream in(SslSocket);
	in.setVersion(QDataStream::Qt_4_0);

	while (receivingNextPacket)
	{
		sslManagePacket(in, receivingNextPacket);
	}	
}
//-----------------------------------------------------------------------------
bool VizCTP::isCertificateValid()
{
	return CertificateValid;
}
//-----------------------------------------------------------------------------
void VizCTP::authenticateUser()
{
	setState(AUTHENTICATING_USER_STATE);

	VizCTPPacket packet(VETPSP_AUTHENTICATION);
	
	packet << User;
	packet << Md5Password;
	
	if (SslSocket->isEncrypted())
		SslSocket->write(packet.getPacketReady());
	else emit error(AUTHENTICATION_WITHOUT_ENCRYPTION_ERROR);
	
}
//-----------------------------------------------------------------------------
void VizCTP::sslErrors(const QList<QSslError> &errors)
{
	
#ifdef LAN_MODE
	// Modo debug para testes junto do servidor
 	SslSocket->ignoreSslErrors();
 	return;
#endif

	// Nosso certificado é self signed, pois não pagamos a nenhuma CA
	if (errors.size() == 1)
		if (errors.at(0).error() == QSslError::SelfSignedCertificate)
		{
			CertificateValid = true;
			SslSocket->ignoreSslErrors();
			return;
		}

	CertificateValid = false;

	foreach (const QSslError &error, errors)
	{
		qDebug() << error.errorString();
	}

}
//-----------------------------------------------------------------------------
#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug debug, VizCTP::VizTPState state)
{
	switch (state) 
	{
	case VizCTP::UNKNOWN_STATE:
		debug << "VizTPState::UNKNOWN_STATE";
		break;
	case VizCTP::HOST_LOOKUP_STATE:
		debug << "VizTPState::HOST_LOOKUP_STATE";
		break;
	case VizCTP::CONNECTING_STATE:
		debug << "VizTPState::CONNECTING_STATE";
		break;
	case VizCTP::CERTIFIED_STATE:
		debug << "VizTPState::CERTIFIED_STATE";
		break;
	case VizCTP::AUTHENTICATING_USER_STATE:
		debug << "VizTPState::AUTHENTICATING_USER_STATE";
		break;	
	case VizCTP::CLOSING_STATE:		
		debug << "VizTPState::CLOSING_STATE";
		break;
	case VizCTP::UNCONNECTED_STATE:		
		debug << "VizTPState::UNCONNECTED_STATE";
		break;
	case VizCTP::STARTING_SESSION_STATE:
		debug << "VizTPState::STARTING_SESSION_STATE";
		break;		
	case VizCTP::AUTHENTICATING_SESSION_STATE:
		debug << "VizTPState::AUTHENTICATING_SESSION_STATE";
		break;
	case VizCTP::CONNECTED_STATE:
		debug << "VizTPState::CONNECTED_STATE";
		break;				
	case VizCTP::WAIT_TCP_CONNECTION:
		debug << "VizTPState::WAIT_TCP_CONNECTION";
		break;
	}

	return debug;
}
#endif
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
