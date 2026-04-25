/*
 * ============================================================
 *  AB~WHEELS CAR MARKETPLACE SYSTEM - ASSIGNMENT 2
 *  Demonstrates: Inheritance, Polymorphism, Abstraction,
 *                Operator Overloading, Friend Functions
 *
 *  Abstract class interfaces are defined in separate .h files:
 *    - AbstractVehicle.h / AbstractVehicle.cpp
 *    - AbstractUser.h    / AbstractUser.cpp
 *    - Printable.h       / Printable.cpp
 *    - Searchable.h      / Searchable.cpp
 * ============================================================ */

#include <iostream>
#include <string>
using namespace std;

// Include the four abstract class headers (interfaces)
#include "AbstractVehicle.h"
#include "AbstractUser.h"
#include "Printable.h"
#include "Searchable.h"


/* ============================================================
 *  ENGINE (Composition class used inside Vehicle)
 * ============================================================ */

class Engine {
private:
    const string engineID;
    int    horsepower;
    double capacity;
    string fuelType;
    bool   turbocharged;
    bool   isRunning;
    static int engineCount;

public:
    Engine()
        : engineID("ENG-000"), horsepower(100), capacity(1.5),
          fuelType("Petrol"), turbocharged(false), isRunning(false) {
        engineCount++;
    }

    Engine(string id, int hp, double cap, string fuel, bool turbo)
        : engineID(id), horsepower(hp), capacity(cap),
          fuelType(fuel), turbocharged(turbo), isRunning(false) {
        engineCount++;
    }

    Engine(const Engine& e)
        : engineID(e.engineID), horsepower(e.horsepower), capacity(e.capacity),
          fuelType(e.fuelType), turbocharged(e.turbocharged), isRunning(e.isRunning) {
        engineCount++;
    }

    void displayInfo() const {
        cout << "  Engine[" << engineID << "] HP:" << horsepower
             << " Cap:" << capacity << "L Fuel:" << fuelType
             << " Turbo:" << (turbocharged ? "Yes" : "No") << "\n";
    }

    void start()               { isRunning = true;  cout << "Engine started.\n"; }
    void stop()                { isRunning = false; cout << "Engine stopped.\n"; }
    void upgradeHorsepower(int hp) { horsepower += hp; }
    void toggleTurbo()         { turbocharged = !turbocharged; }

    int    getHorsepower() const { return horsepower; }
    double getCapacity()   const { return capacity; }
    string getFuelType()   const { return fuelType; }
    bool   isTurbo()       const { return turbocharged; }

    static int getEngineCount() { return engineCount; }

    // ---- Friend operator (non-member, needs private horsepower of both) ----
    // Justification: operator> must compare private 'horsepower' from two Engine objects.
    //                A non-member friend is the correct C++ pattern for symmetric binary operators.
    friend bool operator>(const Engine& e1, const Engine& e2);
};

int Engine::engineCount = 0;

bool operator>(const Engine& e1, const Engine& e2) {
    return e1.horsepower > e2.horsepower;
}


/* ============================================================
 *  VEHICLE HIERARCHY
 *
 *  Inheritance relationships:
 *    1. Vehicle     → AbstractVehicle  (abstract interface)
 *    2. Vehicle     → Printable        (abstract interface)
 *    3. Car         → Vehicle
 *    4. Car         → Searchable       (abstract interface)
 *    5. ElectricCar → Car
 *    6. Bike        → Vehicle
 *    7. Bike        → Searchable       (abstract interface)
 *    8. SportsBike  → Bike
 * ============================================================ */

// ---- Vehicle ----
// Inheritance 1 & 2: implements AbstractVehicle and Printable interfaces
class Vehicle : public AbstractVehicle, public Printable {
protected:
    const string VIN;
    string brand;
    string model;
    int    year;
    double price;
    int    mileage;
    Engine engine;
    static int vehicleCount;

public:
    Vehicle()
        : VIN("VIN-000"), brand("N/A"), model("N/A"), year(2020),
          price(10000), mileage(0), engine() {
        vehicleCount++;
    }

    Vehicle(string vin, string b, string m, int y, double p, int mil, Engine e)
        : VIN(vin), brand(b), model(m), year(y), price(p), mileage(mil), engine(e) {
        vehicleCount++;
    }

    Vehicle(const Vehicle& v)
        : VIN(v.VIN), brand(v.brand), model(v.model), year(v.year),
          price(v.price), mileage(v.mileage), engine(v.engine) {
        vehicleCount++;
    }

    // AbstractVehicle pure virtual implementations
    virtual void displayDetails() const override {
        cout << "[Vehicle] " << brand << " " << model
             << " (" << year << ") $" << price << " | " << mileage << "km\n";
        engine.displayInfo();
    }
    virtual double calculateValue() const override { return price; }
    virtual string getType()        const override { return "Vehicle"; }
    virtual string getBrand()       const override { return brand; }
    virtual double getPrice()       const override { return price; }

    // Printable pure virtual implementation
    void print() const override { displayDetails(); }

    void updatePrice(double p)  { price   = p; }
    void updateMileage(int m)   { mileage = m; }
    void startVehicle()         { engine.start(); }
    void upgradeEngine(int hp)  { engine.upgradeHorsepower(hp); }

    string getModel()  const { return model; }
    int    getYear()   const { return year; }
    int    getMileage()const { return mileage; }

    static int getVehicleCount() { return vehicleCount; }

    // ---- Member Operator Overloads ----
    // Justification: comparing/combining vehicles by price is natural marketplace behavior
    bool   operator==(const Vehicle& v) const { return price == v.price; }
    bool   operator!=(const Vehicle& v) const { return !(*this == v); }
    bool   operator<(const Vehicle& v)  const { return price <  v.price; }
    bool   operator>(const Vehicle& v)  const { return price >  v.price; }
    double operator+(const Vehicle& v)  const { return price +  v.price; }

    // ---- Friend Operator Overloads ----
    // Justification: operator<< / >> need access to protected brand/model/price/year.
    //                Stream operators are always better as non-members so the stream
    //                is on the left-hand side (cout << v, not v << cout).
    friend ostream& operator<<(ostream& out, const Vehicle& v);
    friend istream& operator>>(istream& in,  Vehicle& v);

    // ---- Friend Function ----
    // Justification: isExpensive() is a standalone utility that needs private 'price'
    //                but does not logically belong as a member of Vehicle.
    friend bool isExpensive(const Vehicle& v);
};

int Vehicle::vehicleCount = 0;

ostream& operator<<(ostream& out, const Vehicle& v) {
    out << v.brand << " " << v.model << " (" << v.year << ") $" << v.price;
    return out;
}

istream& operator>>(istream& in, Vehicle& v) {
    cout << "Enter brand model year price: ";
    in >> v.brand >> v.model >> v.year >> v.price;
    return in;
}

bool isExpensive(const Vehicle& v) {
    return v.price > 20000;
}


// ---- Car ----
// Inheritance 3 & 4: Car extends Vehicle, also implements Searchable interface
class Car : public Vehicle, public Searchable {
protected:
    string color;
    string transmission;
    int    doors;
    bool   sunroof;
    bool   isElectric;
    static int carCount;

public:
    Car()
        : Vehicle(), color("White"), transmission("Manual"),
          doors(4), sunroof(false), isElectric(false) {
        carCount++;
    }

    Car(string vin, string b, string m, int y, double p, int mil, Engine e,
        string col, string trans, int d, bool sr, bool electric)
        : Vehicle(vin, b, m, y, p, mil, e), color(col), transmission(trans),
          doors(d), sunroof(sr), isElectric(electric) {
        carCount++;
    }

    Car(const Car& c)
        : Vehicle(c), color(c.color), transmission(c.transmission),
          doors(c.doors), sunroof(c.sunroof), isElectric(c.isElectric) {
        carCount++;
    }

    // Polymorphism: overrides Vehicle::displayDetails()
    void displayDetails() const override {
        cout << "[Car] " << brand << " " << model
             << " (" << year << ") $" << price << " | " << mileage << "km"
             << " | " << color << " | " << doors << " doors"
             << " | Sunroof:" << (sunroof ? "Y" : "N")
             << " | EV:" << (isElectric ? "Y" : "N") << "\n";
        engine.displayInfo();
    }

    double calculateValue() const override {
        return isElectric ? price * 1.05 : price;
    }

    string getType() const override { return "Car"; }

    // Searchable pure virtual implementations
    bool matchesBrand(const string& b)              const override { return brand == b; }
    bool matchesPrice(double min, double max)        const override {
        double v = calculateValue();
        return v >= min && v <= max;
    }

    void paintCar(string c)           { color = c; }
    void toggleSunroof()              { sunroof = !sunroof; }
    void upgradeToElectric()          { isElectric = true; }
    void changeTransmission(string t) { transmission = t; }

    string getColor()        const { return color; }
    string getTransmission() const { return transmission; }
    int    getDoors()        const { return doors; }
    bool   hasSunroof()      const { return sunroof; }
    bool   getIsElectric()   const { return isElectric; }

    static int getCarCount() { return carCount; }
};

int Car::carCount = 0;


// ---- ElectricCar ----
// Inheritance 5: ElectricCar extends Car (multi-level)
class ElectricCar : public Car {
private:
    int    batteryRange;
    double chargeLevel;

public:
    ElectricCar(string vin, string b, string m, int y, double p, int mil, Engine e,
                string col, string trans, int d, bool sr, int range, double charge)
        : Car(vin, b, m, y, p, mil, e, col, trans, d, sr, true),
          batteryRange(range), chargeLevel(charge) {}

    // Polymorphism: overrides Car::displayDetails()
    void displayDetails() const override {
        cout << "[ElectricCar] ";
        Car::displayDetails();
        cout << "  Battery: " << batteryRange << "km range | Charge: " << chargeLevel << "%\n";
    }

    string getType() const override { return "ElectricCar"; }

    double calculateValue() const override {
        return price * 1.1 + batteryRange * 5;
    }

    void charge(double amount) {
        chargeLevel = min(100.0, chargeLevel + amount);
        cout << "Charged to " << chargeLevel << "%\n";
    }

    int    getBatteryRange() const { return batteryRange; }
    double getChargeLevel()  const { return chargeLevel; }
};


// ---- Bike ----
// Inheritance 6 & 7: Bike extends Vehicle, also implements Searchable interface
class Bike : public Vehicle, public Searchable {
protected:
    bool sports;
    int  wheelSize;
    static int bikeCount;

public:
    Bike()
        : Vehicle(), sports(false), wheelSize(17) {
        bikeCount++;
    }

    Bike(string vin, string b, string m, int y, double p, int mil, Engine e,
         bool sp, int ws)
        : Vehicle(vin, b, m, y, p, mil, e), sports(sp), wheelSize(ws) {
        bikeCount++;
    }

    // Polymorphism: overrides Vehicle::displayDetails()
    void displayDetails() const override {
        cout << "[Bike] " << brand << " " << model
             << " (" << year << ") $" << price << " | " << mileage << "km"
             << " | Sports:" << (sports ? "Y" : "N")
             << " | Wheel:" << wheelSize << "in\n";
        engine.displayInfo();
    }

    double calculateValue() const override {
        return sports ? price * 0.95 : price * 0.9;
    }

    string getType() const override { return "Bike"; }

    // Searchable pure virtual implementations
    bool matchesBrand(const string& b)              const override { return brand == b; }
    bool matchesPrice(double min, double max)        const override {
        double v = calculateValue();
        return v >= min && v <= max;
    }

    bool isSports()     const { return sports; }
    int  getWheelSize() const { return wheelSize; }

    static int getBikeCount() { return bikeCount; }
};

int Bike::bikeCount = 0;


// ---- SportsBike ----
// Inheritance 8: SportsBike extends Bike (multi-level)
class SportsBike : public Bike {
private:
    int topSpeed;

public:
    SportsBike(string vin, string b, string m, int y, double p, int mil, Engine e,
               int ws, int speed)
        : Bike(vin, b, m, y, p, mil, e, true, ws), topSpeed(speed) {}

    // Polymorphism: overrides Bike::displayDetails()
    void displayDetails() const override {
        cout << "[SportsBike] ";
        Bike::displayDetails();
        cout << "  Top Speed: " << topSpeed << " km/h\n";
    }

    string getType() const override { return "SportsBike"; }

    double calculateValue() const override {
        return price * 1.0 + topSpeed * 10;
    }

    int getTopSpeed() const { return topSpeed; }
};


/* ============================================================
 *  USER HIERARCHY
 *
 *  Inheritance relationships:
 *    9.  User         → AbstractUser  (abstract interface)
 *    10. Buyer        → User
 *    11. PremiumBuyer → Buyer
 *    12. Seller       → User
 *    13. Admin        → User          (also friend of Listing)
 * ============================================================ */

class Listing;  // Forward declaration needed for Admin::approveListing

// ---- User ----
// Inheritance 9: implements AbstractUser interface
class User : public AbstractUser {
protected:
    const string userID;
    string name;
    string email;
    int    age;
    string phone;
    static int userCount;

public:
    User()
        : userID("USER-000"), name("N/A"), email("N/A"), age(18), phone("0000") {
        userCount++;
    }

    User(string id, string n, string e, int a, string ph)
        : userID(id), name(n), email(e), age(a), phone(ph) {
        userCount++;
    }

    User(const User& u)
        : userID(u.userID), name(u.name), email(u.email),
          age(u.age), phone(u.phone) {
        userCount++;
    }

    // AbstractUser pure virtual implementations
    void displayUser() const override {
        cout << "ID:" << userID << " Name:" << name
             << " Email:" << email << " Age:" << age << " Ph:" << phone << "\n";
    }
    void   performRole() const override { cout << name << " is a general user\n"; }
    string getRole()     const override { return "User"; }
    string getName()     const override { return name; }

    void updateEmail(string e) { email = e; }
    void updatePhone(string p) { phone = p; }
    void sendMessage(User& receiver, string msg) {
        cout << "MSG " << name << " -> " << receiver.name << ": " << msg << "\n";
    }

    string getEmail() const { return email; }
    int    getAge()   const { return age; }

    static int getUserCount() { return userCount; }

    // Member operator: users are equal if they share the same email
    bool operator==(const User& u) const { return email == u.email; }
    bool operator!=(const User& u) const { return !(*this == u); }
};

int User::userCount = 0;


// ---- Buyer ----
// Inheritance 10: Buyer extends User
class Buyer : public User {
protected:
    Car*   favorites[10];
    int    favCount;
    double budget;
    static int buyerCount;

public:
    Buyer()
        : User(), favCount(0), budget(0) {
        buyerCount++;
        for(int i=0;i<10;i++) favorites[i]=nullptr;
    }

    Buyer(string id, string n, string e, int a, string ph, double bud)
        : User(id, n, e, a, ph), favCount(0), budget(bud) {
        buyerCount++;
        for(int i=0;i<10;i++) favorites[i]=nullptr;
    }

    Buyer(const Buyer& b)
        : User(b), favCount(b.favCount), budget(b.budget) {
        buyerCount++;
        for(int i=0;i<10;i++) favorites[i] = b.favorites[i];
    }

    // Polymorphism: overrides User::performRole()
    void performRole() const override {
        cout << name << " is browsing and buying vehicles\n";
    }
    string getRole() const override { return "Buyer"; }

    void displayUser() const override {
        User::displayUser();
        cout << "  Role: Buyer | Budget: $" << budget
             << " | Favorites: " << favCount << "\n";
    }

    void addFavorite(Car& c) {
        if(favCount < 10) favorites[favCount++] = &c;
    }

    void viewFavorites() const {
        cout << "--- " << name << "'s Favorites ---\n";
        for(int i=0;i<favCount;i++)
            if(favorites[i]) favorites[i]->displayDetails();
    }

    void   setBudget(double b) { budget = b; }
    double getBudget()   const { return budget; }

    static int getBuyerCount() { return buyerCount; }

    // Operator: combine two buyers' budgets (e.g. joint purchase)
    double operator+(const Buyer& b) const { return budget + b.budget; }
};

int Buyer::buyerCount = 0;


// ---- PremiumBuyer ----
// Inheritance 11: PremiumBuyer extends Buyer (multi-level)
class PremiumBuyer : public Buyer {
private:
    string membershipLevel;
    double cashbackRate;

public:
    PremiumBuyer(string id, string n, string e, int a, string ph, double bud,
                 string lvl, double cbk)
        : Buyer(id, n, e, a, ph, bud), membershipLevel(lvl), cashbackRate(cbk) {}

    // Polymorphism: overrides Buyer::performRole()
    void performRole() const override {
        cout << name << " is a PREMIUM buyer (Level:" << membershipLevel
             << ") | Cashback:" << cashbackRate << "%\n";
    }
    string getRole() const override { return "PremiumBuyer"; }

    void displayUser() const override {
        Buyer::displayUser();
        cout << "  Membership:" << membershipLevel
             << " | Cashback:" << cashbackRate << "%\n";
    }

    double getEffectivePrice(double p) const {
        return p * (1.0 - cashbackRate / 100.0);
    }
};


// ---- Seller ----
// Inheritance 12: Seller extends User
class Seller : public User {
private:
    Listing* listings[10];
    int      listCount;
    double   totalEarnings;
    int      totalSales;
    static int sellerCount;

public:
    Seller()
        : User(), listCount(0), totalEarnings(0), totalSales(0) {
        sellerCount++;
        for(int i=0;i<10;i++) listings[i]=nullptr;
    }

    Seller(string id, string n, string e, int a, string ph)
        : User(id, n, e, a, ph), listCount(0), totalEarnings(0), totalSales(0) {
        sellerCount++;
        for(int i=0;i<10;i++) listings[i]=nullptr;
    }

    Seller(const Seller& s)
        : User(s), listCount(s.listCount),
          totalEarnings(s.totalEarnings), totalSales(s.totalSales) {
        sellerCount++;
        for(int i=0;i<10;i++) listings[i] = s.listings[i];
    }

    // Polymorphism: overrides User::performRole()
    void performRole() const override {
        cout << name << " is listing and selling vehicles\n";
    }
    string getRole() const override { return "Seller"; }

    void displayUser() const override {
        User::displayUser();
        cout << "  Role: Seller | Listings:" << listCount
             << " | Sales:" << totalSales
             << " | Earnings:$" << totalEarnings << "\n";
    }

    void addListing(Listing& l) {
        if(listCount < 10) listings[listCount++] = &l;
    }

    void removeListing(int idx) {
        if(idx>=0 && idx<listCount) {
            for(int i=idx;i<listCount-1;i++) listings[i]=listings[i+1];
            listCount--;
        }
    }

    void recordSale(double amount) {
        totalSales++;
        totalEarnings += amount;
        cout << "Sale recorded! Earnings: $" << totalEarnings << "\n";
    }

    void viewListings() const;  // defined after Listing is complete

    double getTotalEarnings() const { return totalEarnings; }
    int    getListCount()     const { return listCount; }

    static int getSellerCount() { return sellerCount; }
};

int Seller::sellerCount = 0;


// ---- Admin ----
// Inheritance 13: Admin extends User
// Admin is also a friend of Listing (see Listing class)
class Admin : public User {
private:
    int    approvedCount;
    int    rejectedCount;
    string accessLevel;
    bool   superAdmin;
    static int adminCount;

public:
    Admin()
        : User(), approvedCount(0), rejectedCount(0),
          accessLevel("Standard"), superAdmin(false) {
        adminCount++;
    }

    Admin(string id, string n, string e, int a, string ph)
        : User(id, n, e, a, ph), approvedCount(0), rejectedCount(0),
          accessLevel("Standard"), superAdmin(false) {
        adminCount++;
    }

    Admin(const Admin& adm)
        : User(adm), approvedCount(adm.approvedCount), rejectedCount(adm.rejectedCount),
          accessLevel(adm.accessLevel), superAdmin(adm.superAdmin) {
        adminCount++;
    }

    // Polymorphism: overrides User::performRole()
    void performRole() const override {
        cout << name << " is managing the marketplace system\n";
    }
    string getRole() const override { return "Admin"; }

    void displayUser() const override {
        User::displayUser();
        cout << "  Role: Admin | Level:" << accessLevel
             << " | Approved:" << approvedCount
             << " | Rejected:" << rejectedCount << "\n";
    }

    // Defined after Listing is fully declared
    void approveListing(Listing& l);
    void rejectListing(Listing& l);

    void promoteToSuper() {
        superAdmin    = true;
        accessLevel   = "Super Admin";
        cout << name << " promoted to Super Admin!\n";
    }

    static int getAdminCount() { return adminCount; }
};

int Admin::adminCount = 0;


/* ============================================================
 *  LISTING
 *  Composition: contains a Car.
 *  Friend class Admin: can write directly to private 'status'.
 * ============================================================ */

class Listing : public Printable {
private:
    const string listingID;
    Car          car;        // Composition
    string       sellerID;
    string       status;
    string       datePosted;
    double       discount;
    int          viewCount;
    static int   listingCount;

public:
    Listing(string lid="LIST-000", Car c=Car(), string sid="N/A",
            string st="Pending", string date="N/A", double disc=0.0)
        : listingID(lid), car(c), sellerID(sid), status(st),
          datePosted(date), discount(disc), viewCount(0) {
        listingCount++;
    }

    Listing(const Listing& l)
        : listingID(l.listingID), car(l.car), sellerID(l.sellerID),
          status(l.status), datePosted(l.datePosted),
          discount(l.discount), viewCount(l.viewCount) {
        listingCount++;
    }

    void approve()                { status = "Approved"; }
    void remove()                 { status = "Removed"; }
    void updateDiscount(double d) { discount = d; }
    void incrementViews()         { viewCount++; }

    void displayListing() const {
        cout << "Listing[" << listingID << "] Seller:" << sellerID
             << " Status:" << status << " Disc:" << discount
             << "% Views:" << viewCount << "\n";
        car.displayDetails();
    }

    // Printable pure virtual implementation
    void print() const override { displayListing(); }

    // ---- Operator Overloads (Listing) ----
    // Justification: + gives combined value for bundle deals; == checks identity by ID
    double operator+(const Listing& l) const {
        return car.calculateValue() + l.car.calculateValue();
    }
    bool operator==(const Listing& l) const { return listingID == l.listingID; }
    bool operator!=(const Listing& l) const { return !(*this == l); }

    Car    getCar()       const { return car; }
    string getStatus()    const { return status; }
    string getSellerID()  const { return sellerID; }
    string getListingID() const { return listingID; }

    static int getListingCount() { return listingCount; }

    // ---- Friend class: Admin ----
    // Justification: Admin needs direct write access to private 'status' for the approval
    //                workflow. A public setStatus() would let anyone change it.
    //                Making Admin a friend restricts that power to Admin only.
    friend class Admin;

    // ---- Friend function: comparePrices ----
    // Justification: Used in search ranking; needs private 'car' data from two
    //                Listing objects simultaneously — cleaner as a standalone friend.
    friend bool comparePrices(const Listing& a, const Listing& b);
};

int Listing::listingCount = 0;

bool comparePrices(const Listing& a, const Listing& b) {
    return a.car.getPrice() < b.car.getPrice();
}

// Admin methods that use Listing (defined here because Listing is now complete)
void Admin::approveListing(Listing& l) {
    l.status = "Approved";   // direct private access via friend class
    approvedCount++;
    cout << name << " APPROVED listing " << l.listingID << "\n";
}

void Admin::rejectListing(Listing& l) {
    l.status = "Rejected";   // direct private access via friend class
    rejectedCount++;
    cout << name << " REJECTED listing " << l.listingID << "\n";
}

// Seller::viewListings defined here (needs full Listing definition)
void Seller::viewListings() const {
    cout << "--- " << name << "'s Listings ---\n";
    for(int i=0;i<listCount;i++)
        if(listings[i]) listings[i]->displayListing();
}


/* ============================================================
 *  MESSAGE & REVIEW (retained from Assignment 1)
 * ============================================================ */

class Message {
private:
    const string messageID;
    string senderID, receiverID, content, timestamp;
    bool   isRead;
    int    priority;
    static int messageCount;

public:
    Message(string mid="MSG-000", string s="N/A", string r="N/A",
            string c="N/A", string t="N/A", bool read=false)
        : messageID(mid), senderID(s), receiverID(r), content(c),
          timestamp(t), isRead(read), priority(1) {
        messageCount++;
    }

    Message(const Message& m)
        : messageID(m.messageID), senderID(m.senderID), receiverID(m.receiverID),
          content(m.content), timestamp(m.timestamp),
          isRead(m.isRead), priority(m.priority) {
        messageCount++;
    }

    void markAsRead()       { isRead = true; }
    void setPriority(int p) { priority = p; }

    void display() const {
        cout << "MSG[" << messageID << "] " << senderID << " -> " << receiverID
             << " Read:" << (isRead?"Y":"N") << "\n  " << content << "\n";
    }

    bool operator==(const Message& m) const { return messageID == m.messageID; }

    static int getCount() { return messageCount; }
};

int Message::messageCount = 0;


class Review {
private:
    const string reviewID;
    string userID, vehicleVIN, comment, timestamp;
    bool   isVerified;
    int    rating;
    static int reviewCount;

public:
    Review(string rid="REV-000", string u="N/A", string vin="N/A",
           int r=0, string c="N/A", string t="N/A")
        : reviewID(rid), userID(u), vehicleVIN(vin),
          comment(c), timestamp(t), isVerified(false), rating(r) {
        reviewCount++;
    }

    Review(const Review& rev)
        : reviewID(rev.reviewID), userID(rev.userID), vehicleVIN(rev.vehicleVIN),
          comment(rev.comment), timestamp(rev.timestamp),
          isVerified(rev.isVerified), rating(rev.rating) {
        reviewCount++;
    }

    void verify()            { isVerified = true; }
    void updateRating(int r) { rating = r; }
    bool isPositive()  const { return rating >= 4; }

    void display() const {
        cout << "Review[" << reviewID << "] User:" << userID
             << " Vehicle:" << vehicleVIN << " Rating:" << rating
             << "/5 Verified:" << (isVerified?"Y":"N") << "\n  " << comment << "\n";
    }

    bool operator>(const Review& r) const { return rating >  r.rating; }
    bool operator<(const Review& r) const { return rating <  r.rating; }

    static int getCount() { return reviewCount; }
};

int Review::reviewCount = 0;


/* ============================================================
 *  SEARCH — Function Overloading
 * ============================================================ */

class Search {
public:
    // Overload 1: by brand
    static void byBrand(Vehicle* v[], int n, const string& brand) {
        cout << "\n--- SEARCH: brand=" << brand << " ---\n";
        for(int i=0;i<n;i++)
            if(v[i] && v[i]->getBrand() == brand)
                v[i]->displayDetails();
    }

    // Overload 2: by max price only
    static void byPrice(Vehicle* v[], int n, double maxPrice) {
        cout << "\n--- SEARCH: price<=" << maxPrice << " ---\n";
        for(int i=0;i<n;i++)
            if(v[i] && v[i]->calculateValue() <= maxPrice)
                v[i]->displayDetails();
    }

    // Overload 3: by price range
    static void byPrice(Vehicle* v[], int n, double minPrice, double maxPrice) {
        cout << "\n--- SEARCH: $" << minPrice << " - $" << maxPrice << " ---\n";
        for(int i=0;i<n;i++) {
            if(v[i]) {
                double val = v[i]->calculateValue();
                if(val >= minPrice && val <= maxPrice)
                    v[i]->displayDetails();
            }
        }
    }

    // Overload 4: by vehicle type string
    static void byType(Vehicle* v[], int n, const string& type) {
        cout << "\n--- SEARCH: type=" << type << " ---\n";
        for(int i=0;i<n;i++)
            if(v[i] && v[i]->getType() == type)
                v[i]->displayDetails();
    }
};


/* ============================================================
 *  MAIN
 * ============================================================ */

int main() {
    cout << "========================================\n";
    cout << "   AB~WHEELS - ASSIGNMENT 2 DEMO\n";
    cout << "========================================\n\n";

    // Engines
    Engine eng1("ENG001", 150, 2.0, "Petrol",   true);
    Engine eng2("ENG002", 200, 2.5, "Diesel",   false);
    Engine eng3("ENG003", 120, 1.8, "Petrol",   false);
    Engine eng4("ENG004", 300, 3.0, "Electric", false);
    Engine eng5("ENG005", 250, 2.8, "Petrol",   true);

    // Vehicles
    Car c1("VIN001","Toyota",   "Corolla",2022,15000,20000,eng1,"Black","Auto",  4,true, false);
    Car c2("VIN002","Honda",    "Civic",  2023,22000,10000,eng2,"White","Manual",4,false,false);
    Car c3("VIN003","Suzuki",   "Swift",  2021,12000,35000,eng3,"Red",  "Manual",4,false,false);
    ElectricCar ec1("VIN004","Tesla","Model3",2024,40000,5000,eng4,"Silver","Auto",4,true,500,85.0);
    Bike      b1("VIN005","Yamaha",  "YBR",   2022, 5000,15000,eng3,false,17);
    SportsBike sb1("VIN006","Kawasaki","Ninja", 2023,18000, 8000,eng5,17,280);

    // ---------- POLYMORPHISM ----------
    cout << "\n====== POLYMORPHISM (virtual displayDetails) ======\n";
    Vehicle* fleet[6] = { &c1, &c2, &ec1, &b1, &sb1, &c3 };
    for(int i=0;i<6;i++) {
        fleet[i]->displayDetails();
        cout << "  Type:" << fleet[i]->getType()
             << " Value:$" << fleet[i]->calculateValue() << "\n\n";
    }

    // ---------- ABSTRACTION ----------
    cout << "\n====== ABSTRACTION (AbstractVehicle pointers) ======\n";
    AbstractVehicle* av[] = { &c1, &ec1, &sb1 };
    for(int i=0;i<3;i++) av[i]->displayDetails();

    cout << "\n====== ABSTRACTION (Printable pointers) ======\n";
    Listing l1("L001",c1,"S001","Pending","2026-01-01",5.0);
    Listing l2("L002",c2,"S002","Pending","2026-01-01",0.0);
    Listing l3("L003",c3,"S001","Pending","2026-01-01",10.0);
    Printable* printables[] = { &c1, &l1, &sb1 };
    for(int i=0;i<3;i++) printables[i]->print();

    // ---------- OPERATOR OVERLOADING ----------
    cout << "\n====== OPERATOR OVERLOADING ======\n";
    cout << "c1 == c2:      " << (c1 == c2 ? "Yes" : "No") << "\n";
    cout << "c1 <  c2:      " << (c1 <  c2 ? "Yes" : "No") << "\n";
    cout << "c1 >  c2:      " << (c1 >  c2 ? "Yes" : "No") << "\n";
    cout << "c1 +  c2:      $" << (c1 + c2) << "\n";
    cout << "operator<<:    " << c1 << "\n";
    cout << "eng2 > eng1:   " << (eng2 > eng1 ? "Yes" : "No") << "\n";
    cout << "isExpensive ec1:" << (isExpensive(ec1) ? "Yes" : "No") << "\n";
    cout << "isExpensive c1: " << (isExpensive(c1)  ? "Yes" : "No") << "\n";
    cout << "l1 == l2:      " << (l1 == l2 ? "Yes" : "No") << "\n";
    cout << "l1 +  l2:      $" << (l1 + l2) << "\n";
    cout << "comparePrices(l1,l2): " << (comparePrices(l1,l2) ? "l1 cheaper" : "l2 cheaper") << "\n";

    Buyer buyer1("B001","Ahmed Khan","ahmed@x.com",25,"0311",20000);
    Buyer buyer2("B002","Amjad Ahmed","amjad@x.com",22,"0312",30000);
    cout << "buyer1==buyer2:" << (buyer1==buyer2 ? "Yes":"No") << "\n";
    cout << "buyer1+buyer2: $" << (buyer1+buyer2) << "\n";

    Review r1("R001","B001","VIN001",5,"Excellent!","2026-01");
    Review r2("R002","B002","VIN002",3,"Average.",  "2026-01");
    cout << "r1 > r2:       " << (r1 > r2 ? "Yes" : "No") << "\n";

    // ---------- USERS & ROLE POLYMORPHISM ----------
    cout << "\n====== USER POLYMORPHISM (performRole) ======\n";
    Seller      seller1("S001","Faisal Meer",  "faisal@x.com",30,"0322");
    Seller      seller2("S002","Nadia Hussain","nadia@x.com", 28,"0323");
    Admin       admin1 ("A001","Admin One",    "admin@x.com", 35,"0300");
    PremiumBuyer pb1("B003","VIP Sara","sara@x.com",29,"0345",100000,"Gold",5.0);

    AbstractUser* users[] = { &buyer1, &buyer2, &seller1, &seller2, &admin1, &pb1 };
    for(int i=0;i<6;i++) {
        users[i]->displayUser();
        users[i]->performRole();
        cout << "\n";
    }

    // ---------- ADMIN & FRIEND CLASS ----------
    cout << "\n====== ADMIN (Friend Class access to Listing::status) ======\n";
    seller1.addListing(l1);
    seller1.addListing(l3);
    seller2.addListing(l2);
    admin1.approveListing(l1);
    admin1.rejectListing(l3);
    admin1.approveListing(l2);
    admin1.displayUser();

    // ---------- BUYERS ----------
    cout << "\n====== BUYERS ======\n";
    buyer1.addFavorite(c1);
    buyer1.addFavorite(c3);
    buyer2.addFavorite(c2);
    buyer1.displayUser();
    buyer1.viewFavorites();

    pb1.displayUser();
    cout << "Effective price for $40000: $" << pb1.getEffectivePrice(40000) << "\n";

    // ---------- SEARCH ----------
    cout << "\n====== SEARCH (Function Overloading) ======\n";
    Search::byBrand(fleet, 6, "Toyota");
    Search::byPrice(fleet, 6, 20000);
    Search::byPrice(fleet, 6, 10000, 25000);
    Search::byType(fleet,  6, "Bike");

    // ---------- MESSAGES & REVIEWS ----------
    cout << "\n====== MESSAGES ======\n";
    Message m1("M001","B001","S001","Is Corolla available?","2026-01",false);
    Message m2("M002","S001","B001","Yes, it is!",          "2026-01",false);
    m1.markAsRead();
    m1.display(); m2.display();
    cout << "m1==m2: " << (m1==m2 ? "Yes":"No") << "\n";

    cout << "\n====== REVIEWS ======\n";
    r1.verify();
    r1.display(); r2.display();

    // ---------- STATIC COUNTS ----------
    cout << "\n====== SYSTEM STATISTICS ======\n";
    cout << "Engines:  " << Engine::getEngineCount()   << "\n";
    cout << "Vehicles: " << Vehicle::getVehicleCount() << "\n";
    cout << "Cars:     " << Car::getCarCount()         << "\n";
    cout << "Bikes:    " << Bike::getBikeCount()       << "\n";
    cout << "Users:    " << User::getUserCount()       << "\n";
    cout << "Buyers:   " << Buyer::getBuyerCount()     << "\n";
    cout << "Sellers:  " << Seller::getSellerCount()   << "\n";
    cout << "Admins:   " << Admin::getAdminCount()     << "\n";
    cout << "Listings: " << Listing::getListingCount() << "\n";
    cout << "Messages: " << Message::getCount()        << "\n";
    cout << "Reviews:  " << Review::getCount()         << "\n";

    cout << "\n========================================\n";
    cout << "      Thank you for using AB~Wheels!\n";
    cout << "========================================\n";

    return 0;
}
