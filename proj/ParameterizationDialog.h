#pragma once
#include "afxwin.h"
#include "DetailPatch.h"

// CParameterizationDialog dialog

class CParameterizationDialog : public CDialog
{
	DECLARE_DYNAMIC(CParameterizationDialog)

public:
	CParameterizationDialog(CDetailPatch *pPatch);   // standard constructor
	virtual ~CParameterizationDialog();

// Dialog Data
	enum { IDD = IDD_PARADIAG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CStatic m_CtrlParameterization;
	CDetailPatch* m_pPatch;
public:
	afx_msg void OnPaint();
};
