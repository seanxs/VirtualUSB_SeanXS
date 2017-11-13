#include "roothub.h"
#include "Device.h"
#include "roothub.tmh"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (PAGE, CreateRoothub)
#endif

NTSTATUS
CreateRoothub(
	__in
	WDFDEVICE device
)
{
	PAGED_CODE();
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ROOTHUB, "%!FUNC! Entry");

	NTSTATUS status = STATUS_SUCCESS;
	UCX_ROOTHUB_CONFIG ucxRootHubConfig;
	UCXROOTHUB ucxRootHub;
	PROOTHUB_CONTEXT pRootHubContext;
	WDF_OBJECT_ATTRIBUTES attributes;
	PDEVICE_CONTEXT pDeviceContext = DeviceGetContext(device);
#if 0
	UCX_ROOTHUB_CONFIG_INIT(&ucxRootHubConfig,
		RootHub_EvtRootHubClearHubFeature,
		RootHub_EvtRootHubClearPortFeature,
		RootHub_EvtRootHubGetHubStatus,
		RootHub_EvtRootHubGetPortStatus,
		RootHub_EvtRootHubSetHubFeature,
		RootHub_EvtRootHubSetPortFeature,
		RootHub_EvtRootHubGetPortErrorCount,
		RootHub_EvtRootHubInterruptTx,
		RootHub_EvtRootHubGetInfo,
		RootHub_EvtRootHubGet20PortInfo,
		RootHub_EvtRootHubGet30PortInfo);
#else
	UCX_ROOTHUB_CONFIG_INIT_WITH_CONTROL_URB_HANDLER(
		&ucxRootHubConfig,
		RootHub_EvtRootHubControlUrb,
		RootHub_EvtRootHubInterruptTx,
		RootHub_EvtRootHubGetInfo,
		RootHub_EvtRootHubGet20PortInfo,
		RootHub_EvtRootHubGet30PortInfo
	);
#endif
	WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&attributes, ROOTHUB_CONTEXT);

	status = UcxRootHubCreate(pDeviceContext->USBController, &ucxRootHubConfig, &attributes, &ucxRootHub);
	if (!NT_SUCCESS(status)) {
		TraceEvents(TRACE_LEVEL_ERROR, TRACE_ROOTHUB, "UcxRootHubCreate failed 0x%x", status);
		return status;
	}

	KdPrint(("Virtual USB roothub has been created\n"));

	pRootHubContext = GetRootHubContext(ucxRootHub);
	pRootHubContext->WdfDevice = device;
	pDeviceContext->USBRootHub = ucxRootHub;

	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ROOTHUB, "%!FUNC! Exit");
	return status;
}

VOID
RootHub_EvtRootHubControlUrb(
	__in
	UCXROOTHUB  UcxRootHub,
	__in
	WDFREQUEST  Request
)
{
	UNREFERENCED_PARAMETER(UcxRootHub);
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ROOTHUB, "%!FUNC! Entry");

	PURB urb;
	WDF_REQUEST_PARAMETERS RequestParams;
	//PROOTHUB_CONTEXT pRootHubContext = GetRootHubContext(UcxRootHub);
	PWDF_USB_CONTROL_SETUP_PACKET pSetupPacket;

	WDF_REQUEST_PARAMETERS_INIT(&RequestParams);
	WdfRequestGetParameters(Request, &RequestParams);
	urb = (PURB)RequestParams.Parameters.Others.Arg1;
	pSetupPacket = (PWDF_USB_CONTROL_SETUP_PACKET)&urb->UrbControlTransferEx.SetupPacket[0];

	urb->UrbHeader.Status = USBD_STATUS_SUCCESS;
	WdfRequestComplete(Request, STATUS_SUCCESS);

	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ROOTHUB, "%!FUNC! Exit");
	return;
}

VOID
RootHub_EvtRootHubInterruptTx(
	__in
	UCXROOTHUB  UcxRootHub,
	__in
	WDFREQUEST  Request
)
{
	UNREFERENCED_PARAMETER(UcxRootHub);
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ROOTHUB, "%!FUNC! Entry");

	NTSTATUS status = STATUS_SUCCESS;
	PURB urb;
	ULONG TranBufLen = 0;
	PUCHAR pTranBuf = NULL;
	WDF_REQUEST_PARAMETERS wdfRequestParams;

	WDF_REQUEST_PARAMETERS_INIT(&wdfRequestParams);
	WdfRequestGetParameters(Request, &wdfRequestParams);
	urb = (PURB)wdfRequestParams.Parameters.Others.Arg1;
	_URB_BULK_OR_INTERRUPT_TRANSFER* pInterruptTx = (_URB_BULK_OR_INTERRUPT_TRANSFER*)&urb->UrbBulkOrInterruptTransfer;
	//PROOTHUB_CONTEXT ucxRootHubContext = GetRootHubContext(UcxRootHub);

	if (pInterruptTx->TransferBuffer)
	{
		pTranBuf = (PUCHAR)(pInterruptTx->TransferBuffer);
		TranBufLen = pInterruptTx->TransferBufferLength;
		//RtlCopyMemory(pTranBuf, &(ucxRootHubContext->PortchgBits), TranBufLen);
		TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ROOTHUB, "Root hub ports:%#x", *pTranBuf);
		//ucxRootHubContext->PortchgBits = 0;
	}

	urb->UrbHeader.Status = USBD_STATUS_SUCCESS;
	WdfRequestComplete(Request, status);

	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ROOTHUB, "%!FUNC! Exit");
	return;
}

VOID
RootHub_EvtRootHubGetInfo(
	__in
	UCXROOTHUB  UcxRootHub,
	__in
	WDFREQUEST  Request
)
{
	PROOTHUB_CONTEXT    ucxRootHubContext;
	WDF_REQUEST_PARAMETERS  wdfRequestParams;
	PROOTHUB_INFO           rootHubInfo;
	NTSTATUS                status;

	KdPrint((__FUNCTION__"\n"));
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ROOTHUB, "%!FUNC! Entry");

	ucxRootHubContext = GetRootHubContext(UcxRootHub);

	WDF_REQUEST_PARAMETERS_INIT(&wdfRequestParams);
	WdfRequestGetParameters(Request, &wdfRequestParams);

	rootHubInfo = (PROOTHUB_INFO)wdfRequestParams.Parameters.Others.Arg1;

	if (rootHubInfo->Size < sizeof(ROOTHUB_INFO)) {
		TraceEvents(TRACE_LEVEL_ERROR, TRACE_ROOTHUB, "Invalid ROOTHUB_INFO Size %d", rootHubInfo->Size);
		status = STATUS_INVALID_PARAMETER;
		goto RootHub_EvtRootHubGetInfo;
	}

	rootHubInfo->ControllerType = ControllerTypeSoftXhci;
	rootHubInfo->NumberOf20Ports = ucxRootHubContext->NumberOf20Ports;
	rootHubInfo->NumberOf30Ports = ucxRootHubContext->NumberOf30Ports;
	//rootHubInfo->MaxU1ExitLatency = ucxRootHubContext->U1DeviceExitLatency;
	//rootHubInfo->MaxU2ExitLatency = ucxRootHubContext->U2DeviceExitLatency;

	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ROOTHUB, "RootHub_UcxEvtGetInfo NumberOf20Ports %d NumberOf30Ports %d", rootHubInfo->NumberOf20Ports, rootHubInfo->NumberOf30Ports);

	status = STATUS_SUCCESS;

RootHub_EvtRootHubGetInfo:

	WdfRequestComplete(Request, status);
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ROOTHUB, "%!FUNC! Exit");

	return;
}

VOID
RootHub_EvtRootHubGet20PortInfo(
	__in
	UCXROOTHUB  UcxRootHub,
	__in
	WDFREQUEST  Request
)
{
	PROOTHUB_CONTEXT ucxRootHubContext;
	WDF_REQUEST_PARAMETERS wdfRequestParams;
	PROOTHUB_20PORTS_INFO rootHub20PortsInfo;
	NTSTATUS status;

	KdPrint((__FUNCTION__"\n"));
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ROOTHUB, "%!FUNC! Entry");

	ucxRootHubContext = GetRootHubContext(UcxRootHub);

	WDF_REQUEST_PARAMETERS_INIT(&wdfRequestParams);
	WdfRequestGetParameters(Request, &wdfRequestParams);

	rootHub20PortsInfo = (PROOTHUB_20PORTS_INFO)wdfRequestParams.Parameters.Others.Arg1;

	if (rootHub20PortsInfo->Size < sizeof(ROOTHUB_20PORTS_INFO)) {
		TraceEvents(TRACE_LEVEL_ERROR, TRACE_ROOTHUB, "Invalid ROOTHUB_20PORTS_INFO Size %d", rootHub20PortsInfo->Size);
		status = STATUS_INVALID_PARAMETER;
		goto RootHub_EvtRootHubGet20PortInfoEnd;
	}

	if (rootHub20PortsInfo->NumberOfPorts != ucxRootHubContext->NumberOf20Ports) {
		TraceEvents(TRACE_LEVEL_ERROR, TRACE_ROOTHUB, "Invalid ROOTHUB_20PORTS_INFO NumberOfPorts %d", rootHub20PortsInfo->NumberOfPorts);
		status = STATUS_INVALID_PARAMETER;
		goto RootHub_EvtRootHubGet20PortInfoEnd;
	}

	if (rootHub20PortsInfo->PortInfoSize < sizeof(ROOTHUB_20PORT_INFO)) {
		TraceEvents(TRACE_LEVEL_ERROR, TRACE_ROOTHUB, "Invalid ROOTHUB_20PORT_INFO Size %d", rootHub20PortsInfo->PortInfoSize);
		status = STATUS_INVALID_PARAMETER;
		goto RootHub_EvtRootHubGet20PortInfoEnd;
	}

	//
	// Return static root hub 2.0 port information.
	//
	PROOTHUB_20PORT_INFO pRootHUB20PortInfo = NULL;
	for (int i = 0; i < ucxRootHubContext->NumberOf20Ports; i++)
	{
		pRootHUB20PortInfo = rootHub20PortsInfo->PortInfoArray[i];
		pRootHUB20PortInfo->PortNumber = (USHORT)i + 1;
		pRootHUB20PortInfo->Removable = TriStateTrue;
	}

	status = STATUS_SUCCESS;

RootHub_EvtRootHubGet20PortInfoEnd:

	WdfRequestComplete(Request, status);
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ROOTHUB, "%!FUNC! Exit");

	return;
}

VOID
RootHub_EvtRootHubGet30PortInfo(
	__in
	UCXROOTHUB  UcxRootHub,
	__in
	WDFREQUEST  Request
)
{
	PROOTHUB_CONTEXT    ucxRootHubContext;
	WDF_REQUEST_PARAMETERS  wdfRequestParams;
	PROOTHUB_30PORTS_INFO   rootHub30PortsInfo;
	NTSTATUS                status;

	KdPrint((__FUNCTION__"\n"));
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ROOTHUB, "%!FUNC! Entry");

	ucxRootHubContext = GetRootHubContext(UcxRootHub);

	WDF_REQUEST_PARAMETERS_INIT(&wdfRequestParams);
	WdfRequestGetParameters(Request, &wdfRequestParams);

	rootHub30PortsInfo = (PROOTHUB_30PORTS_INFO)wdfRequestParams.Parameters.Others.Arg1;

	if (rootHub30PortsInfo->Size < sizeof(ROOTHUB_30PORTS_INFO)) {
		TraceEvents(TRACE_LEVEL_ERROR, TRACE_ROOTHUB, "Invalid ROOTHUB_30PORTS_INFO Size %d", rootHub30PortsInfo->Size);
		status = STATUS_INVALID_PARAMETER;
		goto RootHub_EvtRootHubGet30PortInfoEnd;
	}

	if (rootHub30PortsInfo->NumberOfPorts != ucxRootHubContext->NumberOf30Ports) {
		TraceEvents(TRACE_LEVEL_ERROR, TRACE_ROOTHUB, "Invalid ROOTHUB_30PORTS_INFO NumberOfPorts %d", rootHub30PortsInfo->NumberOfPorts);
		status = STATUS_INVALID_PARAMETER;
		goto RootHub_EvtRootHubGet30PortInfoEnd;
	}

	if (rootHub30PortsInfo->PortInfoSize < sizeof(ROOTHUB_30PORT_INFO)) {
		TraceEvents(TRACE_LEVEL_ERROR, TRACE_ROOTHUB, "Invalid ROOTHUB_30PORT_INFO Size %d", rootHub30PortsInfo->PortInfoSize);
		status = STATUS_INVALID_PARAMETER;
		goto RootHub_EvtRootHubGet30PortInfoEnd;
	}

	//
	// Return static root hub 3.0 port information.
	//
	PROOTHUB_30PORT_INFO pRootHUB30PortInfo = NULL;
	for (int i = 0; i < ucxRootHubContext->NumberOf30Ports; i++)
	{
		pRootHUB30PortInfo = rootHub30PortsInfo->PortInfoArray[i];
		pRootHUB30PortInfo->PortNumber = (USHORT)i + 1 + ucxRootHubContext->NumberOf20Ports;
		pRootHUB30PortInfo->Removable = TriStateTrue;
	}

	status = STATUS_SUCCESS;

RootHub_EvtRootHubGet30PortInfoEnd:

	WdfRequestComplete(Request, status);
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_ROOTHUB, "%!FUNC! Exit");

	return;
}