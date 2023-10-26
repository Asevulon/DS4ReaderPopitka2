
// MyDlg.h: файл заголовка
//

#pragma once
#include"Reader.h"

#define MS_GET_REPORT WM_USER + 1

// Диалоговое окно MyDlg
class MyDlg : public CDialogEx
{
// Создание
public:
	MyDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DS4READERPOPITKA2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	LRESULT MyDlg::OnGetReport(WPARAM wParam, LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	Reader reader;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	UINT_PTR timerid;
	CListBox LB;
};
