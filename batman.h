#ifndef BATMAN_LIBRARY_H
#define BATMAN_LIBRARY_H

#include <fstream>
#include <limits>
#include <ratio>
#include <sstream>
#include <string>

#include "power_unit.h"

namespace batman {

class power_supply
{
public:
    static power_supply from(std::string identifier)
    {
        auto sys_path = std::stringstream{};
        sys_path << "/sys/class/power_supply/" << identifier << "/";
        return {sys_path.str()};
    }

    template<class PowerUnit = watts>
    PowerUnit energy_full() const
    {
        return read_mw_from_int<PowerUnit>("energy_full");
    }

    template<class PowerUnit = watts>
    PowerUnit energy_full_design() const
    {
        return read_mw_from_int<PowerUnit>("energy_full_design");
    }

    template<class PowerUnit = watts>
    PowerUnit energy_now() const
    {
        return read_mw_from_int<PowerUnit>("energy_now");
    }

private:
    power_supply(std::string sys_path) :
            sys_path{std::move(sys_path)}
    {}

    template<class PowerUnit = watts>
    PowerUnit read_mw_from_int(const std::string& filename) const
    {
        std::stringstream ss;
        ss << sys_path << filename;

        std::ifstream file{ss.str()};
        int mw;
        file >> mw;
        return power_unit_cast<PowerUnit>(milliwatts{mw});
    }

    std::string sys_path;
};

} /* namespace batman */

#endif
