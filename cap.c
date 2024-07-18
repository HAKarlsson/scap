#include "cap.h"

#include <stdint.h>

#define ARRAY_SIZE(_x) (sizeof(_x) / sizeof(_x[0]))

// Capability table
cap_t ctable[64] = {
    // Initial capability
    [0] = {
        .pid = 0,
        .fuel = 64,
        .max_fuel = 64,
        .rwx = 0x7,
        .base = 0x80000000,
        .size = 0x10000,
    }
};

int cap_derive(int pid, int i, uint8_t fuel, uint32_t base, uint32_t size,
    uint8_t rwx)
{
    // Check capability index
    if (i < 0 || i >= ARRAY_SIZE(ctable))
        return -1;

    // Check owner
    if (ctable[i].pid != pid)
        return -1;

    // Check if sufficient fuel
    if (ctable[i].fuel < fuel || fuel == 0)
        return -1;

    // Check if out of bounds
    if ((base < ctable[i].base)
        || (base + size > ctable[i].base + ctable[i].size))
        return -1;

    // Check RWX permissions.
    if ((ctable[i].rwx & rwx) != rwx)
        return -1;

    // Index of new capability.
    int j = i + ctable[i].fuel - fuel;

    // Reduce fuel of original capability
    ctable[i].fuel -= fuel;

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
    // Check capability index.
    if (i < 0 || i >= ARRAY_SIZE(ctable))
        return -1;

    // Check owner.
    if (ctable[i].pid != pid)
        return -1;

    // Invalidate child capabilities.
    for (int j = ctable[i].fuel; j < ctable[i].max_fuel; ++j)
        ctable[i + j].pid = -1;

    // Restore fuel.
    ctable[i].fuel = ctable[i].max_fuel;

    return i;
}

int cap_delete(int pid, int i)
{
    // Check capability index.
    if (i >= ARRAY_SIZE(ctable))
        return -1;

    // Check owner.
    if (ctable[i].pid != pid)
        return -1;

    // Invalidate capability.
    ctable[i].pid = -1;

    return i;
}
