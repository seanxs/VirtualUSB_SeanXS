#include "wskclient.h"
#include "Driver.h"
#include "wskclient.tmh"

const WSK_CLIENT_DISPATCH WskClientDispatch = {
	MAKE_WSK_VERSION(1, 0), //uses WSK version 1.0
	0,						// Reserved
	NULL					// WskClientEvent callback is not required in WSK version 1.0
};

#ifdef ALLOC_PRAGMA
#pragma alloc_text(PAGE, InitWskClient)
#pragma alloc_text(PAGE, UnInitWskClient)
#pragma alloc_text(PAGE, AttachRemoteDevice)
#endif

NTSTATUS
InitWskClient(
	_In_ WDFDRIVER DriverObject
)
{
	PAGED_CODE();

	NTSTATUS status = STATUS_SUCCESS;
	PDRIVER_CONTEXT pDrvContext = GetDriverContext(DriverObject);

	pDrvContext->wskClientNpi.ClientContext = NULL;
	pDrvContext->wskClientNpi.Dispatch = &WskClientDispatch;
	status = WskRegister(&pDrvContext->wskClientNpi, &pDrvContext->WskRegistration);
	if (!NT_SUCCESS(status))
	{
		TraceEvents(TRACE_LEVEL_ERROR, TRACE_WSKCLIENT, "WskRegister failed, status:%#x", status);
		return status;
	}

	status = WskCaptureProviderNPI(&pDrvContext->WskRegistration, WSK_NO_WAIT, &pDrvContext->WskProviderNpi);
	if (!NT_SUCCESS(status))
	{
		TraceEvents(TRACE_LEVEL_ERROR, TRACE_WSKCLIENT, "WskCaptureProviderNPI failed, status:%#x", status);
		WskDeregister(&pDrvContext->WskRegistration);
		return status;
	}

	return status;
}

VOID UnInitWskClient(
	_In_ WDFDRIVER DriverObject
)
{
	PAGED_CODE();

	PDRIVER_CONTEXT pDrvContext = GetDriverContext(DriverObject);

	WskReleaseProviderNPI(&pDrvContext->WskRegistration);
	WskDeregister(&pDrvContext->WskRegistration);

	return;
}

NTSTATUS AttachRemoteDevice(
	_In_ PCHAR RemoteIp,
	_In_ PCHAR DeviceId,
	_Inout_ WDFCONTEXT pContext
)
{
	PAGED_CODE();
	UNREFERENCED_PARAMETER(RemoteIp);
	UNREFERENCED_PARAMETER(DeviceId);
	UNREFERENCED_PARAMETER(pContext);

	NTSTATUS status = STATUS_SUCCESS;
	return status;
}

NTSTATUS DetachRemoteDevice(
	_Inout_ WDFCONTEXT pContext
)
{
	PAGED_CODE();
	UNREFERENCED_PARAMETER(pContext);

	NTSTATUS status = STATUS_SUCCESS;
	return status;
}