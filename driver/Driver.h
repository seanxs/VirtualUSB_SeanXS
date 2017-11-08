/*++

Module Name:

    driver.h

Abstract:

    This file contains the driver definitions.

Environment:

    Kernel-mode Driver Framework

--*/


#include "common.h"

EXTERN_C_START

typedef struct _DRIVER_CONTEXT
{
	WSK_REGISTRATION WskRegistration;
	WSK_PROVIDER_NPI WskProviderNpi;
	WSK_CLIENT_NPI wskClientNpi;
} DRIVER_CONTEXT, *PDRIVER_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DRIVER_CONTEXT, GetDriverContext)

//
// WDFDRIVER Events
//

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD driverEvtDeviceAdd;
EVT_WDF_DRIVER_UNLOAD driverEvtDriverUnload;
EVT_WDF_OBJECT_CONTEXT_CLEANUP driverEvtDriverContextCleanup;

EXTERN_C_END
