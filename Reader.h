#pragma once
#include "libusb.h"

struct DecodedReport
{

};

class Reader
{
	CListBox* pLB;

	libusb_device_handle* handle;
	const libusb_endpoint_descriptor* endpoint;
	libusb_config_descriptor* conf_desc;

protected:
	libusb_device* FindGamepad(libusb_device** devs);

public:
	//структура данных, хранящая в себе значения из отчета
	struct PSData {
	private:
		//	0 byte
		//Report ID
		char RepId;

		//	1 byte
		//Left Stick axis X 
		char LSx;

		//	2 byte
		//Left Stick axis Y 
		char LSy;

		//	3 byte
		//Right Stick axis X 
		char RSx;

		//	4 byte
		//Right Stick axis Y 
		char RSy;

		//	5 byte
		//triangle btn
		bool TrBtn;
		//circle btn
		bool CirBtn;
		//cross btn
		bool CrosBtn;
		//square btn
		bool SqBtn;
		//D-PAD
		char DPad;

		//	6 byte
		//R3 btn
		bool R3;
		//L3 btn
		bool L3;
		//Options btn
		bool Options;
		//Share btn
		bool Share;
		//R2 btn
		bool R2;
		//L2 btn
		bool L2;
		//R1 btn
		bool R1;
		//L1 btn
		bool L1;

		//	7 byte
		//Counter
		char Counter;
		//T-PAD click
		bool TPadCl;
		//PS btn
		bool PS;

		//	8 byte
		//L2 trigger
		char L2Trigger;

		//	9 byte
		//R2 trigger
		char R2Trigger;

		//	10-11 byte
		//timestamp 
		char TimeStamp1;
		char TimeStamp2;

		//
	public:
		void GetData(int id, char ch) {
			switch (id) {
			case 0:
				RepId = ch;
				cout << "RepId: " << (int)RepId << endl;
				break;
			case 1:
				LSx = ch;
				cout << "Left Stick axis X : " << (int)LSx << endl;
				break;
			case 2:
				LSy = ch;
				cout << "Left Stick axis Y : " << (int)LSy << endl;
				break;
			case 3:
				RSx = ch;
				cout << "Right Stick axis X : " << (int)RSx << endl;
				break;
			case 4:
				RSy = ch;
				cout << "Right Stick axis Y : " << (int)RSy << endl;
				break;
			case 5:
				TrBtn = (bool((1 << 0) & ch));
				CirBtn = (bool((1 << 1) & ch));
				CrosBtn = (bool((1 << 2) & ch));
				SqBtn = (bool((1 << 3) & ch));
				cout << "triangle btn : " << (int)TrBtn <<
					endl << "circle bt : " << (int)CirBtn <<
					endl << "cross btn : " << (int)CrosBtn <<
					endl << "square btn : " << (int)SqBtn << endl;
				break;
			default:
				cout << "Значение не равно 5, 6 и даже 0";
			}

		}
	};

	/*
	Ищет геймпад, подменяет драйвер, получает дескриптор устройства.
	Нужно вызвать эту функцию в MyDlg::InitInstance()
	*/
	int InitInstance(CListBox*LB);
	void GetDecodedReport(DecodedReport& dr);
};