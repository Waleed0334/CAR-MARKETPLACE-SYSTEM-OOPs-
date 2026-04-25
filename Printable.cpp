#include "Printable.h"

// Printable has no concrete method implementations.
// print() is pure virtual — implemented by Vehicle, Listing, etc.
// Allows generic printing loops:  Printable* p[] = {&car, &listing};
//
// This file exists to satisfy the .h / .cpp separation requirement.
