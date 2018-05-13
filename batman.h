#ifndef BATMAN_LIBRARY_H
#define BATMAN_LIBRARY_H

#include "power_unit.h"
#include "udev.h"

#include <sstream>
#include <string>

#include <libudev.h>

namespace batman {

class power_supply
{

public:
    static power_supply enumerate()
    {
        udev::context context = udev::make_context();
        if (not context) {
            throw std::runtime_error("Could not initialize udev.");
        }

        udev::enumerate enumerate = udev::make_enumerate(context);
        udev_enumerate_add_match_subsystem(enumerate.get(), "power_supply");
        udev_enumerate_scan_devices(enumerate.get());

        for (auto device = udev_enumerate_get_list_entry(enumerate.get());
                device != nullptr; device = udev_list_entry_get_next(device)) {
            udev::device dev = udev::make_device(context, udev_list_entry_get_name(device));

            std::string type = udev_device_get_sysattr_value(dev.get(), "type");
            if (type == "Battery") {
                return std::move(dev);
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
        int mw = atoi(udev_device_get_sysattr_value(device.get(), attr.c_str()));
        return power_unit_cast<PowerUnit>(milliwatts{mw});
    }

    udev::device device;
};

} /* namespace batman */

#endif
