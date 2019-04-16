// TCylinderDialog.cpp : implementation file
//

#include "stdafx.h"
#include "GeometryStudio.h"
#include "TCylinderDialog.h"
#include ".\tcylinderdialog.h"


// CTCylinderDialog dialog

IMPLEMENT_DYNAMIC(CTCylinderDialog, CDialog)
CTCylinderDialog::CTCylinderDialog(CTransitionCylinder * pTCylinder)
	: CDialog(CTCylinderDialog::IDD, NULL)
	, m_dHeight(0)
	, m_dBRadius(0)
{
	m_pTCylinder=pTCylinder;
}

CTCylinderDialog::~CTCylinderDialog()
{
}

void CTCylinderDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTCylinderDialog, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CTCylinderDialog message handlers

double CTCylinderDialog::GetHeight(void)
{
	return m_dHeight;
}

void CTCylinderDialog::OnBnClickedOk()
{
	CString svalue;
	GetDlgItem(IDC_HEIGHT)->GetWindowText(svalue);
	m_dHeight=atof(svalue);

	GetDlgItem(IDC_RADIUS)->GetWindowText(svalue);
	m_dBRadius=atof(svalue);
	OnOK();
}

BOOL CTCylinderDialog::OnInitDialog()
{
	CDialog::OnInitDialog();



	CString svalue;

	svalue.Format("%f",m_pTCylinder->GetHeight());
	GetDlgItem(IDC_HEIGHT)->SetWindowText(svalue);

	svalue.Format("%f",m_pTCylinder->GetBottomRadius());
	GetDlgItem(IDC_RADIUS)->SetWindowText(svalue);



	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

double CTCylinderDialog::GetBRadius(void)
{
	return m_dBRadius;
}
