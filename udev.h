#ifndef BATMAN_UDEV_H
#define BATMAN_UDEV_H

#include <libudev.h>

#include <memory>

namespace batman::udev {

using context = std::unique_ptr<::udev, decltype(&udev_unref)>;
context make_context()
{
    return {udev_new(), udev_unref};
}

using enumerate = std::unique_ptr<::udev_enumerate, decltype(&udev_enumerate_unref)>;
enumerate make_enumerate(const context& udev)
{
    return {udev_enumerate_new(udev.get()), udev_enumerate_unref};
}

using device = std::unique_ptr<udev_device, decltype(&udev_device_unref)>;
device make_device(const context& udev, const char* path)
{
    return {udev_device_new_from_syspath(udev.get(), path), udev_device_unref};
}

} /* namespace batman::udev */

#endif //BATMAN_UDEV_H
