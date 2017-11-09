/*++

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    user and kernel

--*/
#ifndef _VBUS_SEANXS_PUBLIC_H
#define _VBUS_SEANXS_PUBLIC_H

#include "common.h"

//
// Define an Interface Guid so that apps can find the device and talk to it.
//

DEFINE_GUID (GUID_DEVINTERFACE_driver,
    0x97005af0,0x3c7f,0x4f98,0xa4,0xeb,0x43,0xd5,0xcc,0x88,0x87,0x7f);
// {97005af0-3c7f-4f98-a4eb-43d5cc88877f}

typedef struct _REMOTE_DEVICE_INFO
{
	CHAR IpAddr[IP_DEV_BUF_SIZE];
	CHAR Device[IP_DEV_BUF_SIZE];
	INT16 Port_Number;
}
REMOTE_DEVICE_INFO, *PREMOTE_DEVICE_INFO;

#define FILE_DEVICE_VBUS_SEANXS FILE_DEVICE_BUS_EXTENDER

#define VBUS_SEANXS_IOCTL(_index_) \
    CTL_CODE (FILE_DEVICE_VBUS_SEANXS, _index_+0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_VBUS_SEANXS_PLUGIN_HARDWARE VBUS_SEANXS_IOCTL (0x0)
#define IOCTL_VBUS_SEANXS_UNPLUGIN_HARDWARE VBUS_SEANXS_IOCTL (0x1)
#define IOCTL_VBUS_SEANXS_QUERY_REMOTE_DEVICELIST VBUS_SEANXS_IOCTL (0x2)
#define IOCTL_VBUS_SEANXS_ATTACH_REMOTE_DEVICE VBUS_SEANXS_IOCTL (0x3)
#define IOCTL_VBUS_SEANXS_DETACH_REMOTE_DEVICE VBUS_SEANXS_IOCTL (0x4)

#endif // !_VBUS_SEANXS_PUBLIC_H
