#ifndef FUELLOG_HPP
#define FUELLOG_HPP

#include <iostream>
using namespace std;

class FuelLog {
protected:
    double* liters;
    int trips;
    double reserveMargin;

public:
    FuelLog();
    FuelLog(int count, double initVal, double extraVal);
    FuelLog(const FuelLog& other);
    virtual ~FuelLog();
    FuelLog& operator=(const FuelLog& other);

    double getTripFuel(int index) const;
    virtual double totalFuel() const;
};

#endif
