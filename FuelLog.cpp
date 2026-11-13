#include "FuelLog.hpp"

FuelLog::FuelLog() : liters(nullptr), trips(0), reserveMargin(2.0) {}

FuelLog::FuelLog(int count, double initVal, double extraVal) : liters(nullptr), trips(count), reserveMargin(extraVal) {
    if (trips > 0) {
        liters = new double[trips];
        for (int i = 0; i < trips; ++i) {
            liters[i] = initVal + static_cast<double>(i);
        }
    } else {
        trips = 0;
    }
}

// TODO 1: Complete the Copy Constructor (Deep Copy)
FuelLog::FuelLog(const FuelLog& other) {
    (void)other;
}

// TODO 2: Complete the Destructor (Release DMA)
FuelLog::~FuelLog() {
}

// TODO 3: Complete the Copy Assignment Operator (Deep Copy + Self-Assignment Guard)
FuelLog& FuelLog::operator=(const FuelLog& other) {
    (void)other;
    return *this;
}

double FuelLog::getTripFuel(int index) const {
    if (index >= 0 && index < trips && liters != nullptr) {
        return liters[index];
    }
    return static_cast<double>(0);
}

// TODO 4: Implement totalFuel() (Maximum 8 lines of implementation)
double FuelLog::totalFuel() const {
    return static_cast<double>(0);
}
