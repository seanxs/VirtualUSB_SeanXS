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

//
// Define an Interface Guid so that apps can find the device and talk to it.
//

DEFINE_GUID (GUID_DEVINTERFACE_driver,
    0x97005af0,0x3c7f,0x4f98,0xa4,0xeb,0x43,0xd5,0xcc,0x88,0x87,0x7f);
// {97005af0-3c7f-4f98-a4eb-43d5cc88877f}

#endif // !_VBUS_SEANXS_PUBLIC_H
