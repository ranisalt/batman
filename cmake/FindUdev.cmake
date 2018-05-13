# Locate Udev library
# This module defines
#   Udev_FOUND
#   Udev_INCLUDE_DIR
#   Udev_LIBRARY

find_path(Udev_INCLUDE_DIR NAMES libudev.h)
find_library(Udev_LIBRARY NAMES udev libudev)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Udev DEFAULT_MSG Udev_INCLUDE_DIR Udev_LIBRARY)

mark_as_advanced(Udev_INCLUDE_DIR Udev_LIBRARY)
