#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "Thread.h"

// CVideoDialog dialog

class CVideoDialog : public CDialog
{
	DECLARE_DYNAMIC(CVideoDialog)

public:
	CVideoDialog();   // standard constructor
	virtual ~CVideoDialog();

// Dialog Data
	enum { IDD = IDD_VIDEO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL Create();
	afx_msg void OnClose();
	afx_msg void OnBnClickedPlay();
	afx_msg void OnBnClickedStop();
private:
	CButton m_btnPlay;
	CButton m_btnStop;
public:
	afx_msg void OnBnClickedTracking();
private:
	bool m_bTracking;
	bool m_bPlay;
	CButton m_btnTracking;
	bool m_bOutputVideo;
public:
	afx_msg void OnBnClickedPath();
private:
	CString m_strFilePath;
	CEdit m_edtFilePath;
	CEdit m_edtFrameRate;
public:
	afx_msg void OnBnClickedVideo();
	CSpinButtonCtrl m_spnAdjustFMRate;
	afx_msg void OnEnChangeFilename();
	afx_msg void OnDeltaposAdjustfmrate(NMHDR *pNMHDR, LRESULT *pResult);
private:
	CEdit m_edtFileName;
public:
	afx_msg void OnBnClickedImage();
private:
	CButton m_btnPath;
	CString m_strFileName;
	int m_iFrameRate;
public:
	afx_msg void OnEnChangeFilepath();
private:
	CThread *m_pThread;
public:
	static void* CaptureScene(void* arg);
	afx_msg void OnBnClickedStatistics();
};
