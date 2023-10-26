#pragma once
#include "libusb.h"

struct DecodedReport
{
	// байты 1 - 4
	//Left stick axis X, 0 = left
	int lsaX = 0;
	//Left Stick axis Y, 0 = up
	int lsaY = 0;
	//Right Stick axis X, 0 = left 
	int rsaX = 0;
	//Right Stick axis Y, 0 = up
	int rsaY = 0;


	//байт 5
	int triangle = 0;
	int circle = 0;
	int cross = 0;
	int rect = 0;

	int N = 0;
	int W = 0;
	int E = 0;
	int S = 0;

	//6
	int R3 = 0;
	int L3 = 0;
	int options = 0;
	int share = 0;
	int R2 = 0;
	int L2 = 0;
	int R1 = 0;
	int L1 = 0;

	//7
	int tpadclick = 0;
	int home = 0;

	//8-9
	int L2trigger = 0;
	int R2trigger = 0;

	//12
	int BatteryLevel = 0;

	//13-18
	INT16 GyroX = 0;
	INT16 GyroY = 0;
	INT16 GyroZ = 0;

	//19-24
	INT16 AccelX;
	INT16 AccelY;
	INT16 AccelZ;

	//30
	CString DeviceDescr;

	//33
	CString tpadEvent;

	//35
	int finger1down = 0;
	int finger1Counter = 0;

	//36-38
	int finger1X = 0;
	int finger1Y = 0;

	//39
	int finger2down = 0;
	int finger2Counter = 0;

	//40-42
	int finger2X = 0;
	int finger2Y = 0;
};

class Reader
{

	libusb_device** devs;
	libusb_device_handle* handle;
	const libusb_endpoint_descriptor* endpoint;
	libusb_config_descriptor* conf_desc;
	libusb_device* dev;

protected:
	libusb_device* FindGamepad(libusb_device** devs);
	inline void SetArrows(DecodedReport& dr);
public:
	/*
	Ищет геймпад, подменяет драйвер, получает дескриптор устройства.
	Нужно вызвать эту функцию в MyDlg::InitInstance()
	*/
	int InitInstance();
	void GetDecodedReport(DecodedReport& dr);

	~Reader();
};