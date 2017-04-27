#include "batman.h"

#include <iostream>
#include <locale>

int main(int argc, char** argv)
{
    auto bat = batman::power_supply::from("BAT0");

    std::cout.imbue(std::locale{""});

    auto energy_full = bat.energy_full();
    std::cout << "Energy when full: " << energy_full.count() << "Wh"
              << std::endl;

    auto energy_full_design = bat.energy_full_design();
    std::cout << "Energy when full (design): " << energy_full_design.count()
              << "Wh" << std::endl;

    auto diff_df = batman::power_unit_cast<batman::milliwatts>(
            energy_full_design - energy_full
    );
    std::cout << "> Difference: " << diff_df.count() << "mWh" << std::endl;

    auto energy_now = bat.energy_now();
    std::cout << "Energy now: " << energy_now.count() << "Wh" << std::endl;

    auto diff_fn = batman::power_unit_cast<batman::milliwatts>(
            energy_full - energy_now
    );
    std::cout << "> Full minus now: " << diff_fn.count() << "mWh" << std::endl;

    auto diff_dn = batman::power_unit_cast<batman::milliwatts>(
            energy_full_design - energy_now
    );
    std::cout << "> Full (design) minus now: " << diff_dn.count() << "mWh" << std::endl;

    return 0;
}
