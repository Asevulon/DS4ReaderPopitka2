#include"pch.h"
#include"Reader.h"


libusb_device* Reader::FindGamepad(libusb_device** devs)
{

	libusb_device* dev;
	int i = 0, j = 0;
	uint8_t path[8];


	while ((dev = devs[i++]) != NULL) {
		struct libusb_device_descriptor desc;
		int r = libusb_get_device_descriptor(dev, &desc);
		if ((desc.idProduct == 2508) && (desc.idVendor == 1356))
		{
			return dev;
		}
	}
}

int Reader::InitInstance(CListBox* LB)
{
	pLB = LB;

	libusb_device** devs;
	int r;
	ssize_t cnt;

	r = libusb_init(/*ctx=*/NULL);// /*options=*/NULL, /*num_options=*/0);
	if (r < 0)
		return r;

	//получили список устройств
	cnt = libusb_get_device_list(NULL, &devs);
	if (cnt < 0) {
		libusb_exit(NULL);
		return (int)cnt;
	}

	//нашли геймпад по ид производителя и устройства
	libusb_device* dev = FindGamepad(devs);

	//получили дескриптор устройства, его потом надо закрыть через libusb_close(...)
	libusb_open(dev, &handle);

	//подмена драйвера
	libusb_set_auto_detach_kernel_driver(handle, 1);

	//захват интерфейса
	//libusb_config_descriptor* conf_desc;



	uint8_t endpoint_in = 0, endpoint_out = 0;	// default IN and OUT endpoints

	libusb_get_config_descriptor(dev, 0, &conf_desc);

	libusb_claim_interface(handle, 3);
	endpoint = &conf_desc->interface[3].altsetting[0].endpoint[0];
	//uint8_t endp = endpoint->bEndpointAddress & 0xF;
	
	//libusb_bulk_transfer
	
	libusb_free_device_list(devs, 1);

	libusb_exit(NULL);
	return 0;
}


void Reader::GetDecodedReport(DecodedReport& dr)
{
	unsigned char OutReport[64];
	int size = 0;
	int r = libusb_bulk_transfer(handle, 0x82, OutReport, 64, &size, 1000);


}