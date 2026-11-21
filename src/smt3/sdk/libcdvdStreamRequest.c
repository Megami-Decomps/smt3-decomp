#include "sdk_cdvd_stream_request.h"

SIZE_CHECK(cdvdStreamRequestNativeSize, cdvdStreamRequest_t, 0x14);

/* Arguments passed as one live record in the stream RPC. */
cdvdStreamRequest_t cdvdStreamRequest __attribute__((section(".data"))) = { 0 };
