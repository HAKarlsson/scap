# Static capability system

Work in progress mockup of static capability system where capabilities are stored in predetermined locations.

In this system, each *root* capability is placed in index 0 of an array of capabilities.
When a new capability is derived from the root, it is placed in this same array.
Each capability has a *fuel* that indicates how many capabilities that can be derived from it.
The fuel of a new capability also determines where it is placed in the array, i.e., capability indices are automatic and deterministic.
This system ensures that we have a finite number of derived capabilities, ensuring an upper-bound on capability revocation.

We have the following features:

- Each capability has a fixed number of children.
- Capability revocation is a simple for-loop (speed).
- Indicies of derived capabilities are automatic and deterministic.
- Capability access and operations do not leak side-channel information.
