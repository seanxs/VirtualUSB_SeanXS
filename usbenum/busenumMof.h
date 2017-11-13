#ifndef _busenumMof_h_
#define _busenumMof_h_

// ToasterBusInformation - ToasterBusInformation
// Toaster Bus driver information
#define ToasterBusInformationGuid \
    { 0x0006a660,0x8f12,0x11d2, { 0xb8,0x54,0x00,0xc0,0x4f,0xad,0x51,0x71 } }

#if ! (defined(MIDL_PASS))
DEFINE_GUID(ToasterBusInformation_GUID, \
            0x0006a660,0x8f12,0x11d2,0xb8,0x54,0x00,0xc0,0x4f,0xad,0x51,0x71);
#endif


typedef struct _ToasterBusInformation
{
    // Number of errors that occurred on this device
    ULONG ErrorCount;
    #define ToasterBusInformation_ErrorCount_SIZE sizeof(ULONG)
    #define ToasterBusInformation_ErrorCount_ID 1

    // The DebugPrintLevel property indicates the debug output level of toaster bus device.
    ULONG DebugPrintLevel;
    #define ToasterBusInformation_DebugPrintLevel_SIZE sizeof(ULONG)
    #define ToasterBusInformation_DebugPrintLevel_ID 2

} ToasterBusInformation, *PToasterBusInformation;

#define ToasterBusInformation_SIZE (FIELD_OFFSET(ToasterBusInformation, DebugPrintLevel) + ToasterBusInformation_DebugPrintLevel_SIZE)

#endif
