
// VPRCTRL_TestToolDlg.h : ͷ�ļ�
//

#pragma once



// CVPRCTRL_TestToolDlg �Ի���
class CVPRCTRL_TestToolDlg : public CDialogEx
{
// ����
public:
	CVPRCTRL_TestToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_VPRCTRL_TESTTOOL_DIALOG };

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
    afx_msg void OnBnClickedButtonSetip();
    afx_msg void OnBnClickedButtoninit();
    afx_msg void OnBnClickedButtonRegcallback();
    afx_msg void OnBnClickedButtonGetdevicestate();
    afx_msg void OnBnClickedButtonAdjusttime();
    afx_msg void OnBnClickedButtonQuit();
};
