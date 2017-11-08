#ifndef _VBUS_SEANXS_WSKCLIENT_H
#define _VBUS_SEANXS_WSKCLIENT_H

#include "common.h"

EXTERN_C_START

NTSTATUS
InitWskClient(
	_In_ WDFDRIVER DriverObject
);

VOID UnInitWskClient(
	_In_ WDFDRIVER DriverObject
);

EXTERN_C_END

#endif