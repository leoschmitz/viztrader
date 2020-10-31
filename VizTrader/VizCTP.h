//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCTPH__
#define __VIZCTPH__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include "VizCSecurityPeriodsManipulator.h"
#include "VizCCompany.h"
#include "VizCTPPacket.h"

#include <QtNetwork/QTcpSocket> 
#include <QtNetwork/QTcpServer> 
#include <QtNetwork/QSslSocket>
#include <QtNetwork/QSslCipher>
#include <QtNetwork/QNetworkProxy>
#include <QString>
#include <QPair>
//-----------------------------------------------------------------------------
namespace viz
{
namespace net
{	
//-----------------------------------------------------------------------------
class VizCTP : public QObject
{	 
	Q_OBJECT
public: 
	enum VizTPError 
	{
		UNKNOWN_ERROR = -1,
		AUTHENTICATION_ERROR = 0,
		ACCOUNT_EXPIRED_ERROR = 1,
		CERTIFICATE_INVALID_ERROR = 2,
		DIFFERENT_PROTOCOL_VERSION_ERROR = 3,
		AUTHENTICATION_WITHOUT_ENCRYPTION_ERROR = 4,
		SOCKET_ERROR = 5,
		CONNECTION_REFUSED_ERROR = 6,
		HOST_NOT_FOUND_ERROR = 7,
		SOCKET_TIMEOUT_ERROR = 8,
		NETWORK_ERROR = 9,
		PROXY_AUTHENTICATION_REQUIRED_ERROR = 10,
		SSL_HANDSHAKE_ERROR = 11,
		PROXY_CONNECTION_REFUSED_ERROR = 12,
		PROXY_CONNECTION_CLOSED_ERROR = 13,
		PROXY_CONNECTION_TIMEOUT_ERROR = 14,
		PROXY_NOT_FOUND_ERROR = 15,
		PROXY_PROTOCOL_ERROR = 16,
		DEFAULT_ERROR = UNKNOWN_ERROR
	};

	enum VizTPState
	{
		UNKNOWN_STATE = -1,
		UNCONNECTED_STATE = 0,
		HOST_LOOKUP_STATE = 1,
		CONNECTING_STATE = 2,
		CLOSING_STATE = 3,
		CERTIFIED_STATE = 4,
		AUTHENTICATING_USER_STATE = 5,
		STARTING_SESSION_STATE = 6,
		AUTHENTICATING_SESSION_STATE = 7,
		CONNECTED_STATE = 8,
		WAIT_TCP_CONNECTION = 9,
		DEFAULT_STATE = UNCONNECTED_STATE	
	};

	static VizCTP* instance();

	bool setProxy(const QNetworkProxy& networkProxy);	
	bool onlineAndAuthenticate();		
	
	void getCertificate();
	void connectToVizServer(const QString& user, const QString& md5Password);	
	void disconnectFromVizServer();	
	VizTPState state();
	void requestSecurity(const QString& securityId, const QDate& lastDate = QDate());
	void requestNegotiableList(const QDate& date = QDate());
	
	bool isCertificateValid();
	QList<QSslCertificate> chainOfCertificates();
	QList<QSslCipher> ciphers();

	QVector<QStringList> NegotiableListElementsToAdd;
	QVector<QString> NegotiableListElementsToRemove;

	QList<QPair<core::VizCCompany, s32> > RequestedCompanies;
private slots:
	void manageSocketError(QAbstractSocket::SocketError socketError);
	void manageLocalError(VizCTP::VizTPError localError);

	void sslSocketStateChanged(QAbstractSocket::SocketState state);
	void tcpSocketStateChanged(QAbstractSocket::SocketState state);
	void sslSocketEncrypted();
	void sslReceiveData();	
	void sslErrors(const QList<QSslError> &errors);	
	void sslDisconnected();

	void tcpReceiveData();
private:
	VizTPState CurrentState;
	VizETPClientPacket TcpReceivingPacketType;

	QString User;
	QString Md5Password;
		
	bool TcpAuthenticationPassed;
	bool CertificateValid;

	QSslSocket* SslSocket;
	QTcpSocket* TcpSocket;
	QString SessionId;

	quint32 SslBlockSize32;
	quint32 TcpBlockSize32;
	s32 OpenSockets;

	void authenticateUser();
	void authenticateTcpConnection();
	void sslConnect();
	void tcpConnect();
	void closeConnection();
	void manageUnavailableSecurity(QDataStream& in);

	void sslManagePacket(QDataStream& in, bool& receivingNextPacket);
	void tcpManagePacket(QDataStream& in, bool& receivingNextPacket);
	void tcpReceiveSecurity(QDataStream& in, const s32& blockSize);
	void tcpReceiveFullSecurityList(QDataStream& in);
	void tcpReceivePartialSecurityList(QDataStream& in);
	void sslReceiveSessionId(QDataStream& in);
	void requestSessionId();	
	void sslReceiveProtocolVersion(QDataStream& in);
	void tcpReceiveFullRealTime(QDataStream& in);
	void tcpReceiveTick(QDataStream& in);
	
	void requestPartialSecurity(const QString& securityId, const QDate& lastDate);
	void requestHistoricData(const QString securityId);	
	void requestFullRealTimePeriod(const QString& symbol);

	void setState(VizTPState newState);

	static VizCTP* Instance;
	VizCTP(QObject* parent = 0);
	~VizCTP();

signals:	
	// Esses sinais são do protocolo e não dos sockets
	void connected();
	void disconnected();
	void stateChanged(VizTPState currentState);
	// não é exatamente por cento mas por unidade, já que vai de 0 a 1
	void percentageReady(VizETPClientPacket packetType, f32 perUnit);

	void securityUnavailable(QString symbol);
	void certificateReady(bool isValid);
	void error(VizCTP::VizTPError error);
		
	void tcpFullSecurityReady();
	void tcpPartialSecurityReady();
	
	void negotiableListReady();
	void syncronizingNegotiableList();
};
//-----------------------------------------------------------------------------
#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug, VizCTP::VizTPState);
#endif
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif