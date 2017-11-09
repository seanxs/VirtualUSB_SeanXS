#pragma once
#ifndef _VBUS_SEANXS_ROOTHUB_H
#define _VBUS_SEANXS_ROOTHUB_H

#include "common.h"

EXTERN_C_START

typedef struct _UCX_ROOTHUB_PORT_STATUS
{
	ULONG Index;
	USHORT port_status;
	USHORT c_port_status;
	ULONG ext_port_status;
} UCX_ROOTHUB_PORT_STATUS, *PUCX_ROOTHUB_PORT_STATUS;

typedef struct _ROOTHUB_CONTEXT
{
	WDFDEVICE WdfDevice;
	UINT8 NumberOf20Ports;
	UINT8 NumberOf30Ports;
	USHORT U1DeviceExitLatency;
	USHORT U2DeviceExitLatency;
	UCX_ROOTHUB_PORT_STATUS mPortStatusArray[USB20_PORTS + USB30_PORTS];
	UINT32 PortchgBits;
} ROOTHUB_CONTEXT, *PROOTHUB_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(ROOTHUB_CONTEXT, GetRootHubContext)

NTSTATUS
CreateRoothub(
	__in
	WDFDEVICE device
);

#if 0
EVT_UCX_ROOTHUB_CONTROL_URB RootHub_EvtRootHubClearHubFeature;
EVT_UCX_ROOTHUB_CONTROL_URB RootHub_EvtRootHubClearPortFeature;
EVT_UCX_ROOTHUB_CONTROL_URB RootHub_EvtRootHubGetHubStatus;
EVT_UCX_ROOTHUB_CONTROL_URB RootHub_EvtRootHubGetPortStatus;
EVT_UCX_ROOTHUB_CONTROL_URB RootHub_EvtRootHubSetHubFeature;
EVT_UCX_ROOTHUB_CONTROL_URB RootHub_EvtRootHubSetPortFeature;
EVT_UCX_ROOTHUB_CONTROL_URB RootHub_EvtRootHubGetPortErrorCount;
#endif
EVT_UCX_ROOTHUB_CONTROL_URB RootHub_EvtRootHubControlUrb;
EVT_UCX_ROOTHUB_INTERRUPT_TX RootHub_EvtRootHubInterruptTx;
EVT_UCX_ROOTHUB_GET_INFO RootHub_EvtRootHubGetInfo;
EVT_UCX_ROOTHUB_GET_20PORT_INFO RootHub_EvtRootHubGet20PortInfo;
EVT_UCX_ROOTHUB_GET_30PORT_INFO RootHub_EvtRootHubGet30PortInfo;

EXTERN_C_END

#endif // !_VBUS_SEANXS_ROOTHUB_H
