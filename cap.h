#pragma once
#include "cap.h"
#include <stdint.h>

/**
 * Basic memory capability
 */
typedef struct {
	int8_t pid;
	uint8_t fuel;
	uint8_t max_fuel;
	uint8_t rwx;
	uint32_t base;
	uint32_t size;
} cap_t;

int cap_derive(int pid, int i, uint8_t fuel, uint32_t base, uint32_t size, uint8_t rwx);

int cap_revoke(int pid, int i);

int cap_delete(int pid, int i);
