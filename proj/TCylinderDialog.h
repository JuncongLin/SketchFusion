#pragma once
#include "TransitionCylinder.h"

// CTCylinderDialog dialog

class CTCylinderDialog : public CDialog
{
	DECLARE_DYNAMIC(CTCylinderDialog)

public:
	CTCylinderDialog(CTransitionCylinder* pTCylinder);   // standard constructor
	virtual ~CTCylinderDialog();

// Dialog Data
	enum { IDD = IDD_TCYLINDER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CTransitionCylinder* m_pTCylinder;
public:
	double GetHeight(void);
private:
	double m_dHeight;
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	double GetBRadius(void);
private:
	double m_dBRadius;
};
