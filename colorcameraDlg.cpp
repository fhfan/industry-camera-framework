
// colorcameraDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "colorcamera.h"
#include "colorcameraDlg.h"
#include "afxdialogex.h"
#include <Windows.h>
#include <ctime>
#include <vector>
#include <string>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CcolorcameraDlg 对话框



CcolorcameraDlg::CcolorcameraDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CcolorcameraDlg::IDD, pParent)
	, delay(0)
	, threslow(0)
	, thresup(0)
	, red(0)
	, green(0)
	, blue(0)
	, time(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CcolorcameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, delay);
	DDX_Text(pDX, IDC_EDIT2, threslow);
	DDX_Text(pDX, IDC_EDIT3, thresup);
	DDX_Text(pDX, IDC_EDIT4, red);
	DDX_Text(pDX, IDC_EDIT5, green);
	DDX_Text(pDX, IDC_EDIT6, blue);
	DDX_Text(pDX, IDC_EDIT7, time);
	DDX_Control(pDX, IDC_CHECK1, save);
}

BEGIN_MESSAGE_MAP(CcolorcameraDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHECK1, &CcolorcameraDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON1, &CcolorcameraDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CcolorcameraDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CcolorcameraDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CcolorcameraDlg 消息处理程序

BOOL CcolorcameraDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	CreateImageWindow();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CcolorcameraDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CcolorcameraDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CcolorcameraDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CcolorcameraDlg::CreateImageWindow()
{
	HTuple HWindowID;
	CRect Rect;
	CWnd* pWnd = GetDlgItem(IDC_PIC);
	HWindowID = (Hlong)pWnd->m_hWnd;//获取父窗口句柄
	pWnd->GetWindowRect(&Rect);
	OpenWindow(0, 0, Rect.Width(), Rect.Height(), HWindowID, "visible", "", &hv_WindowID);
}


Mat HObject2Mat(HObject Hobj)
{
	Mat Image;
	HTuple htCh;

	HString cType;

	ConvertImageType(Hobj, &Hobj, "byte");

	CountChannels(Hobj, &htCh);

	Hlong wid = 0;

	Hlong hgt = 0;

	if (htCh[0].I() == 1)

	{

		HImage hImg(Hobj);

		void *ptr = hImg.GetImagePointer1(&cType, &wid, &hgt);//GetImagePointer1(Hobj, &ptr, &cType, &wid, &hgt);

		int W = (HTuple)wid;

		int H = (HTuple)hgt;

		Image.create(H, W, CV_8UC1);

		unsigned char *pdata = static_cast<unsigned char *>(ptr);

		memcpy(Image.data, pdata, W*H);

	}

	else if (htCh[0].I() == 3)

	{

		void *Rptr;

		void *Gptr;

		void *Bptr;

		HImage hImg(Hobj);

		hImg.GetImagePointer3(&Rptr, &Gptr, &Bptr, &cType, &wid, &hgt);

		int W = (HTuple)wid;

		int H = (HTuple)hgt;

		Image.create(H, W, CV_8UC3);

		vector<cv::Mat> VecM(3);

		VecM[0].create(H, W, CV_8UC1);

		VecM[1].create(H, W, CV_8UC1);

		VecM[2].create(H, W, CV_8UC1);

		unsigned char *R = (unsigned char *)Rptr;

		unsigned char *G = (unsigned char *)Gptr;

		unsigned char *B = (unsigned char *)Bptr;

		memcpy(VecM[2].data, R, W*H);

		memcpy(VecM[1].data, G, W*H);

		memcpy(VecM[0].data, B, W*H);

		cv::merge(VecM, Image);

	}
	return Image;
}


HObject Mat2HObject(const Mat &image)
{
	HObject Hobj = HObject();
	int hgt = image.rows;
	int wid = image.cols;
	int i;
	//  CV_8UC3  
	if (image.type() == CV_8UC3)
	{
		vector<cv::Mat> imgchannel;
		split(image, imgchannel);
		cv::Mat imgB = imgchannel[0];
		cv::Mat imgG = imgchannel[1];
		cv::Mat imgR = imgchannel[2];
		uchar* dataR = new uchar[hgt*wid];
		uchar* dataG = new uchar[hgt*wid];
		uchar* dataB = new uchar[hgt*wid];
		for (i = 0; i < hgt; i++)
		{
			memcpy(dataR + wid * i, imgR.data + imgR.step*i, wid);
			memcpy(dataG + wid * i, imgG.data + imgG.step*i, wid);
			memcpy(dataB + wid * i, imgB.data + imgB.step*i, wid);
		}
		GenImage3(&Hobj, "byte", wid, hgt, (Hlong)dataR, (Hlong)dataG, (Hlong)dataB);
		delete[]dataR;
		delete[]dataG;
		delete[]dataB;
		dataR = NULL;
		dataG = NULL;
		dataB = NULL;
	}
	//  CV_8UC1  
	else if (image.type() == CV_8UC1)
	{
		uchar* data = new uchar[hgt*wid];
		for (i = 0; i < hgt; i++)
			memcpy(data + wid * i, image.data + image.step*i, wid);
		GenImage1(&Hobj, "byte", wid, hgt, (Hlong)data);
		delete[] data;
		data = NULL;
	}
	return Hobj;
}


Mat label(HObject ho_Image, int upvalue, int lowvalue, int b, int g, int r)
{
	Mat srcImage, colImage;
	int j, k;
	Mat grayImage;
	vector<Mat> channels(3);
	srcImage = HObject2Mat(ho_Image);
	if (srcImage.channels() == 1)
	{
		for (j = 0; j < 3; j++)
			channels[j] = srcImage;
		merge(channels, colImage);
		//channels.clear();
		for (j = 0; j < srcImage.rows; j++)
		{
			for (k = 0; k < srcImage.cols; k++)
			{
				if (srcImage.at<uchar>(j, k)>upvalue || srcImage.at<uchar>(j, k)<lowvalue)
				{
					colImage.at<Vec3b>(j, k)[0] = b;
					colImage.at<Vec3b>(j, k)[1] = g;
					colImage.at<Vec3b>(j, k)[2] = r;
				}

			}
		}
		return colImage;
	}
	else
	{
		cvtColor(srcImage, grayImage, CV_BGR2GRAY);
		for (j = 0; j < grayImage.rows; j++)
		{
			for (k = 0; k < grayImage.cols; k++)
			{
				if (grayImage.at<uchar>(j, k)>upvalue || grayImage.at<uchar>(j, k)<lowvalue)
				{
					srcImage.at<Vec3b>(j, k)[0] = b;
					srcImage.at<Vec3b>(j, k)[1] = g;
					srcImage.at<Vec3b>(j, k)[2] = r;
				}
			}
		}
		return srcImage;
	}
}


void CcolorcameraDlg::OnBnClickedCheck1()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
}

HTuple hv_k = 1;
void CcolorcameraDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	AfxBeginThread(StartCameraTest, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
}


UINT CcolorcameraDlg::StartCameraTest(LPVOID pParam)
{
	// Local control variables
	HTuple hv_AcqHandle, end_val36, step_val36;
	clock_t st, et;
	OpenFramegrabber("HMV3rdParty", 0, 0, 0, 0, 0, 0, "progressive", -1, "default", -1, "false", "default", "DahuaTechnology:5M03DF0PAK00003", 0, -1, &hv_AcqHandle);
	//SetFramegrabberParam(hv_AcqHandle, "TriggerMode", "Off") Face;
	CcolorcameraDlg *pDlg = (CcolorcameraDlg*)pParam;
	pDlg->SendMessage(WM_UPDATEDATA, TRUE);
	/*相机参数
	SetFramegrabberParam(hv_AcqHandle, "ExposureTime", pDlg->exp);
	SetFramegrabberParam(hv_AcqHandle, "GainRaw", pDlg->gain);
	SetFramegrabberParam(hv_AcqHandle, "Gamma", pDlg->gamma);*/
	//GrabImageStart(hv_AcqHandle, -1);
	//ClearWindow(pDlg->hv_WindowID);
	SetFramegrabberParam(hv_AcqHandle, "grab_timeout", -1);
	GrabImage(&pDlg->ho_Image, hv_AcqHandle);   //同步拉流
	//GetImagePointer1(pDlg->ho_Image, NULL, NULL, &m_ImageWidth, &m_ImageHeight);     //和加载静态图像相同
	Mat m_Image = HObject2Mat(pDlg->ho_Image);
	SetPart(pDlg->hv_WindowID, 0, 0, m_Image.rows - 1, m_Image.cols - 1);
	pDlg->StartImageState = true;
	while (pDlg->StartImageState)
	{
		GrabImage(&pDlg->ho_Image, hv_AcqHandle);
		st = clock();
		//显示图像
		DispObj(pDlg->ho_Image = Mat2HObject(label(pDlg->ho_Image, pDlg->thresup, pDlg->threslow, pDlg->blue, pDlg->green, pDlg->red)), pDlg->hv_WindowID);
		if (pDlg->save.GetCheck() == 1)
		{
			WriteImage(pDlg->ho_Image, "jpg", 0, "image/" + hv_k);
			++hv_k;
		}
		et = clock();
		pDlg->time = et - st;
		pDlg->SendMessage(WM_UPDATEDATA, FALSE);
		Sleep(pDlg->delay);
	}
	CloseFramegrabber(hv_AcqHandle); // 结束线程
	return 0;
}




void CcolorcameraDlg::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
	StartImageState = false;
}




void CcolorcameraDlg::OnBnClickedButton3()
{
	// TODO:  在此添加控件通知处理程序代码
	StartImageState = false;
	AfxGetMainWnd()->SendMessage(WM_CLOSE);
}
