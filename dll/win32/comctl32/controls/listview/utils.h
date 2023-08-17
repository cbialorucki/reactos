/*
 * PROJECT:     ReactOS common controls
 * LICENSE:     LGPL-2.1-or-later (https://spdx.org/licenses/LGPL-2.1-or-later)
 * PURPOSE:     Listview control utilities
 * COPYRIGHT:   Copyright 2023 Carl Bialorucki <cbialo2@outlook.com>
 */
/* Text functions */
BOOL is_text(LPCWSTR text);
int textlenT(LPCWSTR text, BOOL isW);
void textcpynT(LPWSTR dest, BOOL isDestW, LPCWSTR src, BOOL isSrcW, INT max);
LPWSTR textdupTtoW(LPCWSTR text, BOOL isW);
void textfreeT(LPWSTR wstr, BOOL isW);
BOOL textsetptrT(LPWSTR *dest, LPCWSTR src, BOOL isW);
int textcmpWT(LPCWSTR aw, LPCWSTR bt, BOOL isW);
int lstrncmpiW(LPCWSTR s1, LPCWSTR s2, int n);

/* Debug functions */
LPCSTR debugtext_t(LPCWSTR text, BOOL isW);
LPCSTR debugtext_tn(LPCWSTR text, BOOL isW, INT n);
char* debug_getbuf();
const char* debugrange(const RANGE *lprng);
const char* debugscrollinfo(const SCROLLINFO *pScrollInfo);
const char* debugnmlistview(const NMLISTVIEW *plvnm);
const char* debuglvitem_t(const LVITEMW *lpLVItem, BOOL isW);
const char* debuglvcolumn_t(const LVCOLUMNW *lpColumn, BOOL isW);
const char* debuglvhittestinfo(const LVHITTESTINFO *lpht);
LPCSTR debugscrollcode(int nScrollCode);

/* Notification functions */
int get_ansi_notification(UINT unicodeNotificationCode);
LRESULT notify_forward_header(const LISTVIEW_INFO *infoPtr, NMHEADERW *lpnmhW);
LRESULT notify_hdr(const LISTVIEW_INFO *infoPtr, INT code, LPNMHDR pnmh);
BOOL notify(const LISTVIEW_INFO *infoPtr, INT code);
void notify_itemactivate(const LISTVIEW_INFO *infoPtr, const LVHITTESTINFO *htInfo);
LRESULT notify_listview(const LISTVIEW_INFO *infoPtr, INT code, LPNMLISTVIEW plvnm);
BOOL notify_click(const LISTVIEW_INFO *infoPtr, INT code, const LVHITTESTINFO *lvht);
BOOL notify_deleteitem(const LISTVIEW_INFO *infoPtr, INT nItem);
BOOL notify_dispinfoT(const LISTVIEW_INFO *infoPtr, UINT code, LPNMLVDISPINFOW pdi, BOOL isW);
void customdraw_fill(NMLVCUSTOMDRAW *lpnmlvcd, const LISTVIEW_INFO *infoPtr, HDC hdc, const RECT *rcBounds, const LVITEMW *lplvItem);
DWORD notify_customdraw (const LISTVIEW_INFO *infoPtr, DWORD dwDrawStage, NMLVCUSTOMDRAW *lpnmlvcd);
void prepaint_setup(const LISTVIEW_INFO *infoPtr, HDC hdc, NMLVCUSTOMDRAW *lpnmlvcd, BOOL SubItem);
DWORD notify_postpaint (const LISTVIEW_INFO *infoPtr, NMLVCUSTOMDRAW *lpnmlvcd);
BOOL notify_measureitem(LISTVIEW_INFO *infoPtr);

/* Iterator functions */
BOOL ranges_additem(RANGES ranges, INT nItem);
BOOL ranges_delitem(RANGES ranges, INT nItem);
BOOL iterator_next(ITERATOR* i);
BOOL iterator_prev(ITERATOR* i);
RANGE iterator_range(const ITERATOR *i);
void iterator_destroy(const ITERATOR *i);
void iterator_empty(ITERATOR* i);
void iterator_rangeitems(ITERATOR* i, RANGE range);
void iterator_rangesitems(ITERATOR* i, RANGES ranges);
BOOL iterator_frameditems_absolute(ITERATOR* i, const LISTVIEW_INFO* infoPtr, const RECT *frame);
BOOL iterator_frameditems(ITERATOR* i, const LISTVIEW_INFO* infoPtr, const RECT *lprc);
BOOL iterator_visibleitems(ITERATOR *i, const LISTVIEW_INFO *infoPtr, HDC  hdc);
BOOL iterator_remove_common_items(ITERATOR *iter1, ITERATOR *iter2);

/* Ranges functions */
INT CALLBACK ranges_cmp(LPVOID range1, LPVOID range2, LPARAM flags);
#define ranges_check(ranges, desc) if (TRACE_ON(listview)) ranges_assert(ranges, desc, __FILE__, __LINE__)
void ranges_assert(RANGES ranges, LPCSTR desc, const char *file, int line);
RANGES ranges_create(int count);
void ranges_clear(RANGES ranges);
void ranges_destroy(RANGES ranges);
RANGES ranges_clone(RANGES ranges);
RANGES ranges_diff(RANGES ranges, RANGES sub);
void ranges_dump(RANGES ranges);
BOOL ranges_contain(RANGES ranges, INT nItem);
INT ranges_itemcount(RANGES ranges);
BOOL ranges_shift(RANGES ranges, INT nItem, INT delta, INT nUpper);
BOOL ranges_add(RANGES ranges, RANGE range);
BOOL ranges_del(RANGES ranges, RANGE range);

/* Misc utility functions */
LRESULT CallWindowProcT(WNDPROC proc, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL isW);
BOOL is_autoarrange(const LISTVIEW_INFO *infoPtr);
void toggle_checkbox_state(LISTVIEW_INFO *infoPtr, INT nItem);
void map_style_view(LISTVIEW_INFO *infoPtr);
DWORD get_next_itemid(const LISTVIEW_INFO *infoPtr);
