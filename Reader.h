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
		unsigned char RepId;

		//	1 byte
		//Left Stick axis X 
		unsigned char LSx;

		//	2 byte
		//Left Stick axis Y 
		unsigned char LSy;

		//	3 byte
		//Right Stick axis X 
		unsigned char RSx;

		//	4 byte
		//Right Stick axis Y 
		unsigned char RSy;

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
		unsigned char Counter;
		//T-PAD click
		bool TPadCl;
		//PS btn
		bool PS;

		//	8 byte
		//L2 trigger
		unsigned char L2Trigger;

		//	9 byte
		//R2 trigger
		unsigned char R2Trigger;

		//	10-11 byte
		//timestamp 
		unsigned char TimeStamp1;
		unsigned char TimeStamp2;




		//	30 byte
		//наушники
		unsigned char HeadPh;
		string Device;

		//	31-32  byte
		string byte3132 = "unknown";

		//	33	byte
		unsigned char TPadEvent;

		//	34 byte
		unsigned char TPADcounter;

		//	35 byte
		bool FingerOneDown;
		unsigned char FingerOneCount;

		//	36-38 byte

		// 1111 1111  1111     1111  1111 1111
		unsigned char FingerOnePos1;
		unsigned char FingerOnePos2;
		unsigned char FingerOnePos3;
		unsigned int half1;
		unsigned int half2;
		unsigned int FingerOneX;
		unsigned int FingerOneY;

		//	39 byte
		bool FingerTwoDown;
		unsigned char FingerTwoCount;

		//	40-42 byte
		unsigned char FingerTwoPos1;
		unsigned char FingerTwoPos2;
		unsigned char FingerTwoPos3;
		unsigned int FingerTwoX;
		unsigned int FingerTwoY;




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
			case 30:
				HeadPh = ch;
				if (!bool(ch ^ 0x7b))
					Device = "Head with mic";

				else if (!bool(ch ^ 0x3b))
					Device = "Head";

				else if (!bool(ch ^ 0x1b))
					Device = "None";

				else if (!bool(ch ^ 0x8))
					Device = "Bluetooth ??";

				else
					Device = "???";
				cout << "Device: " << Device << endl;

				break;
			case 31:
				cout << "Byte 31: " << byte3132 << endl;
				break;
			case 32:
				cout << "Byte 32: " << byte3132 << endl;
				break;
			case 33:
				TPadEvent = (ch & 0xf);
				if (!bool(TPadEvent ^ 0x0))
					cout << "TPAD event: No data for Tpad" << endl;
				else if (!bool(TPadEvent ^ 0x01))
					cout << "TPAD event: Set data for 2 current touches" << endl;
				else if (!bool(TPadEvent ^ 0x02))
					cout << "TPAD event: set data for previous touches at [44-51]" << endl;
				break;
			case 34:
				TPADcounter = ch;
				cout << "T-PAD: auto incrementing number to track last update ? : " << (int)TPADcounter << endl;
				break;
			case 35:
				FingerOneDown = (bool((1 << 7) & ch));
				FingerOneCount = ch & 0x7f;
				cout << "FingerOneDown (TPAD): " << FingerOneDown << endl;
				cout << "FingerOneCounter (TPAD): " << (int)FingerOneCount << endl;
				break;
			case 36:
				FingerOnePos1 = ch;
				cout << "FingerOnePos1: " << (int)FingerOnePos1 << endl;
				break;
			case 37:
				FingerOnePos2 = ch;
				cout << "FingerOnePos2: " << (int)FingerOnePos2 << endl;
				break;
			case 38:
				FingerOnePos3 = ch;
				cout << "FingerOnePos3: " << (int)FingerOnePos3 << endl;
				half1 = FingerOnePos2 & 0xf;
				half2 = (FingerOnePos2 & 0xf0) >> 4;

				// half1(0000 xxxx) fingerOnePos1(yyyy yyyy) res: 0000 0000 0000 0000 0000 xxxx yyyy yyyy
				FingerOneX = (half1 << 8) | (unsigned int)FingerOnePos1;

				//  fingerOnePos3(yyyy yyyy) half2(0000 xxxx) res: 0000 0000 0000 0000 0000 yyyy yyyy xxxx
				FingerOneY = ((unsigned int)FingerOnePos3 << 4) | half2;

				cout << "FingerOneX: " << FingerOneX << endl;
				cout << "FingerOneY: " << FingerOneY << endl;

				break;
			case 39:
				FingerTwoDown = (bool((1 << 7) & ch));
				FingerTwoCount = ch & 0x7f;
				cout << "FingerTwoDown (TPAD): " << FingerTwoDown << endl;
				cout << "FingerTwoCounter (TPAD): " << (int)FingerTwoCount << endl;
				break;
			case 40:
				FingerTwoPos1 = ch;
				cout << "FingerTwoPos1: " << (int)FingerTwoPos1 << endl;
				break;
			case 41:
				FingerTwoPos2 = ch;
				cout << "FingerTwoPos2: " << (int)FingerTwoPos2 << endl;
				break;
			case 42:
				FingerTwoPos3 = ch;
				cout << "FingerTwoPos3: " << (int)FingerTwoPos3 << endl;
				half1 = FingerTwoPos2 & 0xf;
				half2 = (FingerTwoPos2 & 0xf0) >> 4;

				// half1(0000 xxxx) fingerOnePos1(yyyy yyyy) res: 0000 0000 0000 0000 0000 xxxx yyyy yyyy
				FingerTwoX = (half1 << 8) | (unsigned int)FingerTwoPos1;

				//  fingerOnePos3(yyyy yyyy) half2(0000 xxxx) res: 0000 0000 0000 0000 0000 yyyy yyyy xxxx
				FingerTwoY = ((unsigned int)FingerTwoPos3 << 4) | half2;

				cout << "FingerTwoX: " << FingerTwoX << endl;
				cout << "FingerTwoY: " << FingerTwoY << endl;

				break;

			default:
				cout << "id:" << id << " NULL" << endl;
				break;
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