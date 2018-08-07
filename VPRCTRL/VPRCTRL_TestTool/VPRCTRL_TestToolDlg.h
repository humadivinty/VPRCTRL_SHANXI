
// VPRCTRL_TestToolDlg.h : 头文件
//

#pragma once



// CVPRCTRL_TestToolDlg 对话框
class CVPRCTRL_TestToolDlg : public CDialogEx
{
// 构造
public:
	CVPRCTRL_TestToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_VPRCTRL_TESTTOOL_DIALOG };

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
    afx_msg void OnBnClickedButtonSetip();
    afx_msg void OnBnClickedButtoninit();
    afx_msg void OnBnClickedButtonRegcallback();
    afx_msg void OnBnClickedButtonGetdevicestate();
    afx_msg void OnBnClickedButtonAdjusttime();
    afx_msg void OnBnClickedButtonQuit();
};
