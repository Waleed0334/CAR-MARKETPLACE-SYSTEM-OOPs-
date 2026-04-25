#include "AbstractVehicle.h"

// AbstractVehicle has no concrete method implementations.
// All methods are pure virtual — their bodies live in derived classes (Car, Bike, etc.).
// The destructor is virtual to ensure proper cleanup through base pointers.
//
// This file exists to satisfy the .h / .cpp separation requirement.
// Linkers expect a translation unit for each header.
