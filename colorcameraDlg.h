
// colorcameraDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "HalconCpp.h"

#define WM_UPDATEDATA WM_USER+5
using namespace HalconCpp;

// CcolorcameraDlg 对话框
class CcolorcameraDlg : public CDialogEx
{
// 构造
public:
	CcolorcameraDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_COLORCAMERA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	HObject ho_Image;
	HTuple hv_WindowID;
	bool StartImageState;
	void CreateImageWindow();
	int delay;
	afx_msg void OnBnClickedCheck1();
	int threslow;
	int thresup;
	int red;
	int green;
	int blue;
	afx_msg void OnBnClickedButton1();
	static UINT StartCameraTest(LPVOID pParam);
	int time;
	CButton save;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
