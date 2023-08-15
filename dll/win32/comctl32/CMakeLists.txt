add_definitions(
    -D__WINESRC__
    -D_WINE
    -D__ROS_LONG64__
    -D_COMCTL32_)

remove_definitions(-D_WIN32_WINNT=0x502 -DWINVER=0x502)
add_definitions(-D_WIN32_WINNT=0x600 -DWINVER=0x600)

include_directories(BEFORE ${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
spec2def(comctl32.dll comctl32.spec ADD_IMPORTLIB)

if(MSVC)

endif()

list(APPEND SOURCE
    animate.cpp
    button.cpp
    combo.cpp
    comboex.cpp
    comctl32undoc.c
    comctl32.cpp
    datetime.cpp
    dpa.cpp
    draglist.cpp
    dsa.cpp
    edit.cpp
    flatsb.cpp
    header.cpp
    hotkey.cpp
    imagelist.c
    ipaddress.cpp
    listbox.cpp
    listview.cpp
    monthcal.cpp
    nativefont.cpp
    pager.cpp
    progress.cpp
    propsheet.cpp
    rebar.cpp
    smoothscroll.cpp
    static.cpp
    status.cpp
    string.cpp
    syslink.cpp
    tab.cpp
    taskdialog.cpp
    theme_scrollbar.cpp
    theming.cpp
    toolbar.cpp
    tooltips.cpp
    trackbar.cpp
    treeview.cpp
    updown.cpp)

list(APPEND PCH_SKIP_SOURCE
    stubs.cpp)

add_library(comctl32 MODULE
    ${SOURCE}
    ${PCH_SKIP_SOURCE}
    comctl32.rc
    ${CMAKE_CURRENT_BINARY_DIR}/comctl32.def)

if(MSVC)
    # Disable warning C4146: unary minus operator applied to unsigned type, result still unsigned
    # Disable warning C4267: '=': conversion from 'size_t' to 'WORD', possible loss of data
    # Disable warning C4477 (printf format warnings)
    target_compile_options(comctl32 PRIVATE /wd4146 /wd4267 /wd4477)
endif()

set_module_type(comctl32 win32dll UNICODE)
target_link_libraries(comctl32 uuid cpprt wine ${PSEH_LIB})
add_delay_importlibs(comctl32 winmm uxtheme)
add_importlibs(comctl32 user32 gdi32 advapi32 usp10 imm32 msvcrt kernel32 ntdll)
add_pch(comctl32 precomp.h "${PCH_SKIP_SOURCE}")
add_cd_file(TARGET comctl32 DESTINATION reactos/system32 FOR all)

add_cd_file(TARGET comctl32 DESTINATION reactos/winsxs/${WINARCH}_microsoft.windows.common-controls_6595b64144ccf1df_5.82.2600.2982_none_deadbeef FOR all)
add_cd_file(TARGET comctl32 DESTINATION reactos/winsxs/${WINARCH}_microsoft.windows.common-controls_6595b64144ccf1df_6.0.2600.2982_none_deadbeef FOR all)
add_cd_file(FILE ${CMAKE_CURRENT_SOURCE_DIR}/${WINARCH}_microsoft.windows.common-controls_6595b64144ccf1df_5.82.2600.2982_none_deadbeef.manifest DESTINATION reactos/winsxs/manifests FOR all)
add_cd_file(FILE ${CMAKE_CURRENT_SOURCE_DIR}/${WINARCH}_microsoft.windows.common-controls_6595b64144ccf1df_6.0.2600.2982_none_deadbeef.manifest DESTINATION reactos/winsxs/manifests FOR all)
