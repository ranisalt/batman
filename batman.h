#ifndef BATMAN_LIBRARY_H
#define BATMAN_LIBRARY_H

#include "power_unit.h"
#include "udev.h"

#include <sstream>
#include <string>

namespace batman {

class power_supply
{

public:
    static power_supply enumerate()
    {
        auto context = udev::context{};
        if (not context) {
            throw std::runtime_error("Could not initialize udev.");
        }

        auto enumerate = udev::enumerate{context};
        enumerate.add_match_subsystem("power_supply");
        enumerate.scan_devices();

        for (auto device: enumerate) {
            std::string type = device.get_sysattr_value("type");
            if (type == "Battery") {
                return std::move(device);
            }
        }

        throw std::runtime_error("No power supply available.");
    }

    template<class PowerUnit = watts>
    PowerUnit energy_full() const
    {
        return read_milliwatts("energy_full");
    }

    template<class PowerUnit = watts>
    PowerUnit energy_full_design() const
    {
        return read_milliwatts("energy_full_design");
    }

    template<class PowerUnit = watts>
    PowerUnit energy_now() const
    {
        return read_milliwatts("energy_now");
    }

private:
    power_supply(udev::device device): device{std::move(device)} {}

    template<class PowerUnit = watts>
    PowerUnit read_milliwatts(const std::string& attr) const
    {
        int mw = std::stoi(device.get_sysattr_value(attr));
        return power_unit_cast<PowerUnit>(milliwatts{mw});
    }

    udev::device device;
};

} /* namespace batman */

#endif
