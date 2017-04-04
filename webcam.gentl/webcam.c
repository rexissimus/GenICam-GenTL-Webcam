#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <math.h>
#include "webcam.gentl.h"
#include "webcam.h"

void* registry;
char* genicam_xml;
size_t genicam_xml_size;

ArvBuffer buffer;
char* consumer_buffer = NULL;

uint32_t frame_id;
double trigger_frequency;

int32_t device_access_status = DEVICE_ACCESS_STATUS_READWRITE;

webcam_fill_pattern fill_pattern_callback;
void *fill_pattern_data;

EVENT_NEW_BUFFER_DATA nbd;

// Copy string by validating buffer size and set size of final buffer
GC_ERROR CopyString(void* pBuffer, char* string, size_t* piSize)
{
	size_t strLen = strlen(string) + 1;
	*piSize = strLen;
	if (pBuffer == NULL)
	{
		return GC_ERR_SUCCESS;
	}
	if (piSize && strLen > *piSize)
		return GC_ERR_BUFFER_TOO_SMALL;
	strcpy(pBuffer, string);
	return GC_ERR_SUCCESS;
}

// Return contents of xml in allocated memory
char* get_webcam_genicam_xml(size_t *size)
{
	char path[MAX_PATH];
	char fullpath[MAX_PATH];
	HINSTANCE hm = NULL;
	GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
		GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		(LPCSTR)*GCGetInfo, &hm);
	GetModuleFileNameA(hm, path, sizeof(path));
	// Remove basename
	path[strlen(path) - 21] = '\0';
	sprintf(fullpath, "%s\\%s", path, "webcam.xml");

	FILE *f = fopen(fullpath, "r");
	fseek(f, 0, SEEK_END);
	*size = ftell(f);
	fseek(f, 0, SEEK_SET);  //same as rewind(f);

	char *string = malloc(*size + 1);
	size_t read_bytes;
	read_bytes = fread(string, 1, *size, f);
	fclose(f);
	string[read_bytes] = 0;
	return string;
}

// Set up register
void webcam_init()
{
	nbd.BufferHandle = 1;
	buffer.data = malloc(BUFFER_SIZE);
	buffer.size = BUFFER_SIZE;
	char genicam_xml_path[MAX_PATH];

	registry = malloc(ARV_FAKE_CAMERA_MEMORY_SIZE);

	fill_pattern_callback = webcam_diagonal_ramp;
	fill_pattern_data = NULL;

	genicam_xml = get_webcam_genicam_xml(&genicam_xml_size);

	strcpy(((char *)registry) + ARV_GVBS_MANUFACTURER_NAME_OFFSET, "MyCompany");
	strcpy(((char *)registry) + ARV_GVBS_MODEL_NAME_OFFSET, "Webcam");
	strcpy(((char *)registry) + ARV_GVBS_DEVICE_VERSION_OFFSET, "1.0");
	strcpy(((char *)registry) + ARV_GVBS_SERIAL_NUMBER_OFFSET, "webcam123");

	sprintf(genicam_xml_path, "Local:webcam.xml;%x;%x",
		ARV_FAKE_CAMERA_MEMORY_SIZE,
		(unsigned int)genicam_xml_size);
	strcpy(((char *)registry) + ARV_GVBS_XML_URL_0_OFFSET, genicam_xml_path);

	webcam_write_register(ARV_FAKE_CAMERA_REGISTER_SENSOR_WIDTH,
		ARV_FAKE_CAMERA_SENSOR_WIDTH);
	webcam_write_register(ARV_FAKE_CAMERA_REGISTER_SENSOR_HEIGHT,
		ARV_FAKE_CAMERA_SENSOR_HEIGHT);
	webcam_write_register(ARV_FAKE_CAMERA_REGISTER_WIDTH,
		ARV_FAKE_CAMERA_WIDTH_DEFAULT);
	webcam_write_register(ARV_FAKE_CAMERA_REGISTER_HEIGHT,
		ARV_FAKE_CAMERA_HEIGHT_DEFAULT);
	webcam_write_register(ARV_FAKE_CAMERA_REGISTER_X_OFFSET, 0);
	webcam_write_register(ARV_FAKE_CAMERA_REGISTER_Y_OFFSET, 0);
	webcam_write_register(ARV_FAKE_CAMERA_REGISTER_BINNING_HORIZONTAL,
		ARV_FAKE_CAMERA_BINNING_HORIZONTAL_DEFAULT);
	webcam_write_register(ARV_FAKE_CAMERA_REGISTER_BINNING_VERTICAL,
		ARV_FAKE_CAMERA_BINNING_HORIZONTAL_DEFAULT);
	webcam_write_register(ARV_FAKE_CAMERA_REGISTER_PIXEL_FORMAT, ARV_FAKE_CAMERA_PIXEL_FORMAT_DEFAULT);

	webcam_write_register(ARV_FAKE_CAMERA_REGISTER_ACQUISITION, 0);
	webcam_write_register(ARV_FAKE_CAMERA_REGISTER_ACQUISITION_MODE, 1);
	webcam_write_register(ARV_FAKE_CAMERA_REGISTER_ACQUISITION_FRAME_PERIOD_US,
		(uint32_t)(1000000.0 / ARV_FAKE_CAMERA_ACQUISITION_FRAME_RATE_DEFAULT));
	webcam_write_register(ARV_FAKE_CAMERA_REGISTER_EXPOSURE_TIME_US,
		(uint32_t)ARV_FAKE_CAMERA_EXPOSURE_TIME_US_DEFAULT);

	webcam_write_register(ARV_FAKE_CAMERA_REGISTER_GAIN_RAW, 0);
	webcam_write_register(ARV_FAKE_CAMERA_REGISTER_GAIN_MODE, 1);
}

uint8_t webcam_read_memory(uint32_t address, uint32_t size, void *buffer)
{
	uint32_t read_size;
	if (buffer == NULL)
	{
		return FALSE;
	}
	if (size <= 0) return FALSE;
	if (address < ARV_FAKE_CAMERA_MEMORY_SIZE) {
		read_size = min(address + size, ARV_FAKE_CAMERA_MEMORY_SIZE) - address;

		memcpy(buffer, ((char *)registry) + address, read_size);

		if (read_size == size)
			return TRUE;

		size = size - read_size;
		address = ARV_FAKE_CAMERA_MEMORY_SIZE;
		buffer = (char*)buffer + read_size;
	}

	address -= ARV_FAKE_CAMERA_MEMORY_SIZE;
	read_size = min(address + size, (uint32_t)genicam_xml_size) - address;

	memcpy(buffer, ((char *)genicam_xml) + address, read_size);
	if (read_size < size)
		memset((char*)buffer + read_size, 0, size - read_size);

	return TRUE;
}

uint8_t webcam_write_memory(uint32_t address, uint32_t size, const void *buffer)
{
	if (address + size >= ARV_FAKE_CAMERA_MEMORY_SIZE + genicam_xml_size)
		return FALSE;
	if (buffer == NULL) return FALSE;
	if (size <= 0) return FALSE;
	/* genicam_data are read only */
	if (address + size > ARV_FAKE_CAMERA_MEMORY_SIZE)
		return FALSE;

	memcpy(((char *)registry) + address, buffer, size);

	return TRUE;
}

uint8_t webcam_read_register(uint32_t address, uint32_t *value)
{
	uint8_t success;

	if (value == NULL)
		return FALSE;

	success = webcam_read_memory(address, sizeof(*value), value);
	return success;
}

uint8_t webcam_write_register(uint32_t address, uint32_t value)
{
	return webcam_write_memory(address, sizeof(value), &value);
}

uint32_t _get_register(uint32_t address)
{
	uint32_t value;

	if (address + sizeof(uint32_t) > ARV_FAKE_CAMERA_MEMORY_SIZE)
		return 0;

	value = *((uint32_t *)(((char *)(registry) + address)));

	return value;
}

static void webcam_diagonal_ramp(ArvBuffer *buffer, void *fill_pattern_data,
	uint32_t exposure_time_us,
	uint32_t gain,
	ArvPixelFormat pixel_format)
{
	double pixel_value;
	double scale;
	uint32_t x, y;
	uint32_t width;
	uint32_t height;

	if (buffer == NULL)
		return;

	if (pixel_format != ARV_PIXEL_FORMAT_MONO_8)
		return;

	width = buffer->width;
	height = buffer->height;

	scale = 1.0 + gain + log10((double)exposure_time_us / 10000.0);

	for (y = 0; y < height; y++)
		for (x = 0; x < width; x++) {
			pixel_value = (x + buffer->frame_id + y) % 255;
			pixel_value *= scale;

			if (pixel_value < 0.0)
				((unsigned char *)buffer->data)[y * width + x] = 0;
			else if (pixel_value > 255.0)
				((unsigned char *)buffer->data)[y * width + x] = 255;
			else
				((unsigned char *)buffer->data)[y * width + x] = (unsigned char)pixel_value;
		}
}

void webcam_set_fill_pattern(
	webcam_fill_pattern new_fill_pattern_callback,
	void *new_fill_pattern_data)
{

	if (new_fill_pattern_callback != NULL) {
		fill_pattern_callback = new_fill_pattern_callback;
		fill_pattern_data = new_fill_pattern_data;
	}
	else {
		fill_pattern_callback = webcam_diagonal_ramp;
		fill_pattern_data = NULL;
	}
}

int clock_gettime(int i, struct timespec *spec)      //C-file part
{
	__int64 wintime;
	GetSystemTimeAsFileTime((FILETIME*)&wintime);
	wintime -= 116444736000000000i64;  //1jan1601 to 1jan1970
	spec->tv_sec = wintime / 10000000i64;           //seconds
	spec->tv_nsec = wintime % 10000000i64 * 100;      //nano-seconds
	return 0;
}

void webcam_fill_buffer()
{
	struct timespec time;
	uint32_t width;
	uint32_t height;
	uint32_t exposure_time_us;
	uint32_t gain;
	uint32_t pixel_format;
	size_t payload;

	clock_gettime(0, &time);

	width = _get_register(ARV_FAKE_CAMERA_REGISTER_WIDTH);
	height = _get_register(ARV_FAKE_CAMERA_REGISTER_HEIGHT);
	payload = width * height;

	if (buffer.size < payload) {
		buffer.status = ARV_BUFFER_STATUS_SIZE_MISMATCH;
		return;
	}

	buffer.width = width;
	buffer.height = height;
	buffer.status = ARV_BUFFER_STATUS_SUCCESS;
	buffer.timestamp_ns = ((uint64_t)time.tv_sec) * 1000000000LL + time.tv_nsec;
	buffer.frame_id = frame_id++;
	buffer.pixel_format = _get_register(ARV_FAKE_CAMERA_REGISTER_PIXEL_FORMAT);

	webcam_read_register(ARV_FAKE_CAMERA_REGISTER_EXPOSURE_TIME_US, &exposure_time_us);
	webcam_read_register(ARV_FAKE_CAMERA_REGISTER_GAIN_RAW, &gain);
	webcam_read_register(ARV_FAKE_CAMERA_REGISTER_PIXEL_FORMAT, &pixel_format);
	fill_pattern_callback(&buffer, fill_pattern_data, exposure_time_us, gain, pixel_format);
	if (consumer_buffer)
		memcpy(consumer_buffer, buffer.data, buffer.size);
}

size_t webcam_get_payload()
{
	uint32_t width, height;
	width = _get_register(ARV_FAKE_CAMERA_REGISTER_WIDTH);
	height = _get_register(ARV_FAKE_CAMERA_REGISTER_HEIGHT);

	return width * height;
}

void webcam_finalize()
{
	free(registry);
	free(genicam_xml);
}
