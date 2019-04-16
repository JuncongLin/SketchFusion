// TPAngleDialog.cpp : implementation file
//

#include "stdafx.h"
#include "GeometryStudio.h"
#include "TPAngleDialog.h"
#include ".\tpangledialog.h"


// CTPAngleDialog dialog

IMPLEMENT_DYNAMIC(CTPAngleDialog, CDialog)
CTPAngleDialog::CTPAngleDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTPAngleDialog::IDD, pParent)
	, m_dAngle(0)
{
}

CTPAngleDialog::~CTPAngleDialog()
{
}

void CTPAngleDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTPAngleDialog, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CTPAngleDialog message handlers

void CTPAngleDialog::OnBnClickedOk()
{
	CString svalue;
	GetDlgItem(IDC_ANGLE)->GetWindowText(svalue);
	m_dAngle=atof(svalue);
	OnOK();
}
