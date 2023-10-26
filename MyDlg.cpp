
// MyDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "DS4ReaderPopitka2.h"
#include "MyDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно MyDlg



MyDlg::MyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DS4READERPOPITKA2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void MyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, LB);
}

BEGIN_MESSAGE_MAP(MyDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &MyDlg::OnBnClickedOk)
	ON_MESSAGE(MS_GET_REPORT,&MyDlg::OnGetReport)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// Обработчики сообщений MyDlg

BOOL MyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	reader.InitInstance();
	
	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void MyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR MyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void MyDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	timerid = SetTimer(123, 200, NULL);
}


LRESULT MyDlg::OnGetReport(WPARAM wParam, LPARAM lParam)
{
	LB.SendMessage(WM_SETREDRAW, 0, 0);



	LB.ResetContent();
	DecodedReport dr;
	reader.GetDecodedReport(dr);
	CString str;
	
	str.Format(L"Left Stick X: %d, Y: %d", dr.lsaX, dr.lsaY);
	LB.InsertString(-1, str);
	str.Format(L"Right Stick X: %d, Y: %d", dr.rsaX, dr.rsaY);
	LB.InsertString(-1, str);

	str.Format(L"Battery Level: %d", dr.BatteryLevel);
	LB.InsertString(-1, str);

	str.Format(L"\x25B2: %d, \x25CF: %d, \x2613: %d, \x25A0: %d", dr.triangle, dr.circle, dr.cross, dr.rect);
	LB.InsertString(-1, str);

	str = L"";
	if (dr.N)str += L"\x2191";
	if (dr.W)str += L"\x2190";
	if (dr.S)str += L"\x2193";
	if (dr.E)str += L"\x2192";
	LB.InsertString(-1, str);

	str.Format(L"L1: %d, R1: %d, L2: %d, R2: %d, L3: %d, R3: %d", dr.L1, dr.R1, dr.L2, dr.R2, dr.L3, dr.R3);
	LB.InsertString(-1, str);

	str.Format(L"Options: %d, Share: %d", dr.options, dr.share);
	LB.InsertString(-1, str);

	str.Format(L"T-PAD Click: %d, Home Click: %d", dr.tpadclick, dr.home);
	LB.InsertString(-1, str);

	str.Format(L"L2 Trigger: %d, R2 Trigger: %d", dr.L2trigger, dr.R2trigger);
	LB.InsertString(-1, str);

	str.Format(L"Gyro X: %d, Gyro Y: %d, Gyro Z: %d", dr.GyroX, dr.GyroY, dr.GyroZ);
	LB.InsertString(-1, str);

	str.Format(L"Accel X: %d, Accel Y: %d, Accel Z: %d", dr.AccelX, dr.AccelY, dr.AccelZ);
	LB.InsertString(-1, str);

	LB.InsertString(-1, dr.DeviceDescr);

	LB.InsertString(-1, dr.tpadEvent);

	str.Format(L"Is Finger 1 down: %d, Finger 1 Counter: %d", dr.finger1down, dr.finger1Counter);
	LB.InsertString(-1, str);

	str.Format(L"Finger 1 (X, Y): (%d, %d)", dr.finger1X, dr.finger1Y);
	LB.InsertString(-1, str);

	str.Format(L"Is Finger 2 down: %d, Finger 2 Counter: %d", dr.finger2down, dr.finger2Counter);
	LB.InsertString(-1, str);

	str.Format(L"Finger 2 (X, Y): (%d, %d)", dr.finger2X, dr.finger2Y);
	LB.InsertString(-1, str);

	LB.SendMessage(WM_SETREDRAW, 1, 0);
	return 0;
}


void MyDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	PostMessage(MS_GET_REPORT, NULL, NULL);
	CDialogEx::OnTimer(nIDEvent);
}
