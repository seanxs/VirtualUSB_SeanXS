/*++

Module Name:

    queue.c

Abstract:

    This file contains the queue entry points and callbacks.

Environment:

    Kernel-mode Driver Framework

--*/

#include "queue.h"
#include "queue.tmh"
#include "Public.h"
#include "wskclient.h"
#include "Device.h"
#include "roothub.h"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (PAGE, driverQueueInitialize)
#endif

NTSTATUS
driverQueueInitialize(
    _In_ WDFDEVICE Device
    )
/*++

Routine Description:

     The I/O dispatch callbacks for the frameworks device object
     are configured in this function.

     A single default I/O Queue is configured for parallel request
     processing, and a driver context memory allocation is created
     to hold our structure QUEUE_CONTEXT.

Arguments:

    Device - Handle to a framework device object.

Return Value:

    VOID

--*/
{
    WDFQUEUE queue;
    NTSTATUS status;
    WDF_IO_QUEUE_CONFIG queueConfig;

    PAGED_CODE();

    //
    // Configure a default queue so that requests that are not
    // configure-fowarded using WdfDeviceConfigureRequestDispatching to goto
    // other queues get dispatched here.
    //
    WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(
         &queueConfig,
        WdfIoQueueDispatchParallel
        );

    queueConfig.EvtIoDeviceControl = driverEvtIoDeviceControl;
    queueConfig.EvtIoStop = driverEvtIoStop;

	status = WdfIoQueueCreate(Device, &queueConfig, WDF_NO_OBJECT_ATTRIBUTES, &queue);

    if(!NT_SUCCESS(status))
	{
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "WdfIoQueueCreate failed %!STATUS!", status);
        return status;
    }

    return status;
}

VOID
driverEvtIoDeviceControl(
    _In_ WDFQUEUE Queue,
    _In_ WDFREQUEST Request,
    _In_ size_t OutputBufferLength,
    _In_ size_t InputBufferLength,
    _In_ ULONG IoControlCode
    )
/*++

Routine Description:

    This event is invoked when the framework receives IRP_MJ_DEVICE_CONTROL request.

Arguments:

    Queue -  Handle to the framework queue object that is associated with the
             I/O request.

    Request - Handle to a framework request object.

    OutputBufferLength - Size of the output buffer in bytes

    InputBufferLength - Size of the input buffer in bytes

    IoControlCode - I/O control code.

Return Value:

    VOID

--*/
{
    TraceEvents(TRACE_LEVEL_INFORMATION, 
                TRACE_QUEUE, 
                "%!FUNC! Queue 0x%p, Request 0x%p OutputBufferLength %d InputBufferLength %d IoControlCode %d", 
                Queue, Request, (int) OutputBufferLength, (int) InputBufferLength, IoControlCode);

	NTSTATUS status = STATUS_SUCCESS;
	KPROCESSOR_MODE requestorMode;
	WDF_REQUEST_PARAMETERS RequestParams;
	PREMOTE_DEVICE_INFO pRemoteDevice;
	WDFDEVICE device;
	PDEVICE_CONTEXT pDeviceContext;
	INT16 Port_Number = 1;
	PROOTHUB_CONTEXT pRoothubContext;

	do
	{
		//
		// Check that the request is coming from user mode
		//
		requestorMode = WdfRequestGetRequestorMode(Request);
		if (requestorMode != UserMode)
		{
			TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "Invalid RequestorMode %d", requestorMode);
			status = STATUS_INVALID_DEVICE_REQUEST;
			break;
		}

		switch (IoControlCode)
		{
			case IOCTL_VBUS_SEANXS_ATTACH_REMOTE_DEVICE:
			{
				TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_QUEUE, "IOCTL_VHCI_SEAN_ATTACH_REMOTE_DEVICE");
				WDF_REQUEST_PARAMETERS_INIT(&RequestParams);
				WdfRequestGetParameters(Request, &RequestParams);
				device = WdfIoQueueGetDevice(Queue);

				if (InputBufferLength == 0)
				{
					//status = AttachRemoteDevice("192.168.10.195", "1-1.1", device);
					Port_Number = 1;
					RtlStringCbCopyA(pDeviceContext->pRemteDevInfo[Port_Number-1].IpAddr, IP_DEV_BUF_SIZE, pRemoteDevice->IpAddr);
					RtlStringCbCopyA(pDeviceContext->pRemteDevInfo[Port_Number-1].Device, IP_DEV_BUF_SIZE, pRemoteDevice->Device);

					pRoothubContext = GetRootHubContext(pDeviceContext->USBRootHub);
					pRoothubContext->mPortStatusArray[Port_Number - 1].port_status |= 1;
					pRoothubContext->mPortStatusArray[Port_Number - 1].c_port_status |= 1;
					pRoothubContext->PortchgBits |= 1 << Port_Number;

					WdfWorkItemEnqueue(pDeviceContext->IoCtrl_WrokItem);
					status = STATUS_SUCCESS;
					break;
				}
				else if (InputBufferLength != sizeof(REMOTE_DEVICE_INFO))
				{
					status = STATUS_INVALID_PARAMETER;
					break;
				}
				else
				{
					status = WdfRequestRetrieveInputBuffer(Request, InputBufferLength, (PVOID*)&pRemoteDevice, NULL);
					if (!NT_SUCCESS(status))
					{
						status = STATUS_INVALID_PARAMETER;
						break;
					}

					if (pRemoteDevice->Port_Number > PORT_COUNT)
					{
						status = STATUS_INVALID_PARAMETER;
						break;
					}

					pDeviceContext = DeviceGetContext(device);
					if (pRemoteDevice->Port_Number == 0)
					{
						for (Port_Number = 0; Port_Number < PORT_COUNT; Port_Number++)
						{
							if (pDeviceContext->pRemteDevInfo[Port_Number].Device[0] == '\0')
							{
								break;
							}
						}
						if (Port_Number == PORT_COUNT)
						{
							status = STATUS_UNSUCCESSFUL;
							break;
						}
					}
					else
					{
						Port_Number = pRemoteDevice->Port_Number - 1;
					}

					RtlStringCbCopyA(pDeviceContext->pRemteDevInfo[Port_Number].IpAddr, IP_DEV_BUF_SIZE, pRemoteDevice->IpAddr);
					RtlStringCbCopyA(pDeviceContext->pRemteDevInfo[Port_Number].Device, IP_DEV_BUF_SIZE, pRemoteDevice->Device);

					pRoothubContext = GetRootHubContext(pDeviceContext->USBRootHub);
					pRoothubContext->mPortStatusArray[Port_Number].port_status |= 1;
					pRoothubContext->mPortStatusArray[Port_Number].c_port_status |= 1;
					pRoothubContext->PortchgBits |= 1 << (Port_Number + 1);

					WdfWorkItemEnqueue(pDeviceContext->IoCtrl_WrokItem);
					status = STATUS_SUCCESS;
					break;
				}			
			}
			case IOCTL_VBUS_SEANXS_DETACH_REMOTE_DEVICE:
			{
				break;
			}
			default:
			{
				TraceEvents(TRACE_LEVEL_WARNING, TRACE_QUEUE, "Unsupported IoControlCode:%#x", IoControlCode);
				status = STATUS_INVALID_DEVICE_REQUEST;
				break;
			}
		}
	} while (FALSE);

    WdfRequestComplete(Request, status);
    return;
}

VOID
driverEvtIoStop(
    _In_ WDFQUEUE Queue,
    _In_ WDFREQUEST Request,
    _In_ ULONG ActionFlags
)
/*++

Routine Description:

    This event is invoked for a power-managed queue before the device leaves the working state (D0).

Arguments:

    Queue -  Handle to the framework queue object that is associated with the
             I/O request.

    Request - Handle to a framework request object.

    ActionFlags - A bitwise OR of one or more WDF_REQUEST_STOP_ACTION_FLAGS-typed flags
                  that identify the reason that the callback function is being called
                  and whether the request is cancelable.

Return Value:

    VOID

--*/
{
    TraceEvents(TRACE_LEVEL_INFORMATION, 
                TRACE_QUEUE, 
                "%!FUNC! Queue 0x%p, Request 0x%p ActionFlags %d", 
                Queue, Request, ActionFlags);

    //
    // In most cases, the EvtIoStop callback function completes, cancels, or postpones
    // further processing of the I/O request.
    //
    // Typically, the driver uses the following rules:
    //
    // - If the driver owns the I/O request, it calls WdfRequestUnmarkCancelable
    //   (if the request is cancelable) and either calls WdfRequestStopAcknowledge
    //   with a Requeue value of TRUE, or it calls WdfRequestComplete with a
    //   completion status value of STATUS_SUCCESS or STATUS_CANCELLED.
    //
    //   Before it can call these methods safely, the driver must make sure that
    //   its implementation of EvtIoStop has exclusive access to the request.
    //
    //   In order to do that, the driver must synchronize access to the request
    //   to prevent other threads from manipulating the request concurrently.
    //   The synchronization method you choose will depend on your driver's design.
    //
    //   For example, if the request is held in a shared context, the EvtIoStop callback
    //   might acquire an internal driver lock, take the request from the shared context,
    //   and then release the lock. At this point, the EvtIoStop callback owns the request
    //   and can safely complete or requeue the request.
    //
    // - If the driver has forwarded the I/O request to an I/O target, it either calls
    //   WdfRequestCancelSentRequest to attempt to cancel the request, or it postpones
    //   further processing of the request and calls WdfRequestStopAcknowledge with
    //   a Requeue value of FALSE.
    //
    // A driver might choose to take no action in EvtIoStop for requests that are
    // guaranteed to complete in a small amount of time.
    //
    // In this case, the framework waits until the specified request is complete
    // before moving the device (or system) to a lower power state or removing the device.
    // Potentially, this inaction can prevent a system from entering its hibernation state
    // or another low system power state. In extreme cases, it can cause the system
    // to crash with bugcheck code 9F.
    //

    return;
}

VOID
DefaultEndpoint_QueuqReady(
	__in
	WDFQUEUE    Queue,
	__in
	WDFCONTEXT  Context
)
{
	UNREFERENCED_PARAMETER(Queue);
	UNREFERENCED_PARAMETER(Context);
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_QUEUE, "%!FUNC! Entry");
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_QUEUE, "%!FUNC! Exit");
	return;
}

VOID
Endpoint_QueuqReady(
	__in
	WDFQUEUE    Queue,
	__in
	WDFCONTEXT  Context
)
{
	UNREFERENCED_PARAMETER(Queue);
	UNREFERENCED_PARAMETER(Context);
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_QUEUE, "%!FUNC! Entry");
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_QUEUE, "%!FUNC! Exit");
	return;
}

VOID UrbIoCanceledOnQueue(
	_In_ WDFQUEUE   Queue,
	_In_ WDFREQUEST Request
)
{
	UNREFERENCED_PARAMETER(Queue);
	UNREFERENCED_PARAMETER(Request);
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_QUEUE, "%!FUNC! Entry");
	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_QUEUE, "%!FUNC! Exit");
	return;
}