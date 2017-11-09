#ifndef _VBUS_SEANXS_COMMON_H
#define _VBUS_SEANXS_COMMON_H

#include <ntddk.h>
#include <wdf.h>
#include <initguid.h>
#include <wsk.h>
#include <usb.h>
#include <usbdlib.h>
#include <wdfusb.h>
#pragma warning(disable:4214; disable:4201; disable:4702)
#include <ucx\1.4\ucxclass.h>
#pragma warning(default:4214; default:4201; default:4702)

#include "Trace.h"

#define USB20_PORTS 4
#define USB30_PORTS 4
#define PORT_COUNT (USB20_PORTS + USB30_PORTS)

#define VBUS_SEAN_POOL_TAG 'NAES'

#define IP_DEV_BUF_SIZE 32

#endif // !_VBUS_SEANXS_COMMON_H
