#ifndef BATMAN_UDEV_H
#define BATMAN_UDEV_H

#include <libudev.h>

#include <memory>

namespace batman::udev {

class context
{
public:
    context(): ctx{udev_new(), udev_unref} {}

    auto get() const noexcept { return ctx.get(); }

    explicit operator bool() const noexcept { return bool(ctx); }

private:
    std::unique_ptr<::udev, decltype(&udev_unref)> ctx;
};

class device {
public:
    device(const context& ctx, const char* path):
        dev{udev_device_new_from_syspath(ctx.get(), path), udev_device_unref}
    {}

    std::string get_sysattr_value(const std::string& sysattr) const {
        return udev_device_get_sysattr_value(dev.get(), sysattr.c_str());
    }

    bool operator!=(const device& rhs) const noexcept {
        return dev != rhs.dev;
    }

private:
    std::unique_ptr<udev_device, decltype(&udev_device_unref)> dev;
};

class enumerate
{
public:
    enumerate(const context& ctx):
        ctx{ctx}, enm{udev_enumerate_new(ctx.get()), udev_enumerate_unref} {}

    bool add_match_subsystem(const char* subsystem) {
        return udev_enumerate_add_match_subsystem(enm.get(), subsystem) > 0;
    }

    bool scan_devices() {
        return udev_enumerate_scan_devices(enm.get()) > 0;
    }

    class iterator {
    public:
        iterator(const context& ctx, udev_list_entry* entry):
            ctx{ctx}, entry{entry}
        {}

        using difference_type = int;
        using iterator_category = std::input_iterator_tag;
        using pointer = device*;
        using reference = device&;
        using value_type = device;

        bool operator==(const iterator& rhs) const noexcept {
            return entry == rhs.entry;
        }

        bool operator!=(const iterator& rhs) const noexcept {
            return entry != rhs.entry;
        }

        iterator& operator++() noexcept {
            entry = udev_list_entry_get_next(entry);
            return *this;
        }

        device operator*() {
            return {ctx, udev_list_entry_get_name(entry)};
        }

    private:
        const context& ctx;
        udev_list_entry* entry;
    };

    iterator begin() const {
        return {ctx, udev_enumerate_get_list_entry(enm.get())};
    }

    iterator end() const {
        return {ctx, nullptr};
    }

private:
    const context& ctx;
    std::unique_ptr<::udev_enumerate, decltype(&udev_enumerate_unref)> enm;
};

} /* namespace batman::udev */

#endif //BATMAN_UDEV_H
