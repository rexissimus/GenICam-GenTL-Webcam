#include <string.h>
#include <stdlib.h>
#include "webcam.gentl.h"
#include "webcam.h"

/* C API Interface Functions */
GC_API GCGetInfo(TL_INFO_CMD iInfoCmd, INFO_DATATYPE* piType, void* pBuffer, size_t* piSize)
{
	
	*piType = INFO_DATATYPE_STRING;
	switch (iInfoCmd)
	{
	case TL_INFO_ID: /* STRING    Transport layer ID. */
		return CopyString(pBuffer, "My Webcam TL", piSize);
	case TL_INFO_VENDOR: /* STRING    Transport layer vendor name. */
		return CopyString(pBuffer, "MyCompany", piSize);
	case TL_INFO_MODEL: /* STRING    Transport layer model name. */
		return CopyString(pBuffer, "My Webcam TL", piSize);
	case TL_INFO_VERSION: /* STRING    Transport layer version. */
		return CopyString(pBuffer, "1.0", piSize);
	case TL_INFO_TLTYPE: /* STRING    Transport layer technology that is supported. */
		return CopyString(pBuffer, "Custom", piSize);
	case TL_INFO_NAME: /* STRING    File name including extension of the library. */
		return CopyString(pBuffer, "webcam.gentl.x64.clt", piSize);
	case TL_INFO_PATHNAME: /* STRING    Full path including file name and extension of the library. */
		return CopyString(pBuffer, "Not really necessary?", piSize);
	case TL_INFO_DISPLAYNAME: /* STRING    User readable name of the device. If this is not defined in the device this should be VENDOR MODEL (ID). */
		return CopyString(pBuffer, "My Webcam TL", piSize);
	case TL_INFO_CHAR_ENCODING: /* INT32     Reporting the char encoding used by this Producer, GenTL v1.4 */
		*(int32_t*)pBuffer = TL_CHAR_ENCODING_ASCII;
		*piType = INFO_DATATYPE_INT32;
		break;
	case TL_INFO_GENTL_VER_MAJOR: /* UINT32    Major number of the GenTL spec this producer complies with, GenTL v1.5 */
		*(uint32_t*)pBuffer = 1;
		*piType = INFO_DATATYPE_INT32;
		break;
	case TL_INFO_GENTL_VER_MINOR: /* UINT32    Minor number of the GenTL spec this producer complies with, GenTL v1.5 */
		*(uint32_t*)pBuffer = 5;
		*piType = INFO_DATATYPE_INT32;
		break;
	default:
		return GC_ERR_NOT_IMPLEMENTED;
	}
	return GC_ERR_SUCCESS;
}

GC_API GCGetLastError(GC_ERROR* piErrorCode, char* sErrText, size_t* piSize)
{
	*piErrorCode = GC_ERR_SUCCESS;
	return CopyString(sErrText, "", piSize);
}

GC_API GCInitLib(void)
{
	webcam_init();
	return GC_ERR_SUCCESS;
}

GC_API GCCloseLib(void)
{
	webcam_finalize();
	return GC_ERR_SUCCESS;
}

GC_API GCReadPort(PORT_HANDLE hPort, uint64_t iAddress, void* pBuffer, size_t* piSize)
{
	if (!webcam_read_memory((uint32_t)iAddress, (uint32_t)*piSize, pBuffer))
		return GC_ERR_ERROR;
	return GC_ERR_SUCCESS;
}

GC_API GCWritePort(PORT_HANDLE hPort, uint64_t iAddress, const void* pBuffer, size_t* piSize)
{
	if (!webcam_write_memory((uint32_t)iAddress, (uint32_t)*piSize, pBuffer))
		return GC_ERR_ERROR;
	return GC_ERR_SUCCESS;
}

GC_API GCGetPortURL(PORT_HANDLE hPort, char* sURL, size_t* piSize)
{
	size_t size;
	size = strlen((char*)registry + ARV_GVBS_XML_URL_0_OFFSET);
	if(!sURL)
	{
		*piSize = size + 1;
		return GC_ERR_SUCCESS;
	}
	strcpy(sURL, (char*)registry + ARV_GVBS_XML_URL_0_OFFSET);
	sURL[size + 1] = '\0'; // Double null breaks
	return GC_ERR_SUCCESS;
}

GC_API GCGetPortInfo(PORT_HANDLE hPort, PORT_INFO_CMD iInfoCmd, INFO_DATATYPE* piType, void* pBuffer, size_t* piSize)
{
	*piType = INFO_DATATYPE_STRING;
	switch (iInfoCmd)
	{
	case PORT_INFO_ID: /* STRING     Unique ID of the port. */
		return CopyString(pBuffer, "WebcamPort1", piSize);
	case PORT_INFO_VENDOR: /* STRING     Port vendor name. */
		return CopyString(pBuffer, "MyCompany", piSize);
	case PORT_INFO_MODEL: /* STRING     Port model name. */
		return CopyString(pBuffer, "WebcamPort", piSize);
	case PORT_INFO_TLTYPE: /* STRING     Transport layer technology that is supported. */
		return CopyString(pBuffer, "Custom", piSize);
	case PORT_INFO_MODULE: /* STRING     GenTL Module the port refers to. */
		return CopyString(pBuffer, "TLDevice", piSize);
	case PORT_INFO_LITTLE_ENDIAN: /* BOOL8      Flag indicating that the port data is little endian. */
		*(uint8_t*)pBuffer = 1;
		*piType = INFO_DATATYPE_BOOL8;
		break;
	case PORT_INFO_BIG_ENDIAN: /* BOOL8      Flag indicating that the port data is big endian. */
		*(uint8_t*)pBuffer = 0;
		*piType = INFO_DATATYPE_BOOL8;
		break;
	case PORT_INFO_ACCESS_READ: /* BOOL8      Port has read access. */
		*(uint8_t*)pBuffer = 1;
		*piType = INFO_DATATYPE_BOOL8;
		break;
	case PORT_INFO_ACCESS_WRITE: /* BOOL8      Port has write access. */
		*(uint8_t*)pBuffer = 1;
		*piType = INFO_DATATYPE_BOOL8;
		break;
	case PORT_INFO_ACCESS_NA: /* BOOL8      Port is not accessible. */
		*(uint8_t*)pBuffer = 0;
		*piType = INFO_DATATYPE_BOOL8;
		break;
	case PORT_INFO_ACCESS_NI: /* BOOL8      Port is not implemented. */
		*(uint8_t*)pBuffer = 0;
		*piType = INFO_DATATYPE_BOOL8;
		break;
	case PORT_INFO_VERSION: /* STRING     Version of the port. */
		return CopyString(pBuffer, "1.0", piSize);
	case PORT_INFO_PORTNAME: /* STRING     Name of the port as referenced in the XML description. */
		return CopyString(pBuffer, "Device", piSize);
	default:
		return GC_ERR_NOT_IMPLEMENTED;
	}
	return GC_ERR_SUCCESS;
}

GC_API GCRegisterEvent(EVENTSRC_HANDLE hEventSrc, EVENT_TYPE iEventID, EVENT_HANDLE* phEvent)
{
	*phEvent = (void*)iEventID;
	return GC_ERR_SUCCESS;
}

GC_API GCUnregisterEvent(EVENTSRC_HANDLE hEventSrc, EVENT_TYPE iEventID)
{
	return GC_ERR_SUCCESS;
}

GC_API EventGetData(EVENT_HANDLE hEvent, void* pBuffer, size_t* piSize, uint64_t iTimeout)
{
	if (!pBuffer)
	{
		return GC_ERR_SUCCESS;
	}
	if (hEvent == EVENT_REMOTE_DEVICE)
	{
		webcam_fill_buffer();
		memcpy(pBuffer, &nbd, sizeof(nbd));
		*piSize = sizeof(nbd);
		return GC_ERR_SUCCESS;
	}
	if (hEvent == EVENT_NEW_BUFFER)
	{
		webcam_fill_buffer();
		memcpy(pBuffer, &nbd, sizeof(nbd));
		*piSize = sizeof(nbd);
		return GC_ERR_SUCCESS;
//		webcam_fill_buffer();
//		*(void**)pBuffer = buffer.data;
//		*piSize = buffer.size;
//		return GC_ERR_SUCCESS;
	}
}

GC_API EventGetDataInfo(EVENT_HANDLE hEvent, const void* pInBuffer, size_t iInSize, EVENT_DATA_INFO_CMD iInfoCmd, INFO_DATATYPE* piType, void* pOutBuffer, size_t* piOutSize)
{
	*piType = INFO_DATATYPE_STRING;
	switch (iInfoCmd)
	{
	case EVENT_DATA_ID: /* Event specific    Unique Event ID (String or Number)*/
		*(void**)pOutBuffer = buffer.data;
		*piType = INFO_DATATYPE_PTR;
		break;
	case EVENT_DATA_VALUE: /* Event specific Data */
		return GC_ERR_NOT_IMPLEMENTED;
	case EVENT_DATA_NUMID: /* UINT64   Numeric representation of the unique Event ID, GenTL v1.3. */
		*(uint64_t*)pOutBuffer = 1;
		*piType = INFO_DATATYPE_UINT64;
	default:
		return GC_ERR_NOT_IMPLEMENTED;
	}
	return GC_ERR_SUCCESS;
}

GC_API EventGetInfo(EVENT_HANDLE hEvent, EVENT_INFO_CMD iInfoCmd, INFO_DATATYPE* piType, void* pBuffer, size_t* piSize)
{
	*piType = INFO_DATATYPE_STRING;
	switch (iInfoCmd)
	{
	case EVENT_EVENT_TYPE: /* INT32      The event type of the event handle (EVENT_TYPE enum value). */
		*(int32_t*)pBuffer = (int64_t)hEvent;
		*piType = INFO_DATATYPE_INT32;
		break;
	case EVENT_NUM_IN_QUEUE: /* SIZET      Number of events in the event data queue. */
		*(size_t*)pBuffer = 1;
		*piType = INFO_DATATYPE_SIZET;
		break;
	case EVENT_NUM_FIRED: /* UINT64     Number of events that were fired since the creation of the module. */
		*(size_t*)pBuffer = 0;
		*piType = INFO_DATATYPE_SIZET;
		break;
	case EVENT_SIZE_MAX: /* SIZET      Max size of data carried with an event in bytes. GenTL v1.2 */
		*(size_t*)pBuffer = 0;
		*piType = INFO_DATATYPE_SIZET;
		break;
	case EVENT_INFO_DATA_SIZE_MAX: /* SIZET      Max size of data provided through EventGetDataInfo in bytes. GenTL v1.2 */
		*(size_t*)pBuffer = 4;
		*piType = INFO_DATATYPE_SIZET;
		break;
	default:
		return GC_ERR_NOT_IMPLEMENTED;
	}
	return GC_ERR_SUCCESS;
}

GC_API EventFlush(EVENT_HANDLE hEvent)
{
	return GC_ERR_SUCCESS;
}

GC_API EventKill(EVENT_HANDLE hEvent)
{
	return GC_ERR_SUCCESS;
}

GC_API TLOpen(TL_HANDLE* phTL)
{
	*phTL = (TL_HANDLE)1;
	return GC_ERR_SUCCESS;
}

GC_API TLClose(TL_HANDLE hTL)
{
	return GC_ERR_SUCCESS;
}

GC_API TLGetInfo(TL_HANDLE hTL, TL_INFO_CMD iInfoCmd, INFO_DATATYPE* piType, void* pBuffer, size_t* piSize)
{
	return GCGetInfo(iInfoCmd, piType, pBuffer, piSize);
}

GC_API TLGetNumInterfaces(TL_HANDLE hTL, uint32_t* piNumIfaces)
{
	// Always 1 for now
	*piNumIfaces = 1;
	return GC_ERR_SUCCESS;
}

GC_API TLGetInterfaceID(TL_HANDLE hTL, uint32_t iIndex, char* sID, size_t* piSize)
{
	return CopyString(sID, "First Webcam", piSize);
}

GC_API TLGetInterfaceInfo(TL_HANDLE hTL, const char* sIfaceID, INTERFACE_INFO_CMD iInfoCmd, INFO_DATATYPE* piType, void* pBuffer, size_t* piSize)
{
	*piType = INFO_DATATYPE_STRING;
	switch (iInfoCmd)
	{
	case INTERFACE_INFO_ID:/* STRING     Unique ID of the interface. */
		return CopyString(pBuffer, "First Webcam", piSize);
	case INTERFACE_INFO_DISPLAYNAME: /* STRING     User readable name of the interface. */
		return CopyString(pBuffer, "First Webcam", piSize);
	case INTERFACE_INFO_TLTYPE: /* STRING     Transport layer technology that is supported. */
		return CopyString(pBuffer, "Custom", piSize);
	default:
		return GC_ERR_NOT_IMPLEMENTED;
	}
	return GC_ERR_SUCCESS;
}

GC_API TLOpenInterface(TL_HANDLE hTL, const char* sIfaceID, IF_HANDLE* phIface)
{
	return GC_ERR_SUCCESS;
}

GC_API TLUpdateInterfaceList(TL_HANDLE hTL, bool8_t* pbChanged, uint64_t iTimeout)
{
	return GC_ERR_SUCCESS;
}

GC_API IFClose(IF_HANDLE hIface)
{
	return GC_ERR_SUCCESS;
}

GC_API IFGetInfo(IF_HANDLE hIface, INTERFACE_INFO_CMD iInfoCmd, INFO_DATATYPE* piType, void* pBuffer, size_t* piSize)
{
	return TLGetInterfaceInfo((TL_HANDLE)1, "First Webcam", iInfoCmd, piType, pBuffer, piSize);
}

GC_API IFGetNumDevices(IF_HANDLE hIface, uint32_t* piNumDevices)
{
	*piNumDevices = 1;
	return GC_ERR_SUCCESS;
}

GC_API IFGetDeviceID(IF_HANDLE hIface, uint32_t iIndex, char* sIDeviceID, size_t* piSize)
{
	return CopyString(sIDeviceID, "My Webcam", piSize);
}

GC_API IFUpdateDeviceList(IF_HANDLE hIface, bool8_t* pbChanged, uint64_t iTimeout)
{
	return GC_ERR_SUCCESS;
}

GC_API IFGetDeviceInfo(IF_HANDLE hIface, const char* sDeviceID, DEVICE_INFO_CMD iInfoCmd, INFO_DATATYPE* piType, void* pBuffer, size_t* piSize)
{
	*piType = INFO_DATATYPE_STRING;
	switch (iInfoCmd)
	{
	case DEVICE_INFO_ID: /* STRING     Unique ID of the device. */
		return CopyString(pBuffer, "My Webcam", piSize);
	case DEVICE_INFO_VENDOR: /* STRING     Device vendor name. */
		return CopyString(pBuffer, "MyCompany", piSize);
	case DEVICE_INFO_MODEL: /* STRING     Device model name. */
		return CopyString(pBuffer, "Dell Webcam", piSize);
	case DEVICE_INFO_TLTYPE: /* STRING     Transport layer technology that is supported. */
		return CopyString(pBuffer, "Custom", piSize);
	case DEVICE_INFO_DISPLAYNAME: /* STRING     String containing a display name for the device ( including a unique id ) */
		return CopyString(pBuffer, "My Webcam", piSize);
	case DEVICE_INFO_ACCESS_STATUS: /* INT32      Gets the access mode the GenTL Producer has on the opened device. (DEVICE_ACCESS_STATUS enumeration value). */
		*(int32_t*)pBuffer = device_access_status;
		*piType = INFO_DATATYPE_INT32;
		break;
	case DEVICE_INFO_USER_DEFINED_NAME: /* STRING     String containing the user defined name, GenTL v1.4  */
		return CopyString(pBuffer, "My Webcam", piSize);
	case DEVICE_INFO_SERIAL_NUMBER: /* STRING     String containing the device's serial number, GenTL v1.4  */
		return CopyString(pBuffer, "PI314", piSize);
	case DEVICE_INFO_VERSION: /* STRING     String containing the device version, GenTL v1.4  */
		return CopyString(pBuffer, "1.0", piSize);
	case DEVICE_INFO_TIMESTAMP_FREQUENCY: /* UINT64     Tick-frequency of the time stamp clock, GenTL v1.4 */
		*(uint64_t*)pBuffer = 1;
		*piType = INFO_DATATYPE_UINT64;
		break;
	default:
		return GC_ERR_NOT_IMPLEMENTED;
	}
	return GC_ERR_SUCCESS;
}

GC_API IFOpenDevice(IF_HANDLE hIface, const char* sDeviceID, DEVICE_ACCESS_FLAGS iOpenFlags, DEV_HANDLE* phDevice)
{
	device_access_status = DEVICE_ACCESS_STATUS_OPEN_READWRITE;
	*phDevice = (DEV_HANDLE)1;
	return GC_ERR_SUCCESS;
}

GC_API DevGetPort(DEV_HANDLE hDevice, PORT_HANDLE* phRemoteDevice)
{
	*phRemoteDevice = (PORT_HANDLE)1;
	return GC_ERR_SUCCESS;
}

GC_API DevGetNumDataStreams(DEV_HANDLE hDevice, uint32_t* piNumDataStreams)
{
	*piNumDataStreams = 1;
	return GC_ERR_SUCCESS;
}

GC_API DevGetDataStreamID(DEV_HANDLE hDevice, uint32_t iIndex, char* sDataStreamID, size_t* piSize)
{
	return CopyString(sDataStreamID, "WebcamStream1", piSize);
}

GC_API DevOpenDataStream(DEV_HANDLE hDevice, const char* sDataStreamID, DS_HANDLE* phDataStream)
{
	*phDataStream = (PORT_HANDLE)1;
	return GC_ERR_SUCCESS;
}

GC_API DevGetInfo(DEV_HANDLE hDevice, DEVICE_INFO_CMD iInfoCmd, INFO_DATATYPE* piType, void* pBuffer, size_t* piSize)
{
	return IFGetDeviceInfo((IF_HANDLE)1, "WebcamStream1", iInfoCmd, piType, pBuffer, piSize);
}

GC_API DevClose(DEV_HANDLE hDevice)
{
	device_access_status = DEVICE_ACCESS_STATUS_READWRITE;
	return GC_ERR_SUCCESS;
}

GC_API DSAnnounceBuffer(DS_HANDLE hDataStream, void* pBuffer, size_t iSize, void* pPrivate, BUFFER_HANDLE* phBuffer)
{
	buffer.user_data = pPrivate;
	nbd.pUserPointer = pPrivate;
	if (consumer_buffer)
	{
		return GC_ERR_RESOURCE_IN_USE;
	}
	consumer_buffer = pBuffer;
	*phBuffer = (BUFFER_HANDLE)1;
	return GC_ERR_SUCCESS;
}

GC_API DSAllocAndAnnounceBuffer(DS_HANDLE hDataStream, size_t iSize, void* pPrivate, BUFFER_HANDLE* phBuffer)
{
	buffer.user_data = pPrivate;
	nbd.pUserPointer = pPrivate;
	*phBuffer = (BUFFER_HANDLE)1;
	if(consumer_buffer)
	{
		// cheat multiple buffer
		return GC_ERR_SUCCESS;
	}
	consumer_buffer = malloc(iSize);
	return GC_ERR_SUCCESS;
}

GC_API DSFlushQueue(DS_HANDLE hDataStream, ACQ_QUEUE_TYPE iOperation)
{
	return GC_ERR_SUCCESS;
}

GC_API DSStartAcquisition(DS_HANDLE hDataStream, ACQ_START_FLAGS iStartFlags, uint64_t iNumToAcquire)
{
	return GC_ERR_SUCCESS;
}

GC_API DSStopAcquisition(DS_HANDLE hDataStream, ACQ_STOP_FLAGS iStopFlags)
{
	return GC_ERR_SUCCESS;
}

GC_API DSGetInfo(DS_HANDLE hDataStream, STREAM_INFO_CMD iInfoCmd, INFO_DATATYPE* piType, void* pBuffer, size_t* piSize)
{
	*piType = INFO_DATATYPE_STRING;
	switch (iInfoCmd)
	{
	case STREAM_INFO_ID: /* STRING     Unique ID of the data stream. */
		return CopyString(pBuffer, "WebcamStream1", piSize);
	case STREAM_INFO_NUM_DELIVERED: /* UINT64     Number of delivered buffers since last acquisition start. */
		*(uint64_t*)pBuffer = 0;
		*piType = INFO_DATATYPE_UINT64;
		break;
	case STREAM_INFO_NUM_UNDERRUN: /* UINT64     Number of lost buffers due to queue underrun. */
		*(uint64_t*)pBuffer = 0;
		*piType = INFO_DATATYPE_UINT64;
		break;
	case STREAM_INFO_NUM_ANNOUNCED: /* SIZET      Number of announced buffers. */
		*(size_t*)pBuffer = 1;
		*piType = INFO_DATATYPE_SIZET;
		break;
	case STREAM_INFO_NUM_QUEUED: /* SIZET      Number of buffers in the input pool. */
		*(size_t*)pBuffer = 1;
		*piType = INFO_DATATYPE_SIZET;
		break;
	case STREAM_INFO_NUM_AWAIT_DELIVERY: /* SIZET      Number of buffers in the output queue. */
		*(size_t*)pBuffer = 1;
		*piType = INFO_DATATYPE_SIZET;
		break;
	case STREAM_INFO_NUM_STARTED: /* UINT64     Number of buffers started in the acquisition engine. */
		*(size_t*)pBuffer = 1;
		*piType = INFO_DATATYPE_UINT64;
		break;
	case STREAM_INFO_PAYLOAD_SIZE: /* SIZET      Size of the expected data in bytes. */
		*(size_t*)pBuffer = 512*512*1;
		*piType = INFO_DATATYPE_SIZET;
		break;
	case STREAM_INFO_IS_GRABBING: /* BOOL8      Flag indicating whether the acquisition engine is started or not. */
		*(size_t*)pBuffer = 1;
		*piType = INFO_DATATYPE_BOOL8;
		break;
	case STREAM_INFO_DEFINES_PAYLOADSIZE: /* BOOL8      Flag that indicated that this data stream defines a payload size independent from the remote device. */
		*(size_t*)pBuffer = 1;
		*piType = INFO_DATATYPE_BOOL8;
		break;
	case STREAM_INFO_TLTYPE: /* STRING     Transport layer technology that is supported. */
		return CopyString(pBuffer, "Custom", piSize);
	case STREAM_INFO_NUM_CHUNKS_MAX: /* SIZET      Max number of chunks in a buffer, if known. GenTL v1.3 */
		*(size_t*)pBuffer = 1;
		*piType = INFO_DATATYPE_SIZET;
		break;
	case STREAM_INFO_BUF_ANNOUNCE_MIN: /* SIZET      Min number of buffers to announce before acq can start, if known. GenTL v1.3 */
		*(size_t*)pBuffer = 1;
		*piType = INFO_DATATYPE_SIZET;
		break;
	case STREAM_INFO_BUF_ALIGNMENT: /* SIZET      Buffer alignment in bytes. GenTL v1.3 */
		*(size_t*)pBuffer = 1;
		*piType = INFO_DATATYPE_SIZET;
		break;
	default:
		return GC_ERR_NOT_IMPLEMENTED;
	}
	return GC_ERR_SUCCESS;
}

GC_API DSGetBufferID(DS_HANDLE hDataStream, uint32_t iIndex, BUFFER_HANDLE* phBuffer)
{
	*phBuffer = (BUFFER_HANDLE)1;
	return GC_ERR_SUCCESS;
}

GC_API DSClose(DS_HANDLE hDataStream)
{
	return GC_ERR_SUCCESS;
}

GC_API DSRevokeBuffer(DS_HANDLE hDataStream, BUFFER_HANDLE hBuffer, void** pBuffer, void** pPrivate)
{
	if (pBuffer)
	{
		*(void**)pBuffer = NULL;
	}
	if (pPrivate)
	{
		*(void**)pPrivate = NULL;
	}
	if (consumer_buffer)
	{
		free(consumer_buffer);
		consumer_buffer = NULL;
	}
	return GC_ERR_SUCCESS;
}

GC_API DSQueueBuffer(DS_HANDLE hDataStream, BUFFER_HANDLE hBuffer)
{
	return GC_ERR_SUCCESS;
}

GC_API DSGetBufferInfo(DS_HANDLE hDataStream, BUFFER_HANDLE hBuffer, BUFFER_INFO_CMD iInfoCmd, INFO_DATATYPE* piType, void* pBuffer, size_t* piSize)
{
	*piType = INFO_DATATYPE_STRING;
	switch (iInfoCmd)
	{
	case BUFFER_INFO_BASE: /* PTR        Base address of the buffer memory. */
		*(void**)pBuffer = consumer_buffer;
	   *piType = INFO_DATATYPE_PTR;
	   break;
	case BUFFER_INFO_SIZE: /* SIZET      Size of the buffer in bytes. */
		*(size_t*)pBuffer = 1;
		*piType = INFO_DATATYPE_SIZET;
		break;
	case BUFFER_INFO_USER_PTR: /* PTR        Private data pointer of the GenTL Consumer. */
		*(void**)pBuffer = buffer.user_data;
		*piType = INFO_DATATYPE_PTR;
		break;
	case BUFFER_INFO_TIMESTAMP: /* UINT64     Timestamp the buffer was acquired. */
		*(uint64_t*)pBuffer = 0;
		*piType = INFO_DATATYPE_UINT64;
		break;
	case BUFFER_INFO_NEW_DATA: /* BOOL8      Flag to indicate that the buffer contains new data since the last call. */
		*(int8_t*)pBuffer = 1;
		*piType = INFO_DATATYPE_BOOL8;
		break;
	case BUFFER_INFO_IS_QUEUED: /* BOOL8      Flag to indicate if the buffer is in the input pool or output queue. */
		*(int8_t*)pBuffer = 1;
		*piType = INFO_DATATYPE_BOOL8;
		break;
	case BUFFER_INFO_IS_ACQUIRING: /* BOOL8      Flag to indicate that the buffer is currently being filled with data. */
		*(int8_t*)pBuffer = 0;
		*piType = INFO_DATATYPE_BOOL8;
		break;
	case BUFFER_INFO_IS_INCOMPLETE: /* BOOL8      Flag to indicate that a buffer was filled but an error occurred during that process. */
		*(int8_t*)pBuffer = 0;
		*piType = INFO_DATATYPE_BOOL8;
		break;
	case BUFFER_INFO_TLTYPE: /* STRING     Transport layer technology that is supported. */
		return CopyString(pBuffer, "Custom", piSize);
	case BUFFER_INFO_SIZE_FILLED: /* SIZET      Number of bytes written into the buffer last time it has been filled. This value is reset to 0 when the buffer is placed into the Input Buffer Pool. */
		*(size_t*)pBuffer = 0;
		*piType = INFO_DATATYPE_SIZET;
		break;
	case BUFFER_INFO_WIDTH: /* SIZET      GenTL v1.2 */
		*(size_t*)pBuffer = 512;
		*piType = INFO_DATATYPE_SIZET;
		break;
	case BUFFER_INFO_HEIGHT: /* SIZET      GenTL v1.2 */
		*(size_t*)pBuffer =512;
		*piType = INFO_DATATYPE_SIZET;
		break;
	case BUFFER_INFO_XOFFSET: /* SIZET      GenTL v1.2 */
		*(size_t*)pBuffer = 0;
		*piType = INFO_DATATYPE_SIZET;
		break;
	case BUFFER_INFO_YOFFSET: /* SIZET      GenTL v1.2 */
		*(size_t*)pBuffer = 0;
		*piType = INFO_DATATYPE_SIZET;
		break;
	case BUFFER_INFO_XPADDING: /* SIZET      GenTL v1.2 */
		*(size_t*)pBuffer = 0;
		*piType = INFO_DATATYPE_SIZET;
		break;
	case BUFFER_INFO_YPADDING: /* SIZET      GenTL v1.2 */
		*(size_t*)pBuffer = 0;
		*piType = INFO_DATATYPE_SIZET;
		break;
	case BUFFER_INFO_FRAMEID: /* UINT64     GenTL v1.2 */
		*(uint64_t*)pBuffer = 0;
		*piType = INFO_DATATYPE_UINT64;
		break;
	case BUFFER_INFO_IMAGEPRESENT: /* BOOL8      GenTL v1.2 */
		*(uint8_t*)pBuffer = 1;
		*piType = INFO_DATATYPE_BOOL8;
		break;
	case BUFFER_INFO_IMAGEOFFSET: /* SIZET      GenTL v1.2 */
		*(size_t*)pBuffer = 0;
		*piType = INFO_DATATYPE_SIZET;
		break;
	case BUFFER_INFO_PAYLOADTYPE: /* SIZET      GenTL v1.2 */
		*(size_t*)pBuffer = PAYLOAD_TYPE_IMAGE;
		*piType = INFO_DATATYPE_SIZET;
		break;
	case BUFFER_INFO_PIXELFORMAT: /* UINT64     GenTL v1.2 */
		*(uint64_t*)pBuffer = ARV_PIXEL_FORMAT_MONO_8;
		*piType = INFO_DATATYPE_UINT64;
		break;
	case BUFFER_INFO_PIXELFORMAT_NAMESPACE: /* UINT64     GenTL v1.2 */
		*(uint64_t*)pBuffer = PIXELFORMAT_NAMESPACE_GEV;
		*piType = INFO_DATATYPE_UINT64;
		break;
	case BUFFER_INFO_DELIVERED_IMAGEHEIGHT: /* SIZET      GenTL v1.2 */
		*(size_t*)pBuffer = 512;
		*piType = INFO_DATATYPE_SIZET;
		break;
	case BUFFER_INFO_DELIVERED_CHUNKPAYLOADSIZE: /* SIZET      GenTL v1.2 */
		*(size_t*)pBuffer = 512*512*1;
		*piType = INFO_DATATYPE_SIZET;
		break;
	case BUFFER_INFO_CHUNKLAYOUTID: /* UINT64     GenTL v1.2 */
		*(uint64_t*)pBuffer = 1;
		*piType = INFO_DATATYPE_UINT64;
		break;
	case BUFFER_INFO_FILENAME: /* STRING     GenTL v1.2 */
		return CopyString(pBuffer, "", piSize);
	case BUFFER_INFO_PIXEL_ENDIANNESS: /* INT32      GenTL v1.4 */
		*(int32_t*)pBuffer = PIXELENDIANNESS_LITTLE;
		*piType = INFO_DATATYPE_INT32;
		break;
	case BUFFER_INFO_DATA_SIZE: /* SIZET      GenTL v1.4 */
		*(size_t*)pBuffer = 512 * 512 * 1;
		*piType = INFO_DATATYPE_SIZET;
		break;
	case BUFFER_INFO_TIMESTAMP_NS: /* UINT64     GenTL v1.4 */
		*(uint64_t*)pBuffer = 0;
		*piType = INFO_DATATYPE_UINT64;
		break;
	case BUFFER_INFO_DATA_LARGER_THAN_BUFFER: /* BOOL8      GenTL v1.4 */
		*(int8_t*)pBuffer = 0;
		*piType = INFO_DATATYPE_BOOL8;
		break;
	case BUFFER_INFO_CONTAINS_CHUNKDATA: /* BOOL8      GenTL v1.4 */
		*(int8_t*)pBuffer = 0;
		*piType = INFO_DATATYPE_BOOL8;
		break;
	default:
		return GC_ERR_NOT_IMPLEMENTED;
	}
	return GC_ERR_SUCCESS;
}

/* GenTL v1.1 */
GC_API GCGetNumPortURLs(PORT_HANDLE hPort, uint32_t* piNumURLs)
{
	*piNumURLs = 1;
	return GC_ERR_SUCCESS;
}

GC_API GCGetPortURLInfo(PORT_HANDLE hPort, uint32_t iURLIndex, URL_INFO_CMD iInfoCmd, INFO_DATATYPE* piType, void* pBuffer, size_t* piSize)
{
	*piType = INFO_DATATYPE_STRING;
	switch (iInfoCmd)
	{
	case URL_INFO_URL: /* STRING     URL as defined in chapter 4.1.2 GenTL v1.1  */
		return CopyString(pBuffer, (char*)registry + ARV_GVBS_XML_URL_0_OFFSET, piSize);
	case URL_INFO_SCHEMA_VER_MAJOR: /* INT32      Major version of the schema this URL refers to. GenTL v1.1 */
		*(int32_t*)pBuffer = 1;
		*piType = INFO_DATATYPE_INT32;
		break;
	case URL_INFO_SCHEMA_VER_MINOR: /* INT32      Minor version of the schema this URL refers to. GenTL v1.1 */
		*(int32_t*)pBuffer = 0;
		*piType = INFO_DATATYPE_INT32;
		break;
	case URL_INFO_FILE_VER_MAJOR: /* INT32      Major version of the XML-file this URL refers to. GenTL v1.1 */
		*(int32_t*)pBuffer = 1;
		*piType = INFO_DATATYPE_INT32;
		break;
	case URL_INFO_FILE_VER_MINOR: /* INT32      Minor version of the XML-file this URL refers to. GenTL v1.1 */
		*(int32_t*)pBuffer = 0;
		*piType = INFO_DATATYPE_INT32;
		break;
	case URL_INFO_FILE_VER_SUBMINOR: /* INT32      Subminor version of the XML-file this URL refers to. GenTL v1.1 */
		*(int32_t*)pBuffer = 0;
		*piType = INFO_DATATYPE_INT32;
		break;
	case URL_INFO_FILE_SHA1_HASH: /* BUFFER     160-bit SHA1 Hash code of XML-file. GenTL v1.4 */
		return CopyString(pBuffer, "COMPUTTHISIFUCAN", piSize);
	case URL_INFO_FILE_REGISTER_ADDRESS: /* UINT64     Register address in the device's register map. GenTL v1.5 */
		*(int64_t*)pBuffer = ARV_FAKE_CAMERA_MEMORY_SIZE;
		*piType = INFO_DATATYPE_UINT64;
		break;
	case URL_INFO_FILE_SIZE: /* UINT64     File size in bytes. GenTL v1.5 */
		*(int64_t*)pBuffer = genicam_xml_size;
		*piType = INFO_DATATYPE_UINT64;
		break;
	case URL_INFO_SCHEME: /* INT32      Scheme of the URL as defined in URL_SCHEME_IDS. GenTL v1.5 */
		*(int32_t*)pBuffer = URL_SCHEME_LOCAL;
		*piType = INFO_DATATYPE_UINT32;
		break;
	case URL_INFO_FILENAME: /* STRING     File name if the scheme of the URL is file. GenTL v1.5 */
	default:
		return GC_ERR_NOT_IMPLEMENTED;
	}
	return GC_ERR_SUCCESS;
}

GC_API GCReadPortStacked(PORT_HANDLE hPort, PORT_REGISTER_STACK_ENTRY* pEntries, size_t* piNumEntries)
{
	return GC_ERR_SUCCESS;
}

GC_API GCWritePortStacked(PORT_HANDLE hPort, PORT_REGISTER_STACK_ENTRY* pEntries, size_t* piNumEntries)
{
	return GC_ERR_SUCCESS;
}

/* GenTL v1.3 */
GC_API DSGetBufferChunkData(DS_HANDLE hDataStream, BUFFER_HANDLE hBuffer, SINGLE_CHUNK_DATA* pChunkData, size_t* piNumChunks)
{
	return GC_ERR_SUCCESS;
}

/* GenTL v1.4 */
GC_API IFGetParentTL(IF_HANDLE hIface, TL_HANDLE* phSystem)
{
	return GC_ERR_SUCCESS;
}

GC_API DevGetParentIF(DEV_HANDLE hDevice, IF_HANDLE* phIface)
{
	return GC_ERR_SUCCESS;
}

GC_API DSGetParentDev(DS_HANDLE hDataStream, DEV_HANDLE* phDevice)
{
	return GC_ERR_SUCCESS;
}

/* GenTL v1.5 */
GC_API DSGetNumBufferParts(DS_HANDLE hDataStream, BUFFER_HANDLE hBuffer, uint32_t* piNumParts)
{
	*piNumParts = 1;
	return GC_ERR_SUCCESS;
}

GC_API DSGetBufferPartInfo(DS_HANDLE hDataStream, BUFFER_HANDLE hBuffer, uint32_t iPartIndex, BUFFER_PART_INFO_CMD iInfoCmd, INFO_DATATYPE* piType, void* pBuffer, size_t* piSize)
{
	*piType = INFO_DATATYPE_STRING;
	switch (iInfoCmd)
	{
	case BUFFER_PART_INFO_BASE: /* PTR        Base address of the buffer part memory. */
		*(void**)pBuffer = consumer_buffer;
		*piType = INFO_DATATYPE_PTR;
		break;
	case BUFFER_PART_INFO_DATA_SIZE: /* SIZET      Size of the buffer part in bytes. */
		*(size_t*)pBuffer = BUFFER_SIZE;
		*piType = INFO_DATATYPE_SIZET;
		break;
	case BUFFER_PART_INFO_DATA_TYPE: /* SIZET      Type of the data in given part (PARTDATATYPE_ID enumeration value). */
		*(size_t*)pBuffer = PART_DATATYPE_2D_IMAGE;
		*piType = INFO_DATATYPE_SIZET;
		break;
	case BUFFER_PART_INFO_DATA_FORMAT: /* UINT64     Format of individual items (such as pixels) in the buffer part. */
		*(uint64_t*)pBuffer = ARV_PIXEL_FORMAT_MONO_8;
		*piType = INFO_DATATYPE_UINT64;
		break;
	case BUFFER_PART_INFO_DATA_FORMAT_NAMESPACE: /* UINT64     Allows interpretation of BUFFER_PART_INFO_DATA_FORMAT (PIXELFORMAT_NAMESPACE_ID enumeration value). */
		*(uint64_t*)pBuffer = PIXELFORMAT_NAMESPACE_GEV;
		*piType = INFO_DATATYPE_UINT64;
		break;
	case BUFFER_PART_INFO_WIDTH: /* SIZET      Width of data in the buffer part in pixels. */
		*(size_t*)pBuffer = 512;
		*piType = INFO_DATATYPE_SIZET;
		break;
	case BUFFER_PART_INFO_HEIGHT: /* SIZET      Expected height of data in the buffer part in pixels . */
		*(size_t*)pBuffer = 512;
		*piType = INFO_DATATYPE_SIZET;
		break;
	case BUFFER_PART_INFO_XOFFSET: /* SIZET      Horizontal offset of data in the buffer part in pixels. */
		*(size_t*)pBuffer = 0;
		*piType = INFO_DATATYPE_SIZET;
		break;
	case BUFFER_PART_INFO_YOFFSET: /* SIZET      Vertical offset of data in the buffer part in pixels. */
		*(size_t*)pBuffer = 0;
		*piType = INFO_DATATYPE_SIZET;
		break;
	case BUFFER_PART_INFO_XPADDING: /* SIZET      Horizontal padding of data in the buffer part in pixels. */
		*(size_t*)pBuffer = 0;
		*piType = INFO_DATATYPE_SIZET;
		break;
	case BUFFER_PART_INFO_SOURCE_ID: /* UINT64     Identifier allowing to group data parts belonging to the same source. */
		*(uint64_t*)pBuffer = 0;
		*piType = INFO_DATATYPE_UINT64;
		break;
	case BUFFER_PART_INFO_DELIVERED_IMAGEHEIGHT: /* SIZET      Height of the data currently in the buffer part in pixels*/
		*(size_t*)pBuffer = 512;
		*piType = INFO_DATATYPE_SIZET;
		break;
	default:
		return GC_ERR_NOT_IMPLEMENTED;
	}
	return GC_ERR_SUCCESS;
}
