#ifndef SEARCHABLE_H
#define SEARCHABLE_H

#include <string>
using namespace std;

// Abstract Class 4: Any vehicle type that can be filtered in search results.
// Car and Bike implement this so the Search class can filter
// through a Searchable* pointer without knowing whether it is a Car or Bike.
class Searchable {
public:
    virtual bool matchesBrand(const string& brand)          const = 0;
    virtual bool matchesPrice(double minPrice, double maxPrice) const = 0;
    virtual ~Searchable() {}
};

#endif
