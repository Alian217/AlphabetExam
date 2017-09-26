
// AlphabetExamDlg.cpp : ʵ���ļ�
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
*                ģ���е�ȫ�ֱ���                    *
****************************************************/
//�����ļ�����ṹ��(�ļ����𿪹���)
INFO_CONFIG stInfoConfig;
//exeִ���ļ�·����Ҳ��������ͼƬ��·��
char PathExeFile[MAX_PATH] = {0};
int ilenPathExeFile =0;
//ͼƬ���������ļ���
char Picfilename[MAX_PATH] = {0};
int ilenPicfilename = 0;
char Wavfilename[MAX_PATH] = {0};
int ilenWavfilename = 0;
//������һ�������ƴ�����������
int backup_pinyin=0;
int backup_yindiao=0;
//�������������ǹ̶�4
int SUM_TYPE_YINGDIAO = 4;


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CAlphabetExamDlg �Ի���




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


// CAlphabetExamDlg ��Ϣ�������

BOOL CAlphabetExamDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	memset( &stInfoConfig, '\0', sizeof(INFO_CONFIG) );
	ReadConfigFile();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CAlphabetExamDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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

	//��ȡ��ǰִ�С�ͼƬ�������ļ�·��
	memset(PathExeFile, '\0', sizeof(PathExeFile));
 	ilenPathExeFile = GetCurrentDirectory( MAX_PATH, PathExeFile );
	if ( ilenPathExeFile < 0 )
	{
		AfxMessageBox(_T("�޷���ȡ��ǰִ���ļ�·��!"));
		//Alian: ����ʹ��������ʾ
		return;
	}

	//�����ȡ�������֣����XXX_X��ʽ��XXXΪ��ĸ��� XΪ����
	srand((unsigned)time(NULL));
	while ( true )
	{
		//�������ȡ
		type_pinyin = (rand()%(stInfoConfig.iSumCurrent))+1;
		type_yindiao = (rand()%(SUM_TYPE_YINGDIAO))+1;
		//ȷ��ǰ�����β�ͬ
		if ( backup_pinyin!=type_pinyin || backup_yindiao!=type_yindiao  )
		{
			break;
		}
	}
	backup_pinyin = type_pinyin;
	backup_yindiao = type_yindiao;

	//ƴһ���������϶��ɵ�XXX_X
	memset( szNo, '\0', SIZE_NO );
	sprintf( szNo, "%03d_%d",type_pinyin, type_yindiao );

	//��XXX_X�����������ļ���ȡ���������Ԫ�أ�������±�
	Index = FindStructIndex(szNo);

	//��λ�±꼴�ɻ�ȡ��ͼƬ(bmp)���ļ���
	memset(Picfilename, '\0', sizeof(Picfilename));
	sprintf( Picfilename, "%s%s.bmp", stInfoConfig.stFileName[Index].szZiMu, stInfoConfig.stFileName[Index].szNo );
	ilenPicfilename = strlen(Picfilename);

	//��λ�±꼴�ɻ�ȡ������(wav)���ļ���������OnBnClickedButton2ʹ��
	memset(Wavfilename, '\0', sizeof(Wavfilename));
	sprintf( Wavfilename, "%s%s.wav", stInfoConfig.stFileName[Index].szZiMu, stInfoConfig.stFileName[Index].szNo );
	ilenWavfilename = strlen(Wavfilename);

	//�ϳ���Ҫ��ʾͼƬ�ļ�·��
	char pPicFileNamePath[MAX_PATH] = {0};
	sprintf( pPicFileNamePath, "%s\\%s",PathExeFile, Picfilename );

	//����ť����Ϊ�����ʾһ��ƴ��ͼƬ����ʾ����
	int cx, cy;  
	CImage  image;  
	CRect   rect; 
	char pPath[100] = {0};
	int iCount = 0;

	//������Ҫ��ʾͼƬ�ļ�·������ͼƬ  
	//if( E_FAIL == image.Load("D:\\workspace\\GitHub\\AlphabetExam\\AlphabetExam\\a00000_001_1.bmp") )
	if( E_FAIL == image.Load(pPicFileNamePath) )
	{
		sprintf( Picfilename+ilenPicfilename, "\n�Ҳ�������ͼƬ�ļ�!" );
		AfxMessageBox(_T(Picfilename));
		//Alian: ����ʹ��������ʾ
		return;
	}

	//��ʾͼƬ
	//��ȡͼƬ�Ŀ� �߶�  
	cx  = image.GetWidth();  
	cy  = image.GetHeight();       
	//��ȡPicture Control�ؼ��Ĵ�С  
	GetDlgItem(IDC_PIC_DISPLAY)->GetWindowRect(&rect);  
	//���ͻ���ѡ�е��ؼ���ʾ�ľ���������  
	ScreenToClient(&rect);  
	//�����ƶ����ؼ���ʾ������  
	GetDlgItem(IDC_PIC_DISPLAY)->MoveWindow(rect.left, rect.top, cx, cy, TRUE);  
	CWnd *pWnd = NULL;  
	pWnd    = GetDlgItem(IDC_PIC_DISPLAY);//��ȡ�ؼ���� 
	pWnd->ShowWindow(TRUE);
	pWnd->GetClientRect(&rect);//��ȡ���ָ��ؼ�����Ĵ�С       
	CDC *pDc = NULL;  
	pDc = pWnd->GetDC();//��ȡpicture��DC      
	image.Draw(pDc->m_hDC, rect);//��ͼƬ���Ƶ�picture��ʾ��������       
	ReleaseDC(pDc);
}


void CAlphabetExamDlg::OnBnClickedButton2()
{
	//�ϳɶ�ӦͼƬ�������ļ�·��
	char pWavFileNamePath[MAX_PATH] = {0};
	int ilenWavFilePath =0;
	sprintf( pWavFileNamePath, "%s\\%s",PathExeFile, Wavfilename );
	ilenWavFilePath = strlen(pWavFileNamePath); 
	//����ֻ��һ��'\'�ַ�
	if ( 1==strlen(pWavFileNamePath) )
	{
		memset( pWavFileNamePath, '\0', MAX_PATH );
	}

	//�ҵ���Ӧ�������ļ�����������
	//if( 0 == access("D:\\workspace\\GitHub\\AlphabetExam\\AlphabetExam\\a00000_004_1.wav",0))
	if ( 0 == access(pWavFileNamePath,0) )
	{
		//��������
		sndPlaySound( pWavFileNamePath, SND_ASYNC );
	}
	else
	{
		//�����ļ������쳣��ʾ�б�
		if ( 0==strlen(pWavFileNamePath) )
		{
			//·��Ϊ�գ���ʾ��Ҫѡ����ʾͼƬ
			AfxMessageBox(_T("���ȵ��\"��ȡͼƬ\"��ť!"));
			//Alian: ����ʹ��������ʾ
		} 
		else
		{
			//·����Ϊ�գ���ʾ���������ļ�ʧ��
			sprintf( Wavfilename+ilenWavfilename, "\n�Ҳ������������ļ�!" );
			AfxMessageBox(_T(Wavfilename));
			//Alian: ����ʹ��������ʾ
		}
	}
}

int CAlphabetExamDlg::ReadConfigFile()
{
	//��ȡ�����ļ�Config.inf��������ǰƴ������
	FILE *fp = NULL;
	char pGetDateBuf[SIZE_ZI_MU] = {0};
	int iHangNo = 0;
	int Index = 0;

	//��Config.inf�ļ�
	fp = fopen( "Config.inf", "a+" );
	if( fp == NULL )    //Config.inf �ļ���ʧ��
	{
		return 0;
	}
	else       //Config.inf �ļ��򿪳ɹ�
	{
		//�����ļ�������(����EOF�ͽ���)
		while( !feof(fp) )
		{
			//���ļ��ж�һ���ַ���
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
