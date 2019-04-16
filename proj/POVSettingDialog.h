#pragma once

#include "GeometryStudioView.h"
#include "afxwin.h"
// CPOVSettingDialog dialog
typedef struct{
	bool bOutput;
	bool bBound;
	bool bBodyDir;
	bool bBoundDir;
	bool bObjDir;
	CString pigment;
	CString finish;
	CString interior;
}POVProperty;
class CPOVSettingDialog : public CDialog
{
	DECLARE_DYNAMIC(CPOVSettingDialog)

public:
	CPOVSettingDialog(CWnd* pParent = NULL);   // standard constructor
	CPOVSettingDialog(CGeometryStudioView* pView);   // standard constructor
	virtual ~CPOVSettingDialog();

// Dialog Data
	enum { IDD = IDD_POV_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CGeometryStudioView* m_pGeomView;
private:
	CComboBox m_CtrlPrimitives;
	vector<POVProperty*> m_PPOVProperties;
	POVProperty* m_pCurPPOVProperty;
public:
	CComboBox m_CtrlPigment;
private:
	CComboBox m_CtrlFinish;
	CComboBox m_CtrlInterior;
	CButton m_CtrlOutput;
	CButton m_CtrlBoundary;
	CButton m_CtrlBodyDir;
	CButton m_CtrlBoundDir;
public:
	BOOL Create();
	afx_msg void OnCbnSelchangePrimitives();
	afx_msg void OnBnClickedOutput();
	afx_msg void OnBnClickedExport();
	void SetView(CGeometryStudioView* pView);
	afx_msg void OnClose();
private:
	CComboBox m_CtrlScene;
	CButton m_CtrlObjectDir;
};
