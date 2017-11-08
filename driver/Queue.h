/*++

Module Name:

    queue.h

Abstract:

    This file contains the queue definitions.

Environment:

    Kernel-mode Driver Framework

--*/
#ifndef _VBUS_SEANXS_QUEUE_H
#define _VBUS_SEANXS_QUEUE_H

#include "common.h"

EXTERN_C_START

//
// This is the context that can be placed per queue
// and would contain per queue information.
//
typedef struct _QUEUE_CONTEXT {

    ULONG PrivateDeviceData;  // just a placeholder

} QUEUE_CONTEXT, *PQUEUE_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(QUEUE_CONTEXT, QueueGetContext)

NTSTATUS
driverQueueInitialize(
    _In_ WDFDEVICE Device
    );

//
// Events from the IoQueue object
//
EVT_WDF_IO_QUEUE_IO_DEVICE_CONTROL driverEvtIoDeviceControl;
EVT_WDF_IO_QUEUE_IO_STOP driverEvtIoStop;

EXTERN_C_END

#endif // !_VBUS_SEANXS_QUEUE_H
