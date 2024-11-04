#pragma once

#include <stdint.h>

typedef struct {
	uint32_t version_length;
	char version[8];
	uint32_t file_total;
} MPKGHeader;

typedef struct {
	uint32_t file_name_length;
	char file_name[256];
	uint32_t index;
	uint32_t file_size;
} MPKGFileEntry;
