#include "others.h"
#include "others.tmh"
#include "Device.h"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (PAGE, IoControlEvtWorkItem)
#endif

_Use_decl_annotations_
VOID
IoCtrl_EvtWorkItem(
	_In_ WDFWORKITEM WorkItem
)
{
	PAGED_CODE();
	WDFDEVICE device = (WDFDEVICE)WdfWorkItemGetParentObject(WorkItem);
	PDEVICE_CONTEXT deviceContext = DeviceGetContext(device);

	UcxRootHubPortChanged(deviceContext->USBRootHub);

	return;
}

_Use_decl_annotations_
VOID
RootHub_TimerFunc(
	WDFTIMER  Timer
)
{
	UNREFERENCED_PARAMETER(Timer);
	return;
}

_Use_decl_annotations_
VOID EvtDpcFunc_UrbComplete(
	_In_ WDFDPC Dpc
)
{
	UNREFERENCED_PARAMETER(Dpc);
	return;
}