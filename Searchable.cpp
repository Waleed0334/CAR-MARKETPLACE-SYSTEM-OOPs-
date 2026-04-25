#include "Searchable.h"

// Searchable has no concrete method implementations.
// matchesBrand() and matchesPrice() are pure virtual — implemented by Car, Bike, etc.
// Allows the Search utility to work on any Searchable* without casting.
//
// This file exists to satisfy the .h / .cpp separation requirement.
