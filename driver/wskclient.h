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

NTSTATUS AttachRemoteDevice(
	_In_ PCHAR RemoteIp,
	_In_ PCHAR DeviceId,
	_Inout_ WDFCONTEXT pContext
);

NTSTATUS DetachRemoteDevice(
	_Inout_ WDFCONTEXT pContext
);

EXTERN_C_END

#endif