#pragma once
#include "TransitionSphere.h"
// CTSphereDialog dialog

class CTSphereDialog : public CDialog
{
	DECLARE_DYNAMIC(CTSphereDialog)

public:
	CTSphereDialog(CWnd* pParent = NULL);   // standard constructor
	CTSphereDialog(CTransitionSphere *pSphere);
	virtual ~CTSphereDialog();

// Dialog Data
	enum { IDD = IDD_TSPHERE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	CVector3d GetCenter(void);
	double GetRadius(void);
private:
	CTransitionSphere* m_pTSphere;
	CVector3d m_vCenter;
	double m_dRadius;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
