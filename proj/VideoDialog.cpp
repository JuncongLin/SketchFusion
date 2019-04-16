// VideoDialog.cpp : implementation file
//

#include "stdafx.h"
#include "GeometryStudio.h"
#include "VideoDialog.h"
#include ".\videodialog.h"
#include "MainFrm.h"
#include "global.h"
#include "CSObject.h"
#include "ChildFrm.h"
#include "GeometryStudioView.h"
#include "..\OpenGLBasic\EasyOpenGL.h"
#include "..\PoissonShapeRepresentation\SceneCapture.h"
#include "Stopwatch.hpp"
extern CCSObject gCriticalSection;
CCSObject lCriticalSection;
extern GLubyte* gpPixelBuffer;
// CVideoDialog dialog
bool bEncode=true;
typedef struct Argument{
	GLubyte* pPixelBuffer;
	CGeometryStudioView* pView;
	CString fileName;
}ARGUMENT;

IMPLEMENT_DYNAMIC(CVideoDialog, CDialog)
CVideoDialog::CVideoDialog()
: m_bTracking(false)
, m_bPlay(false)
, m_bOutputVideo(false)
, m_strFilePath(_T("D:\\"))
, m_strFileName(_T("Scene"))
, m_iFrameRate(20)
{
}

CVideoDialog::~CVideoDialog()
{
}

void CVideoDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLAY, m_btnPlay);
	DDX_Control(pDX, IDC_STOP, m_btnStop);
	DDX_Control(pDX, IDC_TRACKING, m_btnTracking);
	DDX_Control(pDX, IDC_FILEPATH, m_edtFilePath);
	DDX_Control(pDX, IDC_FRAMERATE, m_edtFrameRate);
	DDX_Control(pDX, IDC_SPIN1, m_spnAdjustFMRate);
	DDX_Control(pDX, IDC_FILENAME, m_edtFileName);
	DDX_Control(pDX, IDC_PATH, m_btnPath);
}


BEGIN_MESSAGE_MAP(CVideoDialog, CDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_PLAY, OnBnClickedPlay)
	ON_BN_CLICKED(IDC_STOP, OnBnClickedStop)
	ON_BN_CLICKED(IDC_TRACKING, OnBnClickedTracking)
	ON_BN_CLICKED(IDC_PATH, OnBnClickedPath)
	ON_BN_CLICKED(IDC_VIDEO, OnBnClickedVideo)
	ON_EN_CHANGE(IDC_FILENAME, OnEnChangeFilename)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ADJUSTFMRATE, OnDeltaposAdjustfmrate)
	ON_BN_CLICKED(IDC_IMAGE, OnBnClickedImage)
	ON_EN_CHANGE(IDC_FILEPATH, OnEnChangeFilepath)
	ON_BN_CLICKED(IDC_STATISTICS, OnBnClickedStatistics)
END_MESSAGE_MAP()


// CVideoDialog message handlers

BOOL CVideoDialog::Create()
{
	if(!CDialog::Create(CVideoDialog::IDD))return false;
	m_btnStop.EnableWindow(m_bPlay);
	m_btnPlay.EnableWindow(!m_bPlay);
	m_btnTracking.EnableWindow(!m_bPlay);
	m_btnPath.EnableWindow(!m_bPlay);
	m_edtFilePath.EnableWindow(!m_bPlay);
	m_edtFrameRate.EnableWindow(!m_bPlay);
	m_spnAdjustFMRate.EnableWindow(!m_bPlay);
	m_edtFileName.EnableWindow(!m_bPlay);

	CButton *p;
	p=(CButton*)GetDlgItem(IDC_IMAGE);   
	p->SetCheck(!m_bOutputVideo); 
	p->EnableWindow(!m_bPlay);
	p=(CButton*)GetDlgItem(IDC_VIDEO);   
	p->SetCheck(m_bOutputVideo); 
	p->EnableWindow(!m_bPlay);

	m_btnTracking.SetCheck(m_bTracking);

	m_edtFileName.SetWindowText(m_strFileName);
	m_edtFilePath.SetWindowText(m_strFilePath);
	CString str;
	str.Format("%d",m_iFrameRate);
	m_edtFrameRate.SetWindowText(str);

	return true;
}

void CVideoDialog::OnClose()
{
	CMainFrame *pFrameWnd = (CMainFrame*)AfxGetMainWnd();
	pFrameWnd->OnViewVideo();

}

void CVideoDialog::OnBnClickedPlay()
{

	CMainFrame *pFrameWnd = (CMainFrame*)AfxGetMainWnd();
	CChildFrame *pChildFrame=(CChildFrame*)pFrameWnd->GetActiveFrame();
	CView   *pView  = pChildFrame->GetActiveView();   
	if(pView->IsKindOf(RUNTIME_CLASS(CGeometryStudioView)))
	{
		ARGUMENT * argument=new ARGUMENT;

		argument->pPixelBuffer=gpPixelBuffer;
		argument->fileName=m_strFilePath+"\\"+m_strFileName;
		argument->pView=((CGeometryStudioView*)pView);
		//CaptureScene(argument);
		m_pThread=new CThread(CaptureScene,argument,0);

		//CString fullFileName;
		//fullFileName.Format("%s-111.bmp",argument->fileName);
		//CSceneCapture ::SaveAsBMP(fullFileName.GetBuffer (20),*(argument->pEogl),0,0);

		lCriticalSection.Enter();
		bEncode=true;
		lCriticalSection.Leave();

		m_bPlay=true;
		m_btnStop.EnableWindow(m_bPlay);
		m_btnPlay.EnableWindow(!m_bPlay);
		m_btnTracking.EnableWindow(!m_bPlay);
		m_btnPath.EnableWindow(!m_bPlay);
		m_edtFilePath.EnableWindow(!m_bPlay);
		m_edtFrameRate.EnableWindow(!m_bPlay);
		m_spnAdjustFMRate.EnableWindow(!m_bPlay);
		m_edtFileName.EnableWindow(!m_bPlay);
		CButton *p;
		p=(CButton*)GetDlgItem(IDC_IMAGE);   
		p->EnableWindow(!m_bPlay);
		p=(CButton*)GetDlgItem(IDC_VIDEO);   
		p->EnableWindow(!m_bPlay);
	}
	else
		AfxMessageBox("Please switch to GeometryStudioView");
}

void CVideoDialog::OnBnClickedStop()
{
	lCriticalSection.Enter();
	bEncode=false;
	lCriticalSection.Leave();
	m_bPlay=false;
	m_btnStop.EnableWindow(m_bPlay);
	m_btnPlay.EnableWindow(!m_bPlay);
	m_btnTracking.EnableWindow(!m_bPlay);
	m_btnPath.EnableWindow(!m_bPlay);
	m_edtFilePath.EnableWindow(!m_bPlay);
	m_edtFrameRate.EnableWindow(!m_bPlay);
	m_spnAdjustFMRate.EnableWindow(!m_bPlay);
	m_edtFileName.EnableWindow(!m_bPlay);
	CButton *p;
	p=(CButton*)GetDlgItem(IDC_IMAGE);   
	p->EnableWindow(!m_bPlay);
	p=(CButton*)GetDlgItem(IDC_VIDEO);   
	p->EnableWindow(!m_bPlay);
}	

void CVideoDialog::OnBnClickedTracking()
{
	m_bTracking=!m_bTracking;
}

void CVideoDialog::OnBnClickedPath()
{
	char   szDir[MAX_PATH];   
	BROWSEINFO   bi;   
	ITEMIDLIST   *pidl;   

	bi.hwndOwner   =   this->m_hWnd;   
	bi.pidlRoot   =   NULL;   
	bi.pszDisplayName   =   szDir;   
	bi.lpszTitle   =   "Select Directory";   
	bi.ulFlags   =   BIF_RETURNONLYFSDIRS;   
	bi.lpfn   =   NULL;   
	bi.lParam   =   0;   
	bi.iImage   =   0;   

	pidl   =   SHBrowseForFolder(&bi);   
	if(pidl   !=   NULL)
	{
		if(SHGetPathFromIDList(pidl,   szDir))   
		{
			 m_strFilePath=CString(szDir);   
			 m_edtFilePath.SetWindowText(m_strFilePath);
		}
	}


}

void CVideoDialog::OnBnClickedVideo()
{
	m_bOutputVideo=true;
}


void CVideoDialog::OnEnChangeFilename()
{
    m_edtFileName.GetWindowText(m_strFileName);
}

void CVideoDialog::OnDeltaposAdjustfmrate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CVideoDialog::OnBnClickedImage()
{
	m_bOutputVideo=false;

}

void CVideoDialog::OnEnChangeFilepath()
{
	m_edtFilePath.GetWindowText(m_strFilePath);
}

void* CVideoDialog::CaptureScene(void* arg)
{
	CString fullFileName;
	static int i=0;
	while (bEncode)
	{
		i++;
		ARGUMENT* argument=(ARGUMENT*)arg;
		fullFileName.Format("%s-%d.bmp",argument->fileName,i);
		//gCriticalSection.Enter();
		CSceneCapture ::SaveAsBMP(fullFileName.GetBuffer(20),argument->pView->GetDC(),argument->pPixelBuffer,0,0);

		//gCriticalSection.Leave();
	}
	return NULL;
}

void CVideoDialog::OnBnClickedStatistics()
{
	CMainFrame *pFrameWnd = (CMainFrame*)AfxGetMainWnd();
	CChildFrame *pChildFrame=(CChildFrame*)pFrameWnd->GetActiveFrame();
	CView   *pView  = pChildFrame->GetActiveView();   
	float minTime=FLT_MAX,maxTime=-FLT_MAX,avgTime,tmpTime,totTime=0;
	if(pView->IsKindOf(RUNTIME_CLASS(CGeometryStudioView)))
	{
		Stopwatch time;
		for (int i=0;i<100;i++)
		{
			time.Start();
			CSceneCapture ::SaveAsBMP("D:\\Test.bmp",pView->GetDC(),gpPixelBuffer,0,0);
			time.Stop();
			tmpTime=time.GetTime()-totTime;
			if(minTime>tmpTime)minTime=tmpTime;
			if(maxTime<tmpTime)maxTime=tmpTime;
			totTime=time.GetTime();
		}
		avgTime=time.GetAvgTime();
		CString str;
		str.Format("Minimum Time Cost:  %.2f   Maxmimum Frame Rate: %d\nMaxmimum Time Cost: %.2f Minimum Frame Rate:  %d\nAverage Time Cost:  %.2f   Average Frame Rate:  %d",minTime,(int)(1/minTime),maxTime,(int)(1/maxTime),avgTime,(int)(1/avgTime));
		AfxMessageBox(str);
	}
}
