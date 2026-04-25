#ifndef ABSTRACT_USER_H
#define ABSTRACT_USER_H

#include <string>
using namespace std;

// Abstract Class 2: Defines the mandatory interface for ALL user types.
// Buyer, Seller, and Admin must each implement these pure virtual functions.
class AbstractUser {
public:
    virtual void   displayUser()  const = 0;  // Must show user information
    virtual void   performRole()  const = 0;  // Must describe the user's role action
    virtual string getRole()      const = 0;  // Must return role name as string
    virtual string getName()      const = 0;  // Must return user's name
    virtual ~AbstractUser() {}
};

#endif
