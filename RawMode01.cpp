// RawMode01.cpp : implementation file
//
// (c) 1996-99 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com
//

#include "RawMode01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRawMode01 property page

IMPLEMENT_DYNCREATE(CRawMode01, CPropertyPage)

CRawMode01::CRawMode01() : CPropertyPage(CRawMode01::IDD)
{
	//{{AFX_DATA_INIT(CRawMode01)
	//}}AFX_DATA_INIT

	m_pSupervisor = NULL;
}

CRawMode01::~CRawMode01()
{
}

void CRawMode01::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRawMode01)
	DDX_Control(pDX, IDC_DB07, m_db07);
	DDX_Control(pDX, IDC_DB06, m_db06);
	DDX_Control(pDX, IDC_DB05, m_db05);
	DDX_Control(pDX, IDC_DB04, m_db04);
	DDX_Control(pDX, IDC_DB03, m_db03);
	DDX_Control(pDX, IDC_DB02, m_db02);
	DDX_Control(pDX, IDC_DB01, m_db01);
	DDX_Control(pDX, IDC_DB00, m_db00);
	//}}AFX_DATA_MAP

	//Updates the dialog.
	if (m_pSupervisor != NULL) {
		Refresh(m_pSupervisor->GetEcuData());
	}
}

BEGIN_MESSAGE_MAP(CRawMode01, CPropertyPage)
	//{{AFX_MSG_MAP(CRawMode01)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRawMode01 message handlers
// Updates all of our controls
void CRawMode01::Refresh(const CEcuData* const ecuData)
{
	CString buf;

	unsigned char ucF001[65] = { 0 };

	ecuData->copyFromF001(ucF001, sizeof(ucF001));

	buf.Format("%02X       %02X       %02X       %02X       %02X       %02X       %02X       %02X",
		ucF001[1],
		ucF001[2],
		ucF001[3],
		ucF001[4],
		ucF001[5],
		ucF001[6],
		ucF001[7],
		ucF001[8]);
	m_db00.SetWindowText(buf);

	buf.Format("%02X       %02X       %02X       %02X       %02X       %02X       %02X       %02X",
		ucF001[9],
		ucF001[10],
		ucF001[11],
		ucF001[12],
		ucF001[13],
		ucF001[14],
		ucF001[15],
		ucF001[16]);
	m_db01.SetWindowText(buf);

	buf.Format("%02X       %02X       %02X       %02X       %02X       %02X       %02X       %02X",
		ucF001[17],
		ucF001[18],
		ucF001[19],
		ucF001[20],
		ucF001[21],
		ucF001[22],
		ucF001[23],
		ucF001[24]);
	m_db02.SetWindowText(buf);

	buf.Format("%02X       %02X       %02X       %02X       %02X       %02X       %02X       %02X",
		ucF001[25],
		ucF001[26],
		ucF001[27],
		ucF001[28],
		ucF001[29],
		ucF001[30],
		ucF001[31],
		ucF001[32]);
	m_db03.SetWindowText(buf);

	buf.Format("%02X       %02X       %02X       %02X       %02X       %02X       %02X       %02X",
		ucF001[33],
		ucF001[34],
		ucF001[35],
		ucF001[36],
		ucF001[37],
		ucF001[38],
		ucF001[39],
		ucF001[40]);
	m_db04.SetWindowText(buf);

	buf.Format("%02X       %02X       %02X       %02X       %02X       %02X       %02X       %02X",
		ucF001[41],
		ucF001[42],
		ucF001[43],
		ucF001[44],
		ucF001[45],
		ucF001[46],
		ucF001[47],
		ucF001[48]);
	m_db05.SetWindowText(buf);

	buf.Format("%02X       %02X       %02X       %02X       %02X       %02X       %02X       %02X",
		ucF001[49],
		ucF001[50],
		ucF001[51],
		ucF001[52],
		ucF001[53],
		ucF001[54],
		ucF001[55],
		ucF001[56]);
	m_db06.SetWindowText(buf);

	buf.Format("%02X       %02X       %02X       %02X       %02X       %02X       %02X       %02X",
		ucF001[57],
		ucF001[58],
		ucF001[59],
		ucF001[60],
		ucF001[61],
		ucF001[62],
		ucF001[63],
		ucF001[64]);
	m_db07.SetWindowText(buf);
}

void CRawMode01::RegisterSupervisor(CSupervisorInterface* const pSupervisor) {
	m_pSupervisor = pSupervisor;
}
