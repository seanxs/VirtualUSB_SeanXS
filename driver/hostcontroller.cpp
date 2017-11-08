#include "hostcontroller.h"
#include "Device.h"
#include "hostcontroller.tmh"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (PAGE, CreateHostController)
#pragma alloc_text (PAGE, Controller_EvtControllerUsbDeviceAdd)
#pragma alloc_text (PAGE, Controller_EvtControllerReset)
#endif

NTSTATUS
CreateHostController(
	WDFDEVICE device
)
{
	PAGED_CODE();
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_CONTROLLER, "%!FUNC! Entry");
	NTSTATUS status = STATUS_SUCCESS;
	UCX_CONTROLLER_CONFIG HostControllerConfig;
	UCXCONTROLLER HostController;
	PDEVICE_CONTEXT pDeviceContext = DeviceGetContext(device);
	WDF_OBJECT_ATTRIBUTES attributes;
	PHOST_CONTROLLER_CONTEXT pControllerContext;

	UCX_CONTROLLER_CONFIG_INIT(&HostControllerConfig, "sean virtual USB host controller");
	HostControllerConfig.EvtControllerUsbDeviceAdd = Controller_EvtControllerUsbDeviceAdd;
	HostControllerConfig.EvtControllerQueryUsbCapability = Controller_EvtControllerQueryUsbCapability;
	HostControllerConfig.EvtControllerReset = Controller_EvtControllerReset;
	HostControllerConfig.EvtControllerGetCurrentFrameNumber = Controller_EvtGetCurrentFrameNumber;
	WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&attributes, HOST_CONTROLLER_CONTEXT);

	status = UcxControllerCreate(device, &HostControllerConfig, &attributes, &HostController);
	if (!NT_SUCCESS(status))
	{
		TraceEvents(TRACE_LEVEL_ERROR, TRACE_CONTROLLER, "UcxControllerCreate failed 0x%x", status);
		return status;
	}
	
	//KdPrint((__FUNCTION__"\tVirtual USB host controller has been created\n"));
	VBus_SeanXS_Print("\tVirtual USB host controller has been created\n");

	pControllerContext = GetHostControllerContext(HostController);
	pControllerContext->WdfDevice = device;
	pDeviceContext->USBController = HostController;

	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_CONTROLLER, "%!FUNC! Exit");
	return status;
}

NTSTATUS
Controller_EvtGetCurrentFrameNumber(
	__in
	UCXCONTROLLER   UcxController,
	__out
	PULONG          FrameNumber
)
{
	UNREFERENCED_PARAMETER(UcxController);

	//
	// TODO: Return the current 32-bit frame number.  Do not access the
	// controller registers if the controller is not in D0.
	//

	*FrameNumber = 0xFFFFFFFF;

	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_CONTROLLER, "Controller_EvtGetCurrentFrameNumber");

	return STATUS_SUCCESS;
}

NTSTATUS
Controller_EvtControllerQueryUsbCapability(
	__in
	UCXCONTROLLER   UcxController,
	__in
	PGUID           CapabilityType,
	__in
	ULONG           OutputBufferLength,
	__out_bcount_opt(OutputBufferLength)
	PVOID           OutputBuffer,
	__out
	PULONG          ResultLength
)
{
	NTSTATUS status;

	UNREFERENCED_PARAMETER(UcxController);
	UNREFERENCED_PARAMETER(OutputBufferLength);
	UNREFERENCED_PARAMETER(OutputBuffer);

	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_CONTROLLER, "%!FUNC! Entry");

	*ResultLength = 0;

	if (RtlCompareMemory(CapabilityType,
		&GUID_USB_CAPABILITY_CHAINED_MDLS,
		sizeof(GUID)) == sizeof(GUID)) {

		//
		// TODO: Is GUID_USB_CAPABILITY_CHAINED_MDLS supported?
		//
		TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_CONTROLLER, "GUID_USB_CAPABILITY_CHAINED_MDLS not supported");
		status = STATUS_NOT_SUPPORTED;
	}
	else if (RtlCompareMemory(CapabilityType,
		&GUID_USB_CAPABILITY_STATIC_STREAMS,
		sizeof(GUID)) == sizeof(GUID)) {

		//
		// TODO: Is GUID_USB_CAPABILITY_STATIC_STREAMS supported?
		//
		TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_CONTROLLER, "GUID_USB_CAPABILITY_STATIC_STREAMS not supported");
		status = STATUS_NOT_SUPPORTED;
	}
	else if (RtlCompareMemory(CapabilityType,
		&GUID_USB_CAPABILITY_FUNCTION_SUSPEND,
		sizeof(GUID)) == sizeof(GUID)) {

		//
		// TODO: Is GUID_USB_CAPABILITY_FUNCTION_SUSPEND supported?
		//
		TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_CONTROLLER, "GUID_USB_CAPABILITY_FUNCTION_SUSPEND supported");
		status = STATUS_SUCCESS;
	}
	else if (RtlCompareMemory(CapabilityType,
		&GUID_USB_CAPABILITY_SELECTIVE_SUSPEND,
		sizeof(GUID)) == sizeof(GUID)) {

		TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_CONTROLLER, "GUID_USB_CAPABILITY_SELECTIVE_SUSPEND supported");
		status = STATUS_SUCCESS;
	}
	else if (RtlCompareMemory(CapabilityType,
		&GUID_USB_CAPABILITY_HIGH_BANDWIDTH_ISOCH,
		sizeof(GUID)) == sizeof(GUID)) {

		TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_CONTROLLER, "GUID_USB_CAPABILITY_HIGH_BANDWIDTH_ISOCH not supported");
		status = STATUS_NOT_SUPPORTED;
	}
	else if (RtlCompareMemory(CapabilityType,
		&GUID_USB_CAPABILITY_DEVICE_CONNECTION_HIGH_SPEED_COMPATIBLE,
		sizeof(GUID)) == sizeof(GUID)) {

		TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_CONTROLLER, "GUID_USB_CAPABILITY_DEVICE_CONNECTION_HIGH_SPEED_COMPATIBLE supported");
		status = STATUS_SUCCESS;
	}
	else if (RtlCompareMemory(CapabilityType,
		&GUID_USB_CAPABILITY_DEVICE_CONNECTION_SUPER_SPEED_COMPATIBLE,
		sizeof(GUID)) == sizeof(GUID)) {

		TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_CONTROLLER, "GUID_USB_CAPABILITY_DEVICE_CONNECTION_SUPER_SPEED_COMPATIBLE supported");
		status = STATUS_SUCCESS;
	}
	else if (RtlCompareMemory(CapabilityType,
		&GUID_USB_CAPABILITY_CLEAR_TT_BUFFER_ON_ASYNC_TRANSFER_CANCEL,
		sizeof(GUID)) == sizeof(GUID)) {

		//
		// TODO: Is GUID_USB_CAPABILITY_CLEAR_TT_BUFFER_ON_ASYNC_TRANSFER_CANCEL supported?
		//
		TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_CONTROLLER, "GUID_USB_CAPABILITY_CLEAR_TT_BUFFER_ON_ASYNC_TRANSFER_CANCEL not supported");
		status = STATUS_NOT_SUPPORTED;
	}
	else {
		TraceEvents(TRACE_LEVEL_WARNING, TRACE_CONTROLLER, "Unhandled USB capability");
		VBus_SeanXS_Print("Unhandled USB capability\n");
		status = STATUS_NOT_IMPLEMENTED;
	}

	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_CONTROLLER, "%!FUNC! Exit");

	return status;
}

VOID
Controller_EvtControllerReset(
	__in
	UCXCONTROLLER   UcxController
)
{
	UCX_CONTROLLER_RESET_COMPLETE_INFO controllerResetCompleteInfo;
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_CONTROLLER, "%!FUNC! Entry");

	//
	// TODO: Reset the controller
	//

	//
	// TODO: Were devices and endpoints programmed in the controller before the reset
	// still programmed in the controller after the reset?
	//
	UCX_CONTROLLER_RESET_COMPLETE_INFO_INIT(
		&controllerResetCompleteInfo,
		UcxControllerStateLost,
		TRUE); // reset due to UCX, received EvtReset after WDF power-up

	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_CONTROLLER, "Controller_EvtControllerReset");

	UcxControllerResetComplete(UcxController, &controllerResetCompleteInfo);

	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_CONTROLLER, "%!FUNC! Exit");
	return;
}

NTSTATUS
Controller_EvtControllerUsbDeviceAdd(
	__in
	UCXCONTROLLER       UcxController,
	__in
	PUCXUSBDEVICE_INFO  UcxUsbDeviceInfo,
	__in
	PUCXUSBDEVICE_INIT  UsbDeviceInit
)
{
	PAGED_CODE();
	UNREFERENCED_PARAMETER(UcxController);
	UNREFERENCED_PARAMETER(UcxUsbDeviceInfo);
	UNREFERENCED_PARAMETER(UsbDeviceInit);
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_CONTROLLER, "%!FUNC! Entry");
	NTSTATUS status = STATUS_SUCCESS;

	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_CONTROLLER, "%!FUNC! Exit");
	return status;
}