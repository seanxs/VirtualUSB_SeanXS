#pragma once
#ifndef _USBENUM_NETWORK_H
#define _USBENUM_NETWORK_H

#include "busenum.h"

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