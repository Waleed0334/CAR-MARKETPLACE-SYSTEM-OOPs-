#ifndef ABSTRACT_VEHICLE_H
#define ABSTRACT_VEHICLE_H

#include <string>
using namespace std;

// Abstract Class 1: Defines the mandatory interface for ALL vehicle types.
// Any class that represents a vehicle MUST implement these pure virtual functions.
class AbstractVehicle {
public:
    virtual void   displayDetails() const = 0;  // Must show vehicle info
    virtual double calculateValue() const = 0;  // Must compute current market value
    virtual string getType()        const = 0;  // Must return vehicle type name
    virtual string getBrand()       const = 0;  // Must return brand
    virtual double getPrice()       const = 0;  // Must return price
    virtual ~AbstractVehicle() {}
};

#endif
