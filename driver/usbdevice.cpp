#include "usbdevice.h"
#include "usbdevice.tmh"
#include "endpoint.h"
#include "Queue.h"
#include "hostcontroller.h"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (PAGE, UsbDevice_EvtUcxUsbDeviceDefaultEndpointAdd)
#pragma alloc_text (PAGE, UsbDevice_EvtUcxUsbDeviceEndpointAdd)
#endif

VOID
UsbDevice_EvtUcxUsbDeviceEndpointsConfigure(
	__in
	UCXCONTROLLER   UcxController,
	__in
	WDFREQUEST      Request
)
{
	UNREFERENCED_PARAMETER(UcxController);

	KdPrint((__FUNCTION__"\n"));
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_USBDEVICE, "%!FUNC! Entry");

	WDF_REQUEST_PARAMETERS wdfRequestParams;
	WDF_REQUEST_PARAMETERS_INIT(&wdfRequestParams);
	WdfRequestGetParameters(Request, &wdfRequestParams);
	PENDPOINTS_CONFIGURE pEndPointsCfg = (PENDPOINTS_CONFIGURE)wdfRequestParams.Parameters.Others.Arg1;
	
	KdPrint(("USB device:%#p, \n", pEndPointsCfg->Header.UsbDevice));

	WdfRequestComplete(Request, STATUS_SUCCESS);
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_USBDEVICE, "%!FUNC! Exit");
	return;
}

VOID
UsbDevice_EvtUcxUsbDeviceEnable(
	__in
	UCXCONTROLLER   UcxController,
	__in
	WDFREQUEST      Request
)
{
	UNREFERENCED_PARAMETER(UcxController);

	KdPrint((__FUNCTION__"\n"));
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_USBDEVICE, "%!FUNC! Entry");

	WDF_REQUEST_PARAMETERS wdfRequestParams;
	WDF_REQUEST_PARAMETERS_INIT(&wdfRequestParams);
	WdfRequestGetParameters(Request, &wdfRequestParams);
	PUSBDEVICE_ENABLE pUSBDevEnable = (PUSBDEVICE_ENABLE)wdfRequestParams.Parameters.Others.Arg1;
	PENDPOINT_CONTEXT EndpointContext = GetEndpointContext(pUSBDevEnable->DefaultEndpoint);

	KdPrint(("USB device:%#p, \n", pUSBDevEnable->Header.UsbDevice));
	WdfIoQueueStart(EndpointContext->queue);

	WdfRequestComplete(Request, STATUS_SUCCESS);
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_USBDEVICE, "%!FUNC! Exit");
	return;
}

VOID
UsbDevice_EvtUcxUsbDeviceDisable(
	__in
	UCXCONTROLLER   UcxController,
	__in
	WDFREQUEST      Request
)
{
	UNREFERENCED_PARAMETER(UcxController);

	KdPrint((__FUNCTION__"\n"));
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_USBDEVICE, "%!FUNC! Entry");

	WDF_REQUEST_PARAMETERS wdfRequestParams;
	WDF_REQUEST_PARAMETERS_INIT(&wdfRequestParams);
	WdfRequestGetParameters(Request, &wdfRequestParams);
	PUSBDEVICE_DISABLE pUSBDevDisable = (PUSBDEVICE_DISABLE)wdfRequestParams.Parameters.Others.Arg1;
	PENDPOINT_CONTEXT EndpointContext = GetEndpointContext(pUSBDevDisable->DefaultEndpoint);

	KdPrint(("USB device:%#p, \n", pUSBDevDisable->Header.UsbDevice));
	WdfIoQueueStop(EndpointContext->queue, NULL, NULL);

	WdfRequestComplete(Request, STATUS_SUCCESS);
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_USBDEVICE, "%!FUNC! Exit");
	return;
}

VOID
UsbDevice_EvtUcxUsbDeviceReset(
	__in
	UCXCONTROLLER   UcxController,
	__in
	WDFREQUEST      Request
)
{
	UNREFERENCED_PARAMETER(UcxController);

	KdPrint((__FUNCTION__"\n"));
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_USBDEVICE, "%!FUNC! Entry");

	WDF_REQUEST_PARAMETERS wdfRequestParams;
	WDF_REQUEST_PARAMETERS_INIT(&wdfRequestParams);
	WdfRequestGetParameters(Request, &wdfRequestParams);
	PUSBDEVICE_RESET pUSBDevReset = (PUSBDEVICE_RESET)wdfRequestParams.Parameters.Others.Arg1;
	
	KdPrint(("USB device:%#p, Endpoints Count:%d\n",
		pUSBDevReset->Header.UsbDevice,
		pUSBDevReset->EndpointsToDisableCount));

	WdfRequestComplete(Request, STATUS_SUCCESS);
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_USBDEVICE, "%!FUNC! Exit");
	return;
}

VOID
UsbDevice_EvtUcxUsbDeviceAddress(
	__in
	UCXCONTROLLER   UcxController,
	__in
	WDFREQUEST      Request
)
{
	UNREFERENCED_PARAMETER(UcxController);

	KdPrint((__FUNCTION__"\n"));
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_USBDEVICE, "%!FUNC! Entry");

	WDF_REQUEST_PARAMETERS wdfRequestParams;
	WDF_REQUEST_PARAMETERS_INIT(&wdfRequestParams);
	WdfRequestGetParameters(Request, &wdfRequestParams);
	PUSBDEVICE_ADDRESS pUSBDevAddr = (PUSBDEVICE_ADDRESS)wdfRequestParams.Parameters.Others.Arg1;

	KdPrint(("USB device:%#p, address:%d\n",
		pUSBDevAddr->Header.UsbDevice,
		pUSBDevAddr->Address));

	WdfRequestComplete(Request, STATUS_SUCCESS);
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_USBDEVICE, "%!FUNC! Exit");
	return;
}

VOID
UsbDevice_EvtUcxUsbDeviceUpdate(
	__in
	UCXCONTROLLER   UcxController,
	__in
	WDFREQUEST      Request
)
{
	UNREFERENCED_PARAMETER(UcxController);

	KdPrint((__FUNCTION__"\n"));
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_USBDEVICE, "%!FUNC! Entry");

	WDF_REQUEST_PARAMETERS wdfRequestParams;
	WDF_REQUEST_PARAMETERS_INIT(&wdfRequestParams);
	WdfRequestGetParameters(Request, &wdfRequestParams);
	PUSBDEVICE_UPDATE pUSBDevUpdate = (PUSBDEVICE_UPDATE)wdfRequestParams.Parameters.Others.Arg1;

	KdPrint(("USB device:%#p, update flags:%#x\n", pUSBDevUpdate->Header.UsbDevice, pUSBDevUpdate->Flags));

	WdfRequestComplete(Request, STATUS_SUCCESS);
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_USBDEVICE, "%!FUNC! Exit");
	return;
}

VOID
UsbDevice_EvtUcxUsbDeviceHubInfo(
	__in
	UCXCONTROLLER   UcxController,
	__in
	WDFREQUEST      Request
)
{
	UNREFERENCED_PARAMETER(UcxController);

	KdPrint((__FUNCTION__"\n"));
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_USBDEVICE, "%!FUNC! Entry");
	//
	// Retrieve the USBDEVICE_HUB_INFO pointer from the
	// IOCTL_INTERNAL_USB_USBDEVICE_HUB_INFO WdfRequest.
	//
	WDF_REQUEST_PARAMETERS wdfRequestParams;
	WDF_REQUEST_PARAMETERS_INIT(&wdfRequestParams);
	WdfRequestGetParameters(Request, &wdfRequestParams);
	PUSBDEVICE_HUB_INFO phubInfo = (PUSBDEVICE_HUB_INFO)wdfRequestParams.Parameters.Others.Arg1;

	KdPrint(("USB device:%#p, NumberOfPorts:%d, NumberOfTTs:%d, TTThinkTime:%d\n",
		phubInfo->Header.UsbDevice,
		phubInfo->NumberOfPorts,
		phubInfo->NumberOfTTs,
		phubInfo->TTThinkTime));

	WdfRequestComplete(Request, STATUS_SUCCESS);
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_USBDEVICE, "%!FUNC! Exit");
	return;
}

NTSTATUS
UsbDevice_EvtUcxUsbDeviceDefaultEndpointAdd(
	__in
	UCXCONTROLLER       UcxController,
	__in
	UCXUSBDEVICE        UcxUsbDevice,
	__in
	ULONG               MaxPacketSize,
	__in
	PUCXENDPOINT_INIT   UcxEndpointInit
)
{
	PAGED_CODE();
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_USBDEVICE, "%!FUNC! Entry");
	KdPrint((__FUNCTION__"\n"));

	NTSTATUS status = STATUS_SUCCESS;
	UCX_DEFAULT_ENDPOINT_EVENT_CALLBACKS DefaultEndpointEventCallbacks;
	WDF_OBJECT_ATTRIBUTES objectAttributes;
	UCXENDPOINT ucxEndpoint;
	WDF_IO_QUEUE_CONFIG queueConfig;
	WDFQUEUE wdfQueue;
	PENDPOINT_CONTEXT pEndpointContext;
	PHOST_CONTROLLER_CONTEXT pControllerContext = GetHostControllerContext(UcxController);
	PUSB_DEVICE_CONTEXT pUsbDeviceContext = GetUsbDeviceContext(UcxUsbDevice);

	do
	{
		UCX_DEFAULT_ENDPOINT_EVENT_CALLBACKS_INIT(
			&DefaultEndpointEventCallbacks,
			Endpoint_EvtUcxEndpointPurge,
			Endpoint_EvtUcxEndpointStart,
			Endpoint_EvtUcxEndpointAbort,
			Endpoint_EvtUcxEndpointOkToCancelTransfers,
			Endpoint_EvtUcxDefaultEndpointUpdate);

		UcxDefaultEndpointInitSetEventCallbacks(UcxEndpointInit, &DefaultEndpointEventCallbacks);
		WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&objectAttributes, ENDPOINT_CONTEXT);
		status = UcxEndpointCreate(UcxUsbDevice, &UcxEndpointInit, &objectAttributes, &ucxEndpoint);
		if (!NT_SUCCESS(status))
		{
			TraceEvents(TRACE_LEVEL_ERROR, TRACE_USBDEVICE, "UcxEndpointCreate Failed %!STATUS!", status);
			break;
		}
		KdPrint(("Default endpoint created for usb device(%#p)\n", UcxUsbDevice));

		pEndpointContext = GetEndpointContext(ucxEndpoint);
		pEndpointContext->IsDefault = TRUE;
		pEndpointContext->MaxPacketSize = MaxPacketSize;

		WDF_IO_QUEUE_CONFIG_INIT(&queueConfig, WdfIoQueueDispatchManual);
		//WDF_IO_QUEUE_CONFIG_INIT(&queueConfig, WdfIoQueueDispatchSequential);
		queueConfig.EvtIoCanceledOnQueue = UrbIoCanceledOnQueue;
		WDF_OBJECT_ATTRIBUTES_INIT(&objectAttributes);
		//objectAttributes.ExecutionLevel = WdfExecutionLevelPassive;
		status = WdfIoQueueCreate(pControllerContext->WdfDevice, &queueConfig, &objectAttributes, &wdfQueue);
		if (!NT_SUCCESS(status))
		{
			TraceEvents(TRACE_LEVEL_ERROR, TRACE_USBDEVICE, "WdfIoQueueCreate Failed %!STATUS!", status);
			break;
		}
		UcxEndpointSetWdfIoQueue(ucxEndpoint, wdfQueue);
		pEndpointContext->queue = wdfQueue;

		status = WdfIoQueueReadyNotify(wdfQueue, DefaultEndpoint_QueuqReady, (WDFCONTEXT)pUsbDeviceContext);
		if (!NT_SUCCESS(status))
		{
			TraceEvents(TRACE_LEVEL_ERROR, TRACE_USBDEVICE, "WdfIoQueueReadyNotify Failed %!STATUS!", status);
			break;
		}
		KdPrint(("wdfqueue object created for endpoint(%#p)\n", ucxEndpoint));

	} while (FALSE);

	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_USBDEVICE, "%!FUNC! Exit");
	return status;
}

NTSTATUS
UsbDevice_EvtUcxUsbDeviceEndpointAdd(
	__in
	UCXCONTROLLER   UcxController,
	__in
	UCXUSBDEVICE    UcxUsbDevice,
	__in_bcount(UsbEndpointDescriptorBufferLength)
	PUSB_ENDPOINT_DESCRIPTOR    UsbEndpointDescriptor,
	__in
	ULONG           UsbEndpointDescriptorBufferLength,
	__in_opt
	PUSB_SUPERSPEED_ENDPOINT_COMPANION_DESCRIPTOR SuperSpeedEndpointCompanionDescriptor,
	__in
	PUCXENDPOINT_INIT   UcxEndpointInit
)
{
	UNREFERENCED_PARAMETER(SuperSpeedEndpointCompanionDescriptor);
	UNREFERENCED_PARAMETER(UsbEndpointDescriptorBufferLength);
	PAGED_CODE();
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_USBDEVICE, "%!FUNC! Entry");
	KdPrint((__FUNCTION__"\n"));

	NTSTATUS status = STATUS_SUCCESS;
	UCX_ENDPOINT_EVENT_CALLBACKS EndpointEventCallbacks;
	WDF_OBJECT_ATTRIBUTES objectAttributes;
	UCXENDPOINT ucxEndpoint;
	WDF_IO_QUEUE_CONFIG queueConfig;
	WDFQUEUE wdfQueue;
	PENDPOINT_CONTEXT pEndpointContext;
	PHOST_CONTROLLER_CONTEXT pControllerContext = GetHostControllerContext(UcxController);
	PUSB_DEVICE_CONTEXT pUsbDeviceContext = GetUsbDeviceContext(UcxUsbDevice);

	do
	{
		UCX_ENDPOINT_EVENT_CALLBACKS_INIT(
			&EndpointEventCallbacks,
			Endpoint_EvtUcxEndpointPurge,
			Endpoint_EvtUcxEndpointStart,
			Endpoint_EvtUcxEndpointAbort,
			Endpoint_EvtUcxEndpointReset,
			Endpoint_EvtUcxEndpointOkToCancelTransfers,
			Endpoint_EvtUcxEndpointStaticStreamsAdd,
			Endpoint_EvtUcxEndpointStaticStreamsEnable,
			Endpoint_EvtUcxEndpointStaticStreamsDisable);

		UcxEndpointInitSetEventCallbacks(UcxEndpointInit, &EndpointEventCallbacks);
		WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&objectAttributes, ENDPOINT_CONTEXT);
		status = UcxEndpointCreate(UcxUsbDevice, &UcxEndpointInit, &objectAttributes, &ucxEndpoint);
		if (!NT_SUCCESS(status))
		{
			TraceEvents(TRACE_LEVEL_ERROR, TRACE_USBDEVICE, "UcxEndpointCreate Failed %!STATUS!", status);
			break;
		}
		KdPrint(("endpoint created for usb device(%#p)\n", UcxUsbDevice));

		pEndpointContext = GetEndpointContext(ucxEndpoint);
		pEndpointContext->IsDefault = FALSE;
		pEndpointContext->MaxPacketSize = UsbEndpointDescriptor->wMaxPacketSize;

		WDF_IO_QUEUE_CONFIG_INIT(&queueConfig, WdfIoQueueDispatchManual);
		queueConfig.EvtIoCanceledOnQueue = UrbIoCanceledOnQueue;
		//WDF_IO_QUEUE_CONFIG_INIT(&queueConfig, WdfIoQueueDispatchSequential);
		WDF_OBJECT_ATTRIBUTES_INIT(&objectAttributes);
		//objectAttributes.ExecutionLevel = WdfExecutionLevelPassive;
		status = WdfIoQueueCreate(pControllerContext->WdfDevice, &queueConfig, &objectAttributes, &wdfQueue);
		if (!NT_SUCCESS(status))
		{
			TraceEvents(TRACE_LEVEL_ERROR, TRACE_USBDEVICE, "WdfIoQueueCreate Failed %!STATUS!", status);
			break;
		}
		UcxEndpointSetWdfIoQueue(ucxEndpoint, wdfQueue);
		pEndpointContext->queue = wdfQueue;

		status = WdfIoQueueReadyNotify(wdfQueue, Endpoint_QueuqReady, (WDFCONTEXT)pUsbDeviceContext);
		if (!NT_SUCCESS(status))
		{
			TraceEvents(TRACE_LEVEL_ERROR, TRACE_USBDEVICE, "WdfIoQueueReadyNotify Failed %!STATUS!", status);
			break;
		}
		KdPrint(("wdfqueue object created for endpoint(%#p)\n", ucxEndpoint));

	} while (FALSE);

	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_USBDEVICE, "%!FUNC! Exit");
	return status;
}