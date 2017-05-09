#ifndef BATMAN_LIBRARY_H
#define BATMAN_LIBRARY_H

#include <experimental/filesystem>
#include <fstream>
#include <limits>
#include <ratio>
#include <sstream>
#include <string>

#include "power_unit.h"

namespace batman {

namespace filesystem = std::experimental::filesystem;

class power_supply
{

public:
    static power_supply from(const std::string& identifier)
    {
        auto sys_path = filesystem::path{"/sys/class/power_supply"};
        sys_path.append(identifier);
        return {sys_path};
    }

    template<class PowerUnit = watts>
    PowerUnit energy_full() const
    {
        return read_milliwatts(build_path("energy_full"));
    }

    template<class PowerUnit = watts>
    PowerUnit energy_full_design() const
    {
        return read_milliwatts(build_path("energy_full_design"));
    }

    template<class PowerUnit = watts>
    PowerUnit energy_now() const
    {
        return read_milliwatts(build_path("energy_now"));
    }

private:
    power_supply(filesystem::path sys_path) :
            sys_path{std::move(sys_path)}
    {}

    filesystem::path build_path(const std::string& filename) const
    {
        auto path = filesystem::path{sys_path};
        path.append(filename);
        return path;
    }

    template<class PowerUnit = watts>
    PowerUnit read_milliwatts(const filesystem::path& path) const
    {
        std::ifstream file{path};
        int mw;
        file >> mw;
        return power_unit_cast<PowerUnit>(milliwatts{mw});
    }

    filesystem::path sys_path;
};

} /* namespace batman */

#endif
