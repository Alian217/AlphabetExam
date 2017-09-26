
// AlphabetExamDlg.h : 头文件
//

#pragma once

//本项目中IC类型总数
#define  SIZE_ZI_MU         10    //字母存储区总长
#define  LEN_ZI_MU          7     //字母截至点  a00000_'\0'
#define  SIZE_NO            10    //序号存储区总长
#define  LEN_NO             5     //序号截至点  005_1'\0'
#define  SUM_DUYIN          999 

typedef struct  
{
	char szZiMu[SIZE_ZI_MU];        //字母缓存区
	char szNo[SIZE_NO];             //序号，也是随机数
}FILE_NAME,*P_FILE_NAME;

typedef struct  
{
	int iSumCurrent;
	FILE_NAME stFileName[SUM_DUYIN];
}INFO_CONFIG,*P_INFO_CONFIG;

// CAlphabetExamDlg 对话框
class CAlphabetExamDlg : public CDialogEx
{
// 构造
public:
	CAlphabetExamDlg(CWnd* pParent = NULL);	// 标准构造函数
	int ReadConfigFile();
	int FindStructIndex( char cmpFlag[SIZE_NO] );

// 对话框数据
	enum { IDD = IDD_ALPHABETEXAM_DIALOG };

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
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
