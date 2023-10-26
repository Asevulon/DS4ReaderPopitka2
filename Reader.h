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
	/*
	���� �������, ��������� �������, �������� ���������� ����������.
	����� ������� ��� ������� � MyDlg::InitInstance()
	*/
	int InitInstance(CListBox*LB);
	void GetDecodedReport(DecodedReport& dr);
};