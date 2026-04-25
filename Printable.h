#ifndef PRINTABLE_H
#define PRINTABLE_H

// Abstract Class 3: Any object that can be printed/displayed generically.
// Both Vehicle and Listing implement this so they can be printed
// through a common Printable* pointer without knowing the concrete type.
class Printable {
public:
    virtual void print() const = 0;  // Must output object to console
    virtual ~Printable() {}
};

#endif
