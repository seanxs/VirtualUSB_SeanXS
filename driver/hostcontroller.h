#pragma once
#ifndef _VBUS_SEANXS_HOSTCONTROLLER_H
#define _VBUS_SEANXS_HOSTCONTROLLER_H

#include "common.h"

EXTERN_C_START

typedef struct _HOST_CONTROLLER_CONTEXT
{
	WDFDEVICE WdfDevice;
	//UCXROOTHUB UcxRootHub;
} HOST_CONTROLLER_CONTEXT, *PHOST_CONTROLLER_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(HOST_CONTROLLER_CONTEXT, GetHostControllerContext)

NTSTATUS
CreateHostController(
	__in
	WDFDEVICE device
);

EVT_UCX_CONTROLLER_QUERY_USB_CAPABILITY Controller_EvtControllerQueryUsbCapability;
EVT_UCX_CONTROLLER_USBDEVICE_ADD Controller_EvtControllerUsbDeviceAdd;
EVT_UCX_CONTROLLER_RESET Controller_EvtControllerReset;
EVT_UCX_CONTROLLER_GET_CURRENT_FRAMENUMBER Controller_EvtGetCurrentFrameNumber;

EXTERN_C_END

#endif // !_VBUS_SEANXS_HOSTCONTROLLER_H
