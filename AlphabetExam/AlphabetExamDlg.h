
// AlphabetExamDlg.h : ͷ�ļ�
//

#pragma once

//����Ŀ��IC��������
#define  SIZE_ZI_MU         10    //��ĸ�洢���ܳ�
#define  LEN_ZI_MU          7     //��ĸ������  a00000_'\0'
#define  SIZE_NO            10    //��Ŵ洢���ܳ�
#define  LEN_NO             5     //��Ž�����  005_1'\0'
#define  SUM_DUYIN          999 

typedef struct  
{
	char szZiMu[SIZE_ZI_MU];        //��ĸ������
	char szNo[SIZE_NO];             //��ţ�Ҳ�������
}FILE_NAME,*P_FILE_NAME;

typedef struct  
{
	int iSumCurrent;
	FILE_NAME stFileName[SUM_DUYIN];
}INFO_CONFIG,*P_INFO_CONFIG;

// CAlphabetExamDlg �Ի���
class CAlphabetExamDlg : public CDialogEx
{
// ����
public:
	CAlphabetExamDlg(CWnd* pParent = NULL);	// ��׼���캯��
	int ReadConfigFile();
	int FindStructIndex( char cmpFlag[SIZE_NO] );

// �Ի�������
	enum { IDD = IDD_ALPHABETEXAM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
