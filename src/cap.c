#include "cap.h"
#include <stdint.h>

typedef struct {
	int8_t pid;
	uint8_t fuel;
	uint8_t max_fuel;
	uint8_t rwx;
	uint32_t base;
	uint32_t size;
} Cap;

extern Cap ctable[];

int CapDerive(int pid, int i, uint8_t fuel, uint32_t base, uint32_t size, uint8_t rwx)
{
	Cap *cap = &ctable[i];

	if (i >= ARRAY_SIZE(ctable))
		return -1;
	if (cap->pid != pid)
		return -1;
	if (cap->fuel < fuel)
		return -1;
	if (base < cap->base)
		return -1;
	if (base + size > cap->base + cap->size)
		return -1;

	// Index of new capability
	int j = i + cap->fuel - fuel;

	ctable[j].pid = ctable[i].pid;
	ctable[j].fuel = fuel;
	ctable[j].max_fuel = fuel;
	ctable[j].base = base;
	ctable[j].size = size;

	return j;
}

void CapRevoke(int pid, int i)
{
	if (i >= ARRAY_SIZE(ctable))
		return -1;
	if (cap->pid != pid)
		return -1;
	Cap *cap = &ctable[i];
	Cap *bgn = &ctable[i + cap->fuel];
	Cap *end = &ctable[i + cap->max_fuel];

	while (bgn != end)
		(bgn++).pid = -1;

	cap->fuel = cap->max_fuel;
}

void CapDelete(int pid, int i)
{
	if (i >= ARRAY_SIZE(ctable))
		return -1;
	if (cap->pid != pid)
		return -1;
	Cap *cap = &ctable[i];
	cap->pid = -1;
}
