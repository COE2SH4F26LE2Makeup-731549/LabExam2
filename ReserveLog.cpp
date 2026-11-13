#include "ReserveLog.hpp"

ReserveLog::ReserveLog() : FuelLog() {}

ReserveLog::ReserveLog(int count, double initVal, double extraVal) : FuelLog(count, initVal, extraVal) {}

ReserveLog::ReserveLog(const ReserveLog& other) : FuelLog(other) {}

ReserveLog::~ReserveLog() {}

ReserveLog& ReserveLog::operator=(const ReserveLog& other) {
    if (this != &other) {
        FuelLog::operator=(other);
    }
    return *this;
}

// TODO 5: Override totalFuel() (Maximum 8 lines of implementation)
double ReserveLog::totalFuel() const {
    return static_cast<double>(0);
}
