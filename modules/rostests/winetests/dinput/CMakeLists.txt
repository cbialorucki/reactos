
add_definitions(-DUSE_WINE_TODOS)
include_directories(BEFORE ${REACTOS_SOURCE_DIR}/sdk/include/wine)
remove_definitions(-DWINVER=0x502 -D_WIN32_IE=0x600 -D_WIN32_WINNT=0x502)
add_definitions(-DWINVER=0x602 -D_WIN32_WINNT=0x602)

list(APPEND SOURCE
    device8.c
    dinput.c
    # driver_bus.c
    # driver_hid_poll.c
    # driver_hid.c
    # force_feedback.c
    # hid.c
    # hotplug.c
    joystick8.c
)

list(APPEND PCH_SKIP_SOURCE
    testlist.c)

add_executable(dinput_winetest
    ${SOURCE}
    ${PCH_SKIP_SOURCE})

# if(MSVC)
#     # Disable warning C4005 (macro redefinition)
#     target_compile_options(user32_winetest PRIVATE "/wd4005")
#     # Disable warning C4477 (printf format warnings)
#     remove_target_compile_option(user32_winetest "/we4477")
# endif()

if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR
   CMAKE_C_COMPILER_ID STREQUAL "Clang")
    target_compile_options(dinput_winetest PRIVATE
        -Wno-format
        -Wno-incompatible-pointer-types
        -Wno-int-conversion)
endif()

target_link_libraries(dinput_winetest dinput_data_formats)
set_module_type(dinput_winetest win32cui)
add_importlibs(dinput_winetest dinput ole32 user32 msvcrt kernel32)
add_pch(dinput_winetest precomp.h "${PCH_SKIP_SOURCE}")
add_rostests_file(TARGET dinput_winetest)
