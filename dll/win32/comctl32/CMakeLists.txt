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
    controls/animate/animate.cpp
    controls/button/button.cpp
    controls/combo/combo.cpp
    controls/comboex/comboex.cpp
    comctl32.cpp
    controls/datetime/datetime.cpp
    utils/debug.cpp
    utils/dpa.cpp
    controls/draglist/draglist.cpp
    utils/dsa.cpp
    controls/edit/edit.cpp
    controls/flatsb/flatsb.cpp
    controls/header/header.cpp
    controls/hotkey/hotkey.cpp
    controls/imagelist/imagelist.c
    controls/ipaddress/ipaddress.cpp
    controls/listbox/listbox.cpp
    controls/listview/listview.cpp
    controls/listview/utils.cpp
    controls/monthcal/monthcal.cpp
    controls/nativefont/nativefont.cpp
    controls/pager/pager.cpp
    controls/progress/progress.cpp
    controls/propsheet/propsheet.cpp
    controls/rebar/rebar.cpp
    utils/smoothscroll.cpp
    controls/static/static.cpp
    controls/status/status.cpp
    utils/string.cpp
    controls/syslink/syslink.cpp
    controls/tab/tab.cpp
    controls/taskdialog/taskdialog.cpp
    controls/toolbar/toolbar.cpp
    controls/tooltips/tooltips.cpp
    controls/trackbar/trackbar.cpp
    controls/treeview/treeview.cpp
    controls/updown/updown.cpp
    utils/theme_scrollbar.cpp
    utils/theming.cpp)

list(APPEND PCH_SKIP_SOURCE
    utils/stubs.cpp)

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
