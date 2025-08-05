
add_definitions(-DUSE_WINE_TODOS)

list(APPEND SOURCE
    device8.c
    dinput.c
    driver_bus.c
    driver_hid_poll.c
    driver_hid.c
    force_feedback.c
    hid.c
    hotplug.c
    joystick8.c)

list(APPEND PCH_SKIP_SOURCE
    testlist.c)

add_executable(dinput_winetest
    ${SOURCE}
    ${PCH_SKIP_SOURCE})

target_link_libraries(dinput_winetest dinput_data_formats)
set_module_type(dinput_winetest win32cui)
add_importlibs(dinput_winetest dinput ole32 user32 msvcrt kernel32)
add_pch(dinput_winetest precomp.h "${PCH_SKIP_SOURCE}")
add_rostests_file(TARGET dinput_winetest)
