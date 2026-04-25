#include "AbstractUser.h"

// AbstractUser has no concrete method implementations.
// All methods are pure virtual — their bodies live in derived classes (Buyer, Seller, Admin).
// The virtual destructor ensures correct cleanup when deleting through an AbstractUser pointer.
//
// This file exists to satisfy the .h / .cpp separation requirement.
