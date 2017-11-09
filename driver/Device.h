/*++

Module Name:

    device.h

Abstract:

    This file contains the device definitions.

Environment:

    Kernel-mode Driver Framework

--*/
#ifndef _VBUS_SEANXS_DEVICE_H
#define _VBUS_SEANXS_DEVICE_H

#include "common.h"
#include "Public.h"

EXTERN_C_START

//
// The device context performs the same job as
// a WDM device extension in the driver frameworks
//
typedef struct _DEVICE_CONTEXT
{
	WDFTIMER RootHub_Timer;
	WDFWORKITEM IoCtrl_WrokItem;
	WDFDPC UrbDpc;
	UCXCONTROLLER USBController;
	UCXROOTHUB USBRootHub;
	WDFDPC CompleteUrbDpc;
	WDFQUEUE CompleteUrbQueue;
	PREMOTE_DEVICE_INFO pRemteDevInfo;
} DEVICE_CONTEXT, *PDEVICE_CONTEXT;

//
// This macro will generate an inline function called DeviceGetContext
// which will be used to get a pointer to the device context memory
// in a type safe manner.
//
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, DeviceGetContext)

//
// Function to initialize the device and its callbacks
//
NTSTATUS
driverCreateDevice(
    _Inout_ PWDFDEVICE_INIT DeviceInit
    );

EVT_WDF_OBJECT_CONTEXT_DESTROY Device_EvtDestroyCallback;

EXTERN_C_END

#endif // !_VBUS_SEANXS_DEVICE_H
