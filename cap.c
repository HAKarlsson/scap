#include "cap.h"
#include <stdint.h>

#define ARRAY_SIZE(_x) (sizeof(_x)/sizeof(_x[0]))

// Capability table
cap_t ctable[64] = {
	// Initial capability
	{ .pid = 0,
	  .fuel = 64,
	  .max_fuel = 64,
	  .rwx = 0x7,
	  .base = 0x80000000,
	  .size = 0x10000
	}
};

int cap_derive(int pid, int i, uint8_t fuel, uint32_t base, uint32_t size, uint8_t rwx)
{
	cap_t *cap = &ctable[i];

	// Check capability index
	if (i < 0 || i >= ARRAY_SIZE(ctable))
		return -1;
	// Check owner
	if (cap->pid != pid)
		return -1;
	// Check if sufficient fuel
	if (cap->fuel < fuel)
		return -1;
	// Check if out of bounds
	if ((base < cap->base) || (base + size > cap->base + cap->size))
		return -1;
	// Check RWX permissions. 
	if ((cap->rwx & rwx) != rwx)
		return -1;

	// Index of new capability.
	int j = i + cap->fuel - fuel;

	// Create new capability.
	ctable[j].pid = ctable[i].pid;
	ctable[j].fuel = fuel;
	ctable[j].max_fuel = fuel;
	ctable[j].base = base;
	ctable[j].size = size;

	// Return index of new capability.
	return j;
}

int cap_revoke(int pid, int i)
{
	cap_t *cap = &ctable[i];

	// Check capability index.
	if (i < 0 || i >= ARRAY_SIZE(ctable))
		return -1;

	// Check owner.
	if (cap->pid != pid)
		return -1;

	// Invalidate child capabilities.
	for (int j = cap->fuel; j < cap->max_fuel; ++j)
		ctable[i + j].pid = -1;

	// Restore fuel.
	cap->fuel = cap->max_fuel;

	return i;
}

int cap_delete(int pid, int i)
{
	cap_t *cap = &ctable[i];

	// Check capability index.
	if (i >= ARRAY_SIZE(ctable))
		return -1;
	// Check owner.
	if (cap->pid != pid)
		return -1;

	// Invalidate capability.
	cap->pid = -1;

	return i;
}
