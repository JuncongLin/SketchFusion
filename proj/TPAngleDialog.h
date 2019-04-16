#pragma once


// CTPAngleDialog dialog

class CTPAngleDialog : public CDialog
{
	DECLARE_DYNAMIC(CTPAngleDialog)

public:
	CTPAngleDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTPAngleDialog();

// Dialog Data
	enum { IDD = IDD_TANGLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	double m_dAngle;
};
