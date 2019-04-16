// TSphereDialog.cpp : implementation file
//

#include "stdafx.h"
#include "GeometryStudio.h"
#include "TSphereDialog.h"
#include ".\tspheredialog.h"


// CTSphereDialog dialog

IMPLEMENT_DYNAMIC(CTSphereDialog, CDialog)
CTSphereDialog::CTSphereDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTSphereDialog::IDD, pParent)
{
	m_pTSphere=NULL;
}
CTSphereDialog::CTSphereDialog(CTransitionSphere *pSphere):CDialog(CTSphereDialog::IDD)
{
	m_pTSphere=pSphere;
}
CTSphereDialog::~CTSphereDialog()
{
}

void CTSphereDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTSphereDialog, CDialog)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CTSphereDialog message handlers

CVector3d CTSphereDialog::GetCenter(void)
{
	return m_vCenter;
}

double CTSphereDialog::GetRadius(void)
{
	return m_dRadius;
}

BOOL CTSphereDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	CVector3d cnt=m_pTSphere->GetCenter();

	CString svalue;

	svalue.Format("%f",cnt[0]);
	GetDlgItem(IDC_X)->SetWindowText(svalue);

	svalue.Format("%f",cnt[1]);
	GetDlgItem(IDC_Y)->SetWindowText(svalue);

	svalue.Format("%f",cnt[2]);
	GetDlgItem(IDC_Z)->SetWindowText(svalue);

	svalue.Format("%f",m_pTSphere->GetRadius());
	GetDlgItem(IDC_R)->SetWindowText(svalue);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTSphereDialog::OnBnClickedOk()
{

	double x,y,z;
	CString svalue;
	GetDlgItem(IDC_X)->GetWindowText(svalue);
	x=atof(svalue);
	GetDlgItem(IDC_Y)->GetWindowText(svalue);
	y=atof(svalue);
	GetDlgItem(IDC_Z)->GetWindowText(svalue);
	z=atof(svalue);
	m_vCenter.Set(x,y,z);

	GetDlgItem(IDC_R)->GetWindowText(svalue);
	m_dRadius=atof(svalue);

	OnOK();
}
