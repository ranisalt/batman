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
        std::stringstream filename;
        filename << sys_path << "energy_full";

        std::ifstream file{filename.str()};
        int mw;
        file >> mw;
        return power_unit_cast<PowerUnit>(milliwatts{mw});
    }

    template<class PowerUnit = watts>
    PowerUnit energy_full_design() const
    {
        std::stringstream filename;
        filename << sys_path << "energy_full_design";

        std::ifstream file{filename.str()};
        int mw;
        file >> mw;
        return power_unit_cast<PowerUnit>(milliwatts{mw});
    }

    template<class PowerUnit = watts>
    PowerUnit energy_now() const
    {
        std::stringstream filename;
        filename << sys_path << "energy_now";

        std::ifstream file{filename.str()};
        int mw;
        file >> mw;
        return power_unit_cast<PowerUnit>(milliwatts{mw});
    }

private:
    power_supply(std::string sys_path) :
            sys_path{std::move(sys_path)}
    {}

    std::string sys_path;
};

} /* namespace batman */

#endif
