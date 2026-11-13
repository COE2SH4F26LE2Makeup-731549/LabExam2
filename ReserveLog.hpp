#ifndef RESERVELOG_HPP
#define RESERVELOG_HPP

#include "FuelLog.hpp"

class ReserveLog : public FuelLog {
public:
    ReserveLog();
    ReserveLog(int count, double initVal, double extraVal);
    ReserveLog(const ReserveLog& other);
    virtual ~ReserveLog();
    ReserveLog& operator=(const ReserveLog& other);

    virtual double totalFuel() const override;
};

#endif
