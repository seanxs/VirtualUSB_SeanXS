#include "endpoint.h"
#include "endpoint.tmh"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (PAGE, Endpoint_EvtUcxEndpointStaticStreamsAdd)
#endif

VOID Endpoint_EvtUcxEndpointPurge(
	_In_ UCXCONTROLLER UcxController,
	_In_ UCXENDPOINT Endpoint
)
{
	UNREFERENCED_PARAMETER(UcxController);

	PENDPOINT_CONTEXT pEndpointContext;
	KdPrint((__FUNCTION__"\tEndpoint:%#p\n", Endpoint));
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ENDPOINT, "%!FUNC! Entry");

	pEndpointContext = GetEndpointContext(Endpoint);
	if (pEndpointContext->queue)
	{
		WdfIoQueuePurge(pEndpointContext->queue, Endpoint_PurgeComplete, (WDFCONTEXT)Endpoint);
	}

	UcxEndpointPurgeComplete(Endpoint);

	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ENDPOINT, "%!FUNC! Exit");
	return;
}

VOID Endpoint_EvtUcxEndpointStart(
	_In_ UCXCONTROLLER UcxController,
	_In_ UCXENDPOINT   Endpoint
)
{
	UNREFERENCED_PARAMETER(UcxController);

	KdPrint((__FUNCTION__"\tEndpoint:%#p\n", Endpoint));
	PENDPOINT_CONTEXT pEndpointContext;
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ENDPOINT, "%!FUNC! Entry");

	pEndpointContext = GetEndpointContext(Endpoint);
	if (pEndpointContext->queue)
	{
		WdfIoQueueStart(pEndpointContext->queue);
	}

	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ENDPOINT, "%!FUNC! Exit");
	return;
}

VOID Endpoint_EvtUcxEndpointAbort(
	_In_ UCXCONTROLLER UcxController,
	_In_ UCXENDPOINT   Endpoint
)
{
	UNREFERENCED_PARAMETER(UcxController);

	KdPrint((__FUNCTION__"\tEndpoint:%#p\n", Endpoint));
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ENDPOINT, "%!FUNC! Entry");

	PENDPOINT_CONTEXT pEndpointContext;
	pEndpointContext = GetEndpointContext(Endpoint);
	if (pEndpointContext->queue)
	{
		WdfIoQueueStopAndPurge(pEndpointContext->queue, Endpoint_AbortComplete, (WDFCONTEXT)Endpoint);
	}

	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ENDPOINT, "%!FUNC! Exit");
	return;
}

VOID Endpoint_EvtUcxEndpointOkToCancelTransfers(
	_In_ UCXENDPOINT Endpoint
)
{
	KdPrint((__FUNCTION__"\tEndpoint:%#p\n", Endpoint));
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ENDPOINT, "%!FUNC! Entry");
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ENDPOINT, "%!FUNC! Exit");
	return;
}

VOID Endpoint_EvtUcxDefaultEndpointUpdate(
	_In_ UCXCONTROLLER UcxController,
	_In_ WDFREQUEST    Request
)
{
	UNREFERENCED_PARAMETER(UcxController);

	WDF_REQUEST_PARAMETERS requestParams;
	PDEFAULT_ENDPOINT_UPDATE pEndpointUpdate;

	KdPrint((__FUNCTION__"\n"));
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ENDPOINT, "%!FUNC! Entry");

	WDF_REQUEST_PARAMETERS_INIT(&requestParams);
	WdfRequestGetParameters(Request, &requestParams);

	pEndpointUpdate = (PDEFAULT_ENDPOINT_UPDATE)requestParams.Parameters.Others.Arg1;
	PENDPOINT_CONTEXT pEndpointContext = GetEndpointContext(pEndpointUpdate->DefaultEndpoint);
	pEndpointContext->MaxPacketSize = pEndpointUpdate->MaxPacketSize;

	WdfRequestComplete(Request, STATUS_SUCCESS);
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ENDPOINT, "%!FUNC! Exit");
	return;
}

VOID Endpoint_EvtUcxEndpointReset(
	_In_ UCXCONTROLLER UcxController,
	_In_ UCXENDPOINT   Endpoint,
	_In_ WDFREQUEST    Request
)
{
	UNREFERENCED_PARAMETER(UcxController);
	UNREFERENCED_PARAMETER(Endpoint);

	KdPrint((__FUNCTION__"\tEndpoint:%#p\n", Endpoint));
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ENDPOINT, "%!FUNC! Entry");

	WdfRequestComplete(Request, STATUS_SUCCESS);

	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ENDPOINT, "%!FUNC! Exit");
	return;
}

VOID Endpoint_EvtUcxEndpointStaticStreamsEnable(
	_In_ UCXENDPOINT Endpoint,
	_In_ UCXSSTREAMS UcxStaticStreams,
	_In_ WDFREQUEST  Request
)
{
	UNREFERENCED_PARAMETER(UcxStaticStreams);
	UNREFERENCED_PARAMETER(Endpoint);

	KdPrint((__FUNCTION__"Endpoint:%#p\n", Endpoint));
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ENDPOINT, "%!FUNC! Entry");

	WDF_REQUEST_PARAMETERS requestParams;
	PURB urb;

	WDF_REQUEST_PARAMETERS_INIT(&requestParams);
	WdfRequestGetParameters(Request, &requestParams);
	urb = (PURB)requestParams.Parameters.Others.Arg1;

	urb->UrbHeader.Status = USBD_STATUS_SUCCESS;
	WdfRequestComplete(Request, STATUS_SUCCESS);

	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ENDPOINT, "%!FUNC! Exit");
	return;
}

VOID Endpoint_EvtUcxEndpointStaticStreamsDisable(
	_In_ UCXENDPOINT Endpoint,
	_In_ UCXSSTREAMS UcxStaticStreams,
	_In_ WDFREQUEST  Request
)
{
	UNREFERENCED_PARAMETER(UcxStaticStreams);
	UNREFERENCED_PARAMETER(Endpoint);

	KdPrint((__FUNCTION__"Endpoint:%#p\n", Endpoint));
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ENDPOINT, "%!FUNC! Entry");

	WDF_REQUEST_PARAMETERS requestParams;
	PURB urb;

	WDF_REQUEST_PARAMETERS_INIT(&requestParams);
	WdfRequestGetParameters(Request, &requestParams);
	urb = (PURB)requestParams.Parameters.Others.Arg1;

	urb->UrbHeader.Status = USBD_STATUS_SUCCESS;
	WdfRequestComplete(Request, STATUS_SUCCESS);

	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ENDPOINT, "%!FUNC! Exit");
	return;
}

NTSTATUS Endpoint_EvtUcxEndpointStaticStreamsAdd(
	_In_ UCXENDPOINT       Endpoint,
	_In_ ULONG             NumberOfStreams,
	_In_ PUCXSSTREAMS_INIT UcxStaticStreamsInit
)
{
	PAGED_CODE();
	UNREFERENCED_PARAMETER(NumberOfStreams);
	UNREFERENCED_PARAMETER(Endpoint);
	UNREFERENCED_PARAMETER(UcxStaticStreamsInit);

	NTSTATUS status = STATUS_SUCCESS;
	KdPrint((__FUNCTION__"\n"));
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ENDPOINT, "%!FUNC! Entry");
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ENDPOINT, "%!FUNC! Exit");
	return status;
}

VOID
Endpoint_PurgeComplete(
	__in
	WDFQUEUE    Queue,
	__in
	WDFCONTEXT  Context
)
{
	UNREFERENCED_PARAMETER(Queue);
	KdPrint((__FUNCTION__"\n"));
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ENDPOINT, "%!FUNC! Entry");

	UCXENDPOINT Endpoint = (UCXENDPOINT)Context;
	UcxEndpointPurgeComplete(Endpoint);

	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ENDPOINT, "%!FUNC! Exit");
	return;
}

VOID
Endpoint_AbortComplete(
	__in
	WDFQUEUE    Queue,
	__in
	WDFCONTEXT  Context
)
{
	UNREFERENCED_PARAMETER(Queue);
	KdPrint((__FUNCTION__"\n"));
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ENDPOINT, "%!FUNC! Entry");

	UCXENDPOINT Endpoint = (UCXENDPOINT)Context;
	UcxEndpointAbortComplete(Endpoint);

	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ENDPOINT, "%!FUNC! Exit");
	return;
}