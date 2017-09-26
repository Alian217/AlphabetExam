
// AlphabetExamDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "mmsystem.h"
#include "AlphabetExam.h"
#include "AlphabetExamDlg.h"
#include "afxdialogex.h"
#include <io.h> 
#include <locale>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/****************************************************
*                模块中的全局变量                    *
****************************************************/
//配置文件管理结构体(文件名拆开管理)
INFO_CONFIG stInfoConfig;
//exe执行文件路径，也是声音和图片的路径
char PathExeFile[MAX_PATH] = {0};
int ilenPathExeFile =0;
//图片和声音的文件名
char Picfilename[MAX_PATH] = {0};
int ilenPicfilename = 0;
char Wavfilename[MAX_PATH] = {0};
int ilenWavfilename = 0;
//备份上一个随机的拼音和音调序号
int backup_pinyin=0;
int backup_yindiao=0;
//音调总数，就是固定4
int SUM_TYPE_YINGDIAO = 4;


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


// CAlphabetExamDlg 对话框




CAlphabetExamDlg::CAlphabetExamDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAlphabetExamDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAlphabetExamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAlphabetExamDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CAlphabetExamDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CAlphabetExamDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CAlphabetExamDlg 消息处理程序

BOOL CAlphabetExamDlg::OnInitDialog()
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	memset( &stInfoConfig, '\0', sizeof(INFO_CONFIG) );
	ReadConfigFile();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CAlphabetExamDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAlphabetExamDlg::OnPaint()
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
HCURSOR CAlphabetExamDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAlphabetExamDlg::OnBnClickedButton1()
{
	int type_pinyin=0;
	int type_yindiao=0;
	int Index = 0;
	char szNo[SIZE_NO] = {0};

	//获取当前执行、图片、声音文件路径
	memset(PathExeFile, '\0', sizeof(PathExeFile));
 	ilenPathExeFile = GetCurrentDirectory( MAX_PATH, PathExeFile );
	if ( ilenPathExeFile < 0 )
	{
		AfxMessageBox(_T("无法获取当前执行文件路径!"));
		//Alian: 考虑使用语音提示
		return;
	}

	//随机获取两个数字，组成XXX_X格式，XXX为字母序号 X为音调
	srand((unsigned)time(NULL));
	while ( true )
	{
		//现随机获取
		type_pinyin = (rand()%(stInfoConfig.iSumCurrent))+1;
		type_yindiao = (rand()%(SUM_TYPE_YINGDIAO))+1;
		//确保前后两次不同
		if ( backup_pinyin!=type_pinyin || backup_yindiao!=type_yindiao  )
		{
			break;
		}
	}
	backup_pinyin = type_pinyin;
	backup_yindiao = type_yindiao;

	//拼一个随机数组合而成的XXX_X
	memset( szNo, '\0', SIZE_NO );
	sprintf( szNo, "%03d_%d",type_pinyin, type_yindiao );

	//用XXX_X来查找配置文件读取到数组里的元素，并获得下标
	Index = FindStructIndex(szNo);

	//定位下标即可获取到图片(bmp)的文件名
	memset(Picfilename, '\0', sizeof(Picfilename));
	sprintf( Picfilename, "%s%s.bmp", stInfoConfig.stFileName[Index].szZiMu, stInfoConfig.stFileName[Index].szNo );
	ilenPicfilename = strlen(Picfilename);

	//定位下标即可获取到声音(wav)的文件名，留给OnBnClickedButton2使用
	memset(Wavfilename, '\0', sizeof(Wavfilename));
	sprintf( Wavfilename, "%s%s.wav", stInfoConfig.stFileName[Index].szZiMu, stInfoConfig.stFileName[Index].szNo );
	ilenWavfilename = strlen(Wavfilename);

	//合成需要显示图片文件路径
	char pPicFileNamePath[MAX_PATH] = {0};
	sprintf( pPicFileNamePath, "%s\\%s",PathExeFile, Picfilename );

	//本按钮作用为随机显示一张拼音图片到显示窗中
	int cx, cy;  
	CImage  image;  
	CRect   rect; 
	char pPath[100] = {0};
	int iCount = 0;

	//根据需要显示图片文件路径载入图片  
	//if( E_FAIL == image.Load("D:\\workspace\\GitHub\\AlphabetExam\\AlphabetExam\\a00000_001_1.bmp") )
	if( E_FAIL == image.Load(pPicFileNamePath) )
	{
		sprintf( Picfilename+ilenPicfilename, "\n找不到以上图片文件!" );
		AfxMessageBox(_T(Picfilename));
		//Alian: 考虑使用语音提示
		return;
	}

	//显示图片
	//获取图片的宽 高度  
	cx  = image.GetWidth();  
	cy  = image.GetHeight();       
	//获取Picture Control控件的大小  
	GetDlgItem(IDC_PIC_DISPLAY)->GetWindowRect(&rect);  
	//将客户区选中到控件表示的矩形区域内  
	ScreenToClient(&rect);  
	//窗口移动到控件表示的区域  
	GetDlgItem(IDC_PIC_DISPLAY)->MoveWindow(rect.left, rect.top, cx, cy, TRUE);  
	CWnd *pWnd = NULL;  
	pWnd    = GetDlgItem(IDC_PIC_DISPLAY);//获取控件句柄 
	pWnd->ShowWindow(TRUE);
	pWnd->GetClientRect(&rect);//获取句柄指向控件区域的大小       
	CDC *pDc = NULL;  
	pDc = pWnd->GetDC();//获取picture的DC      
	image.Draw(pDc->m_hDC, rect);//将图片绘制到picture表示的区域内       
	ReleaseDC(pDc);
}


void CAlphabetExamDlg::OnBnClickedButton2()
{
	//合成对应图片的声音文件路径
	char pWavFileNamePath[MAX_PATH] = {0};
	int ilenWavFilePath =0;
	sprintf( pWavFileNamePath, "%s\\%s",PathExeFile, Wavfilename );
	ilenWavFilePath = strlen(pWavFileNamePath); 
	//过滤只有一个'\'字符
	if ( 1==strlen(pWavFileNamePath) )
	{
		memset( pWavFileNamePath, '\0', MAX_PATH );
	}

	//找到对应的声音文件，播放声音
	//if( 0 == access("D:\\workspace\\GitHub\\AlphabetExam\\AlphabetExam\\a00000_004_1.wav",0))
	if ( 0 == access(pWavFileNamePath,0) )
	{
		//播放声音
		sndPlaySound( pWavFileNamePath, SND_ASYNC );
	}
	else
	{
		//声音文件加载异常提示判别
		if ( 0==strlen(pWavFileNamePath) )
		{
			//路径为空，提示先要选择显示图片
			AfxMessageBox(_T("请先点击\"获取图片\"按钮!"));
			//Alian: 考虑使用语音提示
		} 
		else
		{
			//路径不为空，提示加载声音文件失败
			sprintf( Wavfilename+ilenWavfilename, "\n找不到以上声音文件!" );
			AfxMessageBox(_T(Wavfilename));
			//Alian: 考虑使用语音提示
		}
	}
}

int CAlphabetExamDlg::ReadConfigFile()
{
	//读取配置文件Config.inf，当做当前拼音个数
	FILE *fp = NULL;
	char pGetDateBuf[SIZE_ZI_MU] = {0};
	int iHangNo = 0;
	int Index = 0;

	//打开Config.inf文件
	fp = fopen( "Config.inf", "a+" );
	if( fp == NULL )    //Config.inf 文件打开失败
	{
		return 0;
	}
	else       //Config.inf 文件打开成功
	{
		//读到文件结束符(到达EOF就结束)
		while( !feof(fp) )
		{
			//从文件中读一行字符串
			fgets( pGetDateBuf, SIZE_ZI_MU, fp );
			if( 0 == iHangNo )
			{
				memcpy( stInfoConfig.stFileName[Index].szZiMu, pGetDateBuf, SIZE_ZI_MU );
				stInfoConfig.stFileName[Index].szZiMu[LEN_ZI_MU] = '\0';
				iHangNo = 1;
			}
			else if ( 1 == iHangNo )
			{
				memcpy( stInfoConfig.stFileName[Index].szNo, pGetDateBuf, SIZE_NO );
				stInfoConfig.stFileName[Index].szNo[LEN_NO] = '\0';
				iHangNo = 0;
				Index++;
				if ( 0 == Index%4 )
				{
					stInfoConfig.iSumCurrent++;
				}
			}
			memset( pGetDateBuf, '\0', SIZE_ZI_MU );
		}
	}
	return 0;
}

int CAlphabetExamDlg::FindStructIndex( char cmpFlag[SIZE_NO] )
{
	int iRes = -1;
	int Cnt = 0;
	while ( 0 != memcmp( stInfoConfig.stFileName[Cnt].szNo, cmpFlag, 5 ) )
	{
		Cnt++;
	}
	iRes = Cnt;
	return iRes;
}
