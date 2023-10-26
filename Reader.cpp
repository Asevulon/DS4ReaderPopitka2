#include"pch.h"
#include"Reader.h"


Reader::~Reader()
{
	libusb_free_device_list(devs, 1);
	libusb_close(handle);
	libusb_exit(NULL);
}
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

int Reader::InitInstance()
{

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
	dev = FindGamepad(devs);

	//получили дескриптор устройства, его потом надо закрыть через libusb_close(...)
	libusb_open(dev, &handle);

	/*
	uint8_t bus, port_path[8];
	struct libusb_bos_descriptor* bos_desc;
	struct libusb_config_descriptor* conf_desc;
	const struct libusb_endpoint_descriptor* endpoint;
	int i, j, k;
	int iface, nb_ifaces, first_iface = -1;
	struct libusb_device_descriptor dev_desc;
	const char* const speed_name[6] = { "Unknown", "1.5 Mbit/s (USB LowSpeed)", "12 Mbit/s (USB FullSpeed)",
		"480 Mbit/s (USB HighSpeed)", "5000 Mbit/s (USB SuperSpeed)", "10000 Mbit/s (USB SuperSpeedPlus)" };
	char string[128];
	uint8_t string_index[3];	// indexes of the string descriptors
	uint8_t endpoint_in = 0, endpoint_out = 0;	// default IN and OUT endpoints
	CString str;
	libusb_get_config_descriptor(dev, 0, &conf_desc);
	str.Format(L"              total length: %d\n", conf_desc->wTotalLength);
	pLB->InsertString(-1, str);
	str.Format(L"         descriptor length: %d\n", conf_desc->bLength);
	pLB->InsertString(-1, str);

	nb_ifaces = conf_desc->bNumInterfaces;
	str.Format(L"             nb interfaces: %d\n", nb_ifaces);
	pLB->InsertString(-1, str);

	if (nb_ifaces > 0)
		first_iface = conf_desc->interface[0].altsetting[0].bInterfaceNumber;
	for (i = 0; i < nb_ifaces; i++) {
		str.Format(L"              interface[%d]: id = %d\n", i,
			conf_desc->interface[i].altsetting[0].bInterfaceNumber);
		pLB->InsertString(-1, str);

		for (j = 0; j < conf_desc->interface[i].num_altsetting; j++) {
			str.Format(L"interface[%d].altsetting[%d]: num endpoints = %d\n",
				i, j, conf_desc->interface[i].altsetting[j].bNumEndpoints);
			pLB->InsertString(-1, str);

			str.Format(L"   Class.SubClass.Protocol: %02X.%02X.%02X\n",
				conf_desc->interface[i].altsetting[j].bInterfaceClass,
				conf_desc->interface[i].altsetting[j].bInterfaceSubClass,
				conf_desc->interface[i].altsetting[j].bInterfaceProtocol);
			pLB->InsertString(-1, str);

			if ((conf_desc->interface[i].altsetting[j].bInterfaceClass == LIBUSB_CLASS_MASS_STORAGE)
				&& ((conf_desc->interface[i].altsetting[j].bInterfaceSubClass == 0x01)
					|| (conf_desc->interface[i].altsetting[j].bInterfaceSubClass == 0x06))
				&& (conf_desc->interface[i].altsetting[j].bInterfaceProtocol == 0x50)) {
				// Mass storage devices that can use basic SCSI commands
			}
			for (k = 0; k < conf_desc->interface[i].altsetting[j].bNumEndpoints; k++) {
				struct libusb_ss_endpoint_companion_descriptor* ep_comp = NULL;
				endpoint = &conf_desc->interface[i].altsetting[j].endpoint[k];
				str.Format(L"       endpoint[%d].address: %02X\n", k, endpoint->bEndpointAddress);
				pLB->InsertString(-1, str);

				// Use the first interrupt or bulk IN/OUT endpoints as default for testing
				if ((endpoint->bmAttributes & LIBUSB_TRANSFER_TYPE_MASK) & (LIBUSB_TRANSFER_TYPE_BULK | LIBUSB_TRANSFER_TYPE_INTERRUPT)) {
					if (endpoint->bEndpointAddress & LIBUSB_ENDPOINT_IN) {
						if (!endpoint_in)
							endpoint_in = endpoint->bEndpointAddress;
					}
					else {
						if (!endpoint_out)
							endpoint_out = endpoint->bEndpointAddress;
					}
				}
				str.Format(L"           max packet size: %04X\n", endpoint->wMaxPacketSize);
				pLB->InsertString(-1, str);

				str.Format(L"          polling interval: %02X\n", endpoint->bInterval);
				pLB->InsertString(-1, str);

				libusb_get_ss_endpoint_companion_descriptor(NULL, endpoint, &ep_comp);
				if (ep_comp) {
					str.Format(L"                 max burst: %02X   (USB 3.0)\n", ep_comp->bMaxBurst);
					pLB->InsertString(-1, str);

					str.Format(L"        bytes per interval: %04X (USB 3.0)\n", ep_comp->wBytesPerInterval);
					pLB->InsertString(-1, str);

					libusb_free_ss_endpoint_companion_descriptor(ep_comp);
				}
			}
		}
	}*/


	//подмена драйвера
	libusb_set_auto_detach_kernel_driver(handle, 1);

	//захват интерфейса
	libusb_get_config_descriptor(dev, 0, &conf_desc);

	libusb_claim_interface(handle, 3);
	endpoint = &conf_desc->interface[3].altsetting[0].endpoint[0];
	
	
	return 0;
}

int TPadEvent = 0;
unsigned char rep[64];
int size = 0;
int arrows = 0;
int half1 = 0;
int half2 = 0;
void Reader::GetDecodedReport(DecodedReport& dr)
{

	libusb_bulk_transfer(handle, 0x82, rep, 64, &size, 1000);

	//1-4
	dr.lsaX = rep[1];
	dr.lsaY = rep[2];
	dr.rsaX = rep[3];
	dr.rsaY = rep[4];

	//5
	dr.triangle = (rep[5] >> 7) & 1;
	dr.circle = (rep[5] >> 6) & 1;
	dr.cross = (rep[5] >> 5) & 1;
	dr.rect = (rep[5] >> 4) & 1;

	arrows = rep[5] & 0xF;
	SetArrows(dr);

	//6
	dr.R3 = (rep[6] >> 7) & 1;
	dr.L3 = (rep[6] >> 6) & 1;
	dr.options = (rep[6] >> 5) & 1;
	dr.share = (rep[6] >> 4) & 1;
	dr.R2 = (rep[6] >> 3) & 1;
	dr.L2 = (rep[6] >> 2) & 1;
	dr.R1 = (rep[6] >> 1) & 1;
	dr.L1 = rep[6] & 1;

	//7
	dr.tpadclick = (rep[7] >> 1) & 1;
	dr.home = rep[7] & 1;

	//8-9
	dr.L2trigger = rep[8];
	dr.R2trigger = rep[9];

	//12
	dr.BatteryLevel = rep[12];

	//13-18
	dr.GyroX = (INT16(rep[14]) << 8) | (INT16(rep[13]));
	dr.GyroY = (INT16(rep[16]) << 8) | (INT16(rep[15]));
	dr.GyroZ = (INT16(rep[18]) << 8) | (INT16(rep[17]));

	//19-24
	dr.AccelX = (INT16(rep[20]) << 8) | (INT16(rep[19]));
	dr.AccelY = (INT16(rep[22]) << 8) | (INT16(rep[21]));
	dr.AccelZ = (INT16(rep[24]) << 8) | (INT16(rep[23]));

	//30
	if (!bool(rep[30] ^ 0x7b))
		dr.DeviceDescr = L"Head with mic";

	else if (!bool(rep[30] ^ 0x3b))
		dr.DeviceDescr = L"Head";

	else if (!bool(rep[30] ^ 0x1b))
		dr.DeviceDescr = L"None";

	else if (!bool(rep[30] ^ 0x8))
		dr.DeviceDescr = L"Bluetooth";

	//33
	TPadEvent = (rep[33] & 0xf);
	if (!bool(TPadEvent ^ 0x0))
		dr.tpadEvent = L"TPAD event: No data for Tpad";
	else if (!bool(TPadEvent ^ 0x01))
		dr.tpadEvent = L"TPAD event: Set data for 2 current touches";
	else if (!bool(TPadEvent ^ 0x02))
		dr.tpadEvent = L"TPAD event: set data for previous touches at [44-51]";

	//35 
	dr.finger1down = (rep[35] >> 7) & 1;
	dr.finger1Counter = rep[35] & 0x7f;

	//36-38
	half1 = rep[37] & 0xf;
	half2 = (rep[37] & 0xf0) >> 4;
	dr.finger1X = (half1 << 8) | (unsigned int)rep[36];
	dr.finger1Y = ((unsigned int)rep[38] << 4) | half2;

	//39
	dr.finger2down = (rep[39] >> 7) & 1;
	dr.finger2Counter = rep[39] & 0x7f;

	half1 = rep[41] & 0xf;
	half2 = (rep[41] & 0xf0) >> 4;
	dr.finger2X = (half1 << 8) | (unsigned int)rep[40];
	dr.finger2Y = ((unsigned int)rep[42] << 4) | half2;
}


void Reader::SetArrows(DecodedReport& dr)
{
	switch (arrows)
	{
	case 8:
		dr.N = 0;
		dr.W = 0;
		dr.S = 0;
		dr.E = 0;
		break;

	case 7 :
		dr.N = 1;
		dr.W = 1;
		dr.S = 0;
		dr.E = 0;
		break;

	case 6:
		dr.N = 0;
		dr.W = 1;
		dr.S = 0;
		dr.E = 0;
		break;

	case 5:
		dr.N = 0;
		dr.W = 1;
		dr.S = 1;
		dr.E = 0;
		break;
	case 4:
		dr.N = 0;
		dr.W = 0;
		dr.S = 1;
		dr.E = 0;
		break;
	case 3:
		dr.N = 0;
		dr.W = 0;
		dr.S = 1;
		dr.E = 1;
		break;
	case 2:
		dr.N = 0;
		dr.W = 0;
		dr.S = 0;
		dr.E = 1;
		break;

	case 1:
		dr.N = 1;
		dr.W = 0;
		dr.S = 0;
		dr.E = 1;
		break;

	case 0:
		dr.N = 1;
		dr.W = 0;
		dr.S = 0;
		dr.E = 0;
		break;

	default:
		break;
	}
}