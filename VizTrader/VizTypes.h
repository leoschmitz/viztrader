//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZTYPES_H__
#define __VIZTYPES_H__
//-----------------------------------------------------------------------------
#include <QObject>
#include <QObjectList>
#include <QStringList>
#include <QVariantList>
#include <QVariant>
#include <QMetaProperty>
#include <QMetaProperty>
#include "VizCMemoryLeakReport.h"
//-----------------------------------------------------------------------------
namespace viz
{
//-----------------------------------------------------------------------------
typedef unsigned char		u8; 
typedef signed char			s8; 
typedef char				c8; 
typedef unsigned short		u16;
typedef signed short		s16; 
typedef unsigned int		u32;
typedef signed int			s32;
typedef qlonglong			s64;
typedef qulonglong			u64;
typedef float				f32;
typedef double				f64;
//-----------------------------------------------------------------------------
enum VizEPenWidth
{
	VEPW_1 = 1,
	VEPW_2 = 2,
	VEPW_3 = 3,
	VEPW_4 = 4,
	VEPW_5 = 5,
	VEPW_DEFAULT = VEPW_1
};
//-----------------------------------------------------------------------------
enum VizEPenStyle
{
	VEPP_INVISIBLE = Qt::NoPen,
	VEPP_SOLID_LINE = Qt::SolidLine,
	VEPP_DASH_LINE = Qt::DashLine,
	VEPP_DOT_LINE = Qt::DotLine,
	VEPP_DASH_DOT_LINE = Qt::DashDotLine,
	VEPP_DASH_DOT_DOT_LINE = Qt::DashDotDotLine,
	VEPP_DEFAULT = VEPP_SOLID_LINE
};
//-----------------------------------------------------------------------------
enum VizEScaleInYOptions
{
	VESIYO_MERGE_WITH_SCALE,
	VESIYO_OVERLAY_WITHOUT_SCALE,
	VESIYO_DEFAULT = VESIYO_MERGE_WITH_SCALE
};
//-----------------------------------------------------------------------------
enum VizEIndicatorStyle
{
	VEIS_LINES = 0,
	VEIS_POINTS = 1,
	VEIS_HISTOGRAM = 2,
	VEIS_DEFAULT = VEIS_LINES
};
//-----------------------------------------------------------------------------
enum VizEIndicatorType
{
	VEIT_MOVING_AVERAGE = 0,
	VEIT_RELATIVE_STRENGTH_INDEX,
	VEIT_VOLUME,
	VEIT_ADX,
	VEIT_PLUS_DI,
	VEIT_MINUS_DI,
	VEIT_DEFAULT = VEIT_VOLUME
};
//-----------------------------------------------------------------------------
enum VizEChartType
{
	VECT_BARS = 0,
	VECT_CANDLE_STICKS,
	VECT_HEIKIN_ASHI_CANDLE_STICKS,
	VECT_HEIKIN_ASHI_BARS,
	VECT_CANDLE_VOLUME,
	VECT_KAGI,
	VECT_LINE,
	VECT_POINT_AND_FIGURE,
	VECT_RENKO,
	VECT_THREE_LINE_BREAK
};
/*
enum VizEPrimaryChartType
{
	VEPCT_BARS = 0,
	VEPCT_CANDLE_STICKS,
	VEPCT_HEIKIN_ASHI_CANDLE_STICKS,
	VEPCT_HEIKIN_ASHI_BARS,
	VEPCT_CANDLE_VOLUME,
	VEPCT_KAGI,
	VEPCT_LINE,
	VEPCT_POINT_AND_FIGURE,
	VEPCT_RENKO,
	VEPCT_THREE_LINE_BREAK,
	VEPCT_DEFAULT = VEPCT_CANDLE_STICKS
};
//-----------------------------------------------------------------------------
enum VizESecundaryChartType
{
	VESCT_BARS = 0,
	VESCT_CANDLE_STICKS,
	VESCT_HEIKIN_ASHI_CANDLE_STICKS,
	VESCT_HEIKIN_ASHI_BARS,
	VESCT_LINE,
	VESCT_DEFAULT = VESCT_CANDLE_STICKS
};
*/
//-----------------------------------------------------------------------------
enum VizEPriceField
{
	VEPF_OPEN_PRICE = 0,
	VEPF_CLOSE_PRICE,
	VEPF_MAX_PRICE,
	VEPF_MIN_PRICE,
	VEPF_DEFAULT = VEPF_CLOSE_PRICE
};
//-----------------------------------------------------------------------------
enum VizEMovingAverageMethod
{
	VEMAM_SIMPLE = 0,
	VEMAM_EXPONENTIAL,
	VEMAM_WEIGHTED,
	VEMAM_DOUBLE_EXPONENTIAL,
	VEMAM_TRIPLE_EXPONENTIAL,
	VEMAM_TRIANGULAR,
	VEMAM_KAYFMAN,
	VEMAM_MESA,
	VEMAM_T3,
	VEMAM_DEFAULT = VEMAM_SIMPLE
};
//-----------------------------------------------------------------------------
enum VizESecurityPeriodicity
{
	VESP_DAILY = 0,
	VESP_WEEKLY,
	VESP_MONTHLY,
	VESP_QUARTERLY,
	VESP_YEARLY,
	VESP_DEFAULT = VESP_MONTHLY
};
//-----------------------------------------------------------------------------
// Codigo BDI
enum VizESecurityBDI
{
	VESB_INVALID = -1,					// Valor invalido
	VESB_PADRAO = 2,					// Lote Padrao
	VESB_CONCORDATARIA = 6,				// Concordataria
	VESB_DIREITOS_E_RECIBOS = 10,		// Direitos e recibos
	VESB_FUNDOS_IMOBILIARIOS = 12,		// Fundos imobiliarios
	VESB_CERTIFIC_INVESTIMENTO = 14,	// Certific. investimento / debentures / titulo divida publica
	VESB_OBRIGACOES = 18,				// Obrigacoes
	VESB_BONUS_PRIVADOS = 22,			// Bonus (privados)
	VESB_APOLICES = 26,					// Apolices / bonus / titulos publicos
	VESB_EXERCICIO_COMPRA_INDICE = 32,	// Exercicio de opcoes de compra de indice
	VESB_EXERCICIO_VENDA_INDICE = 33,	// Exercicio de opcoes de venda de indice
	VESB_EXERCICIO_COMPRA = 38,			// Exercicio de opcoes de compra 
	VESB_EXERCICIO_VENDA = 42,			// Exercicio de opcoes de venda 
	VESB_LEILAO_TITULOS_N_COTADOS = 46,	// Leilao de titulos nao cotados
	VESB_LEILAO_PRIVATIZACAO = 48,		// Leilao de privatizacao
	VESB_LEILAO = 50,					// Leilao
	VESB_LEILAO_FINOR = 51,				// Leilao FINOR
	VESB_LEILAO_FINAM = 52,				// Leilao FINAM
	VESB_LEILAO_FISET = 53,				// Leilao FISET
	VESB_LEILAO_ACOES_MORA = 54,		// Leilao de acoes em mora
	VESB_VENDAS_ALVARA_JUDICIAL = 56,	// Vendas por alvara judicial
	VESB_OUTROS = 58,					// Outros
	VESB_PERMUTA = 60,					// Permuta por acoes
	VESB_META = 61,						// Meta
	VESB_TERMO = 62,					// Termo
	VESB_DEBENTUR_VENCIMENTO_ATE3 = 66,	// Debentures com data de vencimento ate 3 anos
	VESB_DEBENTUR_VENCIMENTO_MAI3 = 68, // Debentures com data de vencimento maior que 3 anos
	VESB_FUTURO_MOV_CONTINUA = 70,		// Futuro com movimentacao continua
	VESB_FUTURO_RETENCAO_GANHO = 71,	// Futuro com retencao de ganho
	VESB_OPCOES_COMPRA_INDICE = 74,		// Opcoes de compra de indices
	VESB_OPCOES_VENDA_INDICE = 75,		// Opcoes de venda de indices
	VESB_OPCOES_COMPRA = 78,			// Opcoes de compra
	VESB_OPCOES_VENDA = 82,				// Opcoes de venda
	VESB_DEBENTUR_NOTA_PROMISSORIA = 83,// Debentures e notas promissorias
	VESB_FRACIONARIO = 96,				// Fracionario
	VESB_TOTAL_GERAL = 99				// Total geral
};
//-----------------------------------------------------------------------------
static inline QDataStream& operator<<(QDataStream& out, const VizESecurityBDI& bdi)
{	
	out << static_cast<int>(bdi);
	return out;
}
//-----------------------------------------------------------------------------
static inline QDataStream& operator>>(QDataStream& in, VizESecurityBDI& bdi)
{	
	int ibdi; 
	in >> ibdi;
	bdi = static_cast<VizESecurityBDI>(ibdi);
	return in;
}
//-----------------------------------------------------------------------------
// Tabela de tpmerc - relacao dos valores para tipo de mercado
enum VizEMarketType 
{
	VEMT_INVALID = -1,
	VEMT_VISTA = 10,
	VEMT_EXERCICIO_OPCOES_COMPRA = 12,
	VEMT_EXERCICIO_OPCOES_VENDA = 13,
	VEMT_LEILAO = 17,
	VEMT_FRACIONARIO = 20,
	VEMT_TERMO = 30,
	VEMT_FUTURO_RETENCAO_GANHO = 50,
	VEMT_FUTURO_MOV_CONTINUA = 60,
	VEMT_OPCOES_COMPRA = 70,
	VEMT_OPCOES_VENDA = 80	
};
//-----------------------------------------------------------------------------
static inline QDataStream& operator<<(QDataStream& out, const VizEMarketType& marketType)
{	
	out << static_cast<int>(marketType);
	return out;
}
//-----------------------------------------------------------------------------
static inline QDataStream& operator>>(QDataStream& in, VizEMarketType& marketType)
{	
	int imarkettype;
	in >> imarkettype;
	marketType = static_cast<VizEMarketType>(imarkettype);
	return in;
}
//-----------------------------------------------------------------------------
// Tabela de INDOPC - relacao dos valores para correcao de contratos
enum VizEContractCorrection
{
	VECC_INVALID = -1,
	VECC_USD = 1,				// Correcao pela taxa de dolar
	VECC_TJLP = 2,				// Correcao pela tjlp
	VECC_TR = 3,				// Correcao pelo tr
	VECC_IPCR = 4,				// Correcao pelo ipcr
	VECC_SWA = 5,				// Opcoes de troca - swoptions
	VECC_INDICES = 6,			// Opcoes referenciadas em pontos de indice
	VECC_USD_PROTEGIDA = 7,		// Correcao pela taxa do dolar - opcoes protegidas
	VECC_IGPM_PROTEGIDA = 8,	// Correcao pelo igp-m - opcoes protegidas
	VECC_URV = 9				// Correcao pela URV
};
//-----------------------------------------------------------------------------
static inline QDataStream& operator<<(QDataStream& out, const VizEContractCorrection& v)
{	
	out << static_cast<int>(v);
	return out;
}
//-----------------------------------------------------------------------------
static inline QDataStream& operator>>(QDataStream& in, VizEContractCorrection& v)
{	
	int iv;
	in >> iv;
	v = static_cast<VizEContractCorrection>(iv);
	return in;
}
//-----------------------------------------------------------------------------
enum VizETPServerPacket
{
	VETPSP_INVALID = -1,
	// SSL
	VETPSP_AUTHENTICATION = 1,
	VETPSP_REQUEST_SESSIONID = 2,
	VETPSP_REQUEST_FULL_HISTO = 100,
	VETPSP_REQUEST_PARTIAL_HISTO = 101,
	VETPSP_REQUEST_FULL_REALTIME = 200,
	VETPSP_REQUEST_PARTIAL_REALTIME = 201,
	VETPSP_REQUEST_FULL_NEGOTIABLE_LIST = 300,
	VETPSP_REQUEST_PARTIAL_NEGOTIABLE_LIST = 301,
	// TCP
	VETPSP_UNENCRYPTED_SESSIONID = 1000,
	VETPSP_DEFAULT = VETPSP_INVALID
};
//-----------------------------------------------------------------------------
static inline QDataStream& operator<<(QDataStream& out, const VizETPServerPacket& v)
{	
	out << static_cast<int>(v);
	return out;
}
//-----------------------------------------------------------------------------
static inline QDataStream& operator>>(QDataStream& in, VizETPServerPacket& v)
{	
	int iv;
	in >> iv;
	v = static_cast<VizETPServerPacket>(iv);
	return in;
}
//-----------------------------------------------------------------------------
enum VizETPClientPacket
{
	VETPCP_INVALID = -1,	
	// autenticação
	VETPCP_SSL_AUTHENTICATION_OK = 1,
	VETPCP_SSL_ERROR_AUTHENTICATION = 2,
	VETPCP_SSL_SESSIONID = 3,
	VETPCP_SSL_UNENCRYPTED_SESSIONID_OK = 4,
	VETPCP_SSL_ERROR_UNENCRYPTED_SESSIONID = 5,
	VETPCP_SSL_READY_FOR_SESSIONID = 6,
	VETPCP_SSL_PROTOCOL_VERSION = 7,
	// dados históricos
	VETPCP_TCP_FULL_HISTO = 100,
	VETPCP_TCP_CURRENTLY_UNAVAILABLE_HISTO = 101,	
	VETPCP_TCP_PARTIAL_HISTO = 102,
	VETPCP_TCP_HISTO_UP_TO_DATE = 103,
	VETPCP_TCP_UPDATE_COMPANY_INFO = 104,
	VETPCP_TCP_UPDATE_SECURITY_INFO = 105,
	VETPCP_TCP_REMOVE_COMPANY_HISTO = 106,	
	VETPCP_TCP_REMOVE_SECURITY_HISTO = 107,
	VETPCP_TCP_HISTO_EMPTY = 108,
	// tempo-real
	VETPCP_TCP_FULL_REALTIME = 200,	
	VETPCP_TCP_CURRENTLY_UNAVAILABLE_REALTIME = 201,
	VETPCP_TCP_PARTIAL_REALTIME = 202,
	VETPCP_TCP_REAL_TIME_EMPTY = 203,
	VETPCP_TCP_REAL_TIME_TICK = 204,
	// lista de ativos
	VETPCP_TCP_FULL_NEGOTIABLE_LIST = 300,
	VETPCP_TCP_PARTIAL_NEGOTIABLE_LIST = 301,
	VETPCP_TCP_NEGOTIABLE_UP_TO_DATE = 302,

	VETPCP_DEFAULT = VETPCP_INVALID
};
//-----------------------------------------------------------------------------
static inline QDataStream& operator<<(QDataStream& out, const VizETPClientPacket& v)
{	
	out << static_cast<int>(v);
	return out;
}
//-----------------------------------------------------------------------------
static inline QDataStream& operator>>(QDataStream& in, VizETPClientPacket& v)
{	
	int iv;
	in >> iv;
	v = static_cast<VizETPClientPacket>(iv);
	return in;
}
//-----------------------------------------------------------------------------
enum VizESortOrder
{
	VESO_UNSORTED = 0,
	VESO_ASCENDING_ORDER,
	VESO_DESCENDING_ORDER
};
//-----------------------------------------------------------------------------
enum VizEMarketSegment 
{
	VEMS_INVALID = -1,
	VEMS_CASH = 1,
	VEMS_FORWARD = 2,
	VEMS_FRACTIONAL = 3,
	VEMS_OPTIONS = 4,
	VEMS_AUCTION = 5,
	VEMS_AUCTION_NON_QU0TED = 6,
	VEMS_RESERVED = 7,
	VEMS_FORWARD_IN_POINTS = 8,
	VEMS_OPTION_EXERCISE = 9,
	VEMS_INDEXES = 90,
	VEMS_DEFAULT = VEMS_INVALID
};
//-----------------------------------------------------------------------------
static inline QDataStream& operator<<(QDataStream& out, const VizEMarketSegment& marketType)
{	
	out << static_cast<int>(marketType);
	return out;
}
//-----------------------------------------------------------------------------
static inline QDataStream& operator>>(QDataStream& in, VizEMarketSegment& marketType)
{	
	int imarkettype;
	in >> imarkettype;
	marketType = static_cast<VizEMarketSegment>(imarkettype);
	return in;
}
//-----------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
