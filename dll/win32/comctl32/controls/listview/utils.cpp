/*
 * PROJECT:     ReactOS common controls
 * LICENSE:     LGPL-2.1-or-later (https://spdx.org/licenses/LGPL-2.1-or-later)
 * PURPOSE:     Listview control utilities
 * COPYRIGHT:   Copyright 2023 Carl Bialorucki <cbialo2@outlook.com>
 */
#include "listview.h"
#include "utils.h"

/******** Text handling functions *************************************/

/* A text pointer is either NULL, LPSTR_TEXTCALLBACK, or points to a
 * text string. The string may be ANSI or Unicode, in which case
 * the boolean isW tells us the type of the string.
 *
 * The name of the function tell what type of strings it expects:
 *   W: Unicode, T: ANSI/Unicode - function of isW
 */

BOOL is_text(LPCWSTR text)
{
    return text != NULL && text != LPSTR_TEXTCALLBACKW;
}

int textlenT(LPCWSTR text, BOOL isW)
{
    return !is_text(text) ? 0 :
       isW ? lstrlenW(text) : lstrlenA((LPCSTR)text);
}

void textcpynT(LPWSTR dest, BOOL isDestW, LPCWSTR src, BOOL isSrcW, INT max)
{
    if (isDestW)
    if (isSrcW) lstrcpynW(dest, src, max);
    else MultiByteToWideChar(CP_ACP, 0, (LPCSTR)src, -1, dest, max);
    else
    if (isSrcW) WideCharToMultiByte(CP_ACP, 0, src, -1, (LPSTR)dest, max, NULL, NULL);
    else lstrcpynA((LPSTR)dest, (LPCSTR)src, max);
}

LPWSTR textdupTtoW(LPCWSTR text, BOOL isW)
{
    LPWSTR wstr = (LPWSTR)text;

    if (!isW && is_text(text))
    {
    INT len = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)text, -1, NULL, 0);
    wstr = (LPWSTR)Alloc(len * sizeof(WCHAR));
    if (wstr) MultiByteToWideChar(CP_ACP, 0, (LPCSTR)text, -1, wstr, len);
    }
    TRACE("   wstr=%s\n", text == LPSTR_TEXTCALLBACKW ?  "(callback)" : debugstr_w(wstr));
    return wstr;
}

void textfreeT(LPWSTR wstr, BOOL isW)
{
    if (!isW && is_text(wstr)) Free (wstr);
}

/*
 * dest is a pointer to a Unicode string
 * src is a pointer to a string (Unicode if isW, ANSI if !isW)
 */
BOOL textsetptrT(LPWSTR *dest, LPCWSTR src, BOOL isW)
{
    BOOL bResult = TRUE;

    if (src == LPSTR_TEXTCALLBACKW)
    {
    if (is_text(*dest)) Free(*dest);
    *dest = LPSTR_TEXTCALLBACKW;
    }
    else
    {
    LPWSTR pszText = textdupTtoW(src, isW);
    if (*dest == LPSTR_TEXTCALLBACKW) *dest = NULL;
    bResult = Str_SetPtrW(dest, pszText);
    textfreeT(pszText, isW);
    }
    return bResult;
}

/*
 * compares a Unicode to a Unicode/ANSI text string
 */
int textcmpWT(LPCWSTR aw, LPCWSTR bt, BOOL isW)
{
    if (!aw) return bt ? -1 : 0;
    if (!bt) return 1;
    if (aw == LPSTR_TEXTCALLBACKW)
    return bt == LPSTR_TEXTCALLBACKW ? 1 : -1;
    if (bt != LPSTR_TEXTCALLBACKW)
    {
    LPWSTR bw = textdupTtoW(bt, isW);
    int r = bw ? lstrcmpW(aw, bw) : 1;
    textfreeT(bw, isW);
    return r;
    }

    return 1;
}

int lstrncmpiW(LPCWSTR s1, LPCWSTR s2, int n)
{
    n = min(min(n, lstrlenW(s1)), lstrlenW(s2));
    return CompareStringW(LOCALE_USER_DEFAULT, NORM_IGNORECASE, s1, n, s2, n) - CSTR_EQUAL;
}

/******** Debugging functions *****************************************/

LPCSTR debugtext_t(LPCWSTR text, BOOL isW)
{
    if (text == LPSTR_TEXTCALLBACKW) return "(callback)";
    return isW ? debugstr_w(text) : debugstr_a((LPCSTR)text);
}

LPCSTR debugtext_tn(LPCWSTR text, BOOL isW, INT n)
{
    if (text == LPSTR_TEXTCALLBACKW) return "(callback)";
    n = min(textlenT(text, isW), n);
    return isW ? debugstr_wn(text, n) : debugstr_an((LPCSTR)text, n);
}

char* debug_getbuf()
{
    static int index = 0;
    static char buffers[DEBUG_BUFFERS][DEBUG_BUFFER_SIZE];
    return buffers[index++ % DEBUG_BUFFERS];
}

const char* debugrange(const RANGE *lprng)
{
    if (!lprng) return "(null)";
    return wine_dbg_sprintf("[%d, %d]", lprng->lower, lprng->upper);
}

const char* debugscrollinfo(const SCROLLINFO *pScrollInfo)
{
    char* buf = debug_getbuf(), *text = buf;
    int len, size = DEBUG_BUFFER_SIZE;

    if (pScrollInfo == NULL) return "(null)";
    len = snprintf(buf, size, "{cbSize=%u, ", pScrollInfo->cbSize);
    if (len == -1) goto end;
    buf += len; size -= len;
    if (pScrollInfo->fMask & SIF_RANGE)
	len = snprintf(buf, size, "nMin=%d, nMax=%d, ", pScrollInfo->nMin, pScrollInfo->nMax);
    else len = 0;
    if (len == -1) goto end;
    buf += len; size -= len;
    if (pScrollInfo->fMask & SIF_PAGE)
	len = snprintf(buf, size, "nPage=%u, ", pScrollInfo->nPage);
    else len = 0;
    if (len == -1) goto end;
    buf += len; size -= len;
    if (pScrollInfo->fMask & SIF_POS)
	len = snprintf(buf, size, "nPos=%d, ", pScrollInfo->nPos);
    else len = 0;
    if (len == -1) goto end;
    buf += len; size -= len;
    if (pScrollInfo->fMask & SIF_TRACKPOS)
	len = snprintf(buf, size, "nTrackPos=%d, ", pScrollInfo->nTrackPos);
    else len = 0;
    if (len == -1) goto end;
    buf += len;
    goto undo;
end:
    buf = text + strlen(text);
undo:
    if (buf - text > 2) { buf[-2] = '}'; buf[-1] = 0; }
    return text;
} 

const char* debugnmlistview(const NMLISTVIEW *plvnm)
{
    if (!plvnm) return "(null)";
    return wine_dbg_sprintf("iItem=%d, iSubItem=%d, uNewState=0x%x,"
	         " uOldState=0x%x, uChanged=0x%x, ptAction=%s, lParam=%ld",
	         plvnm->iItem, plvnm->iSubItem, plvnm->uNewState, plvnm->uOldState,
		 plvnm->uChanged, wine_dbgstr_point(&plvnm->ptAction), plvnm->lParam);
}

const char* debuglvitem_t(const LVITEMW *lpLVItem, BOOL isW)
{
    char* buf = debug_getbuf(), *text = buf;
    int len, size = DEBUG_BUFFER_SIZE;
    
    if (lpLVItem == NULL) return "(null)";
    len = snprintf(buf, size, "{iItem=%d, iSubItem=%d, ", lpLVItem->iItem, lpLVItem->iSubItem);
    if (len == -1) goto end;
    buf += len; size -= len;
    if (lpLVItem->mask & LVIF_STATE)
	len = snprintf(buf, size, "state=%x, stateMask=%x, ", lpLVItem->state, lpLVItem->stateMask);
    else len = 0;
    if (len == -1) goto end;
    buf += len; size -= len;
    if (lpLVItem->mask & LVIF_TEXT)
	len = snprintf(buf, size, "pszText=%s, cchTextMax=%d, ", debugtext_tn(lpLVItem->pszText, isW, 80), lpLVItem->cchTextMax);
    else len = 0;
    if (len == -1) goto end;
    buf += len; size -= len;
    if (lpLVItem->mask & LVIF_IMAGE)
	len = snprintf(buf, size, "iImage=%d, ", lpLVItem->iImage);
    else len = 0;
    if (len == -1) goto end;
    buf += len; size -= len;
    if (lpLVItem->mask & LVIF_PARAM)
	len = snprintf(buf, size, "lParam=%lx, ", lpLVItem->lParam);
    else len = 0;
    if (len == -1) goto end;
    buf += len; size -= len;
    if (lpLVItem->mask & LVIF_INDENT)
	len = snprintf(buf, size, "iIndent=%d, ", lpLVItem->iIndent);
    else len = 0;
    if (len == -1) goto end;
    buf += len;
    goto undo;
end:
    buf = text + strlen(text);
undo:
    if (buf - text > 2) { buf[-2] = '}'; buf[-1] = 0; }
    return text;
}

const char* debuglvcolumn_t(const LVCOLUMNW *lpColumn, BOOL isW)
{
    char* buf = debug_getbuf(), *text = buf;
    int len, size = DEBUG_BUFFER_SIZE;
    
    if (lpColumn == NULL) return "(null)";
    len = snprintf(buf, size, "{");
    if (len == -1) goto end;
    buf += len; size -= len;
    if (lpColumn->mask & LVCF_SUBITEM)
	len = snprintf(buf, size, "iSubItem=%d, ",  lpColumn->iSubItem);
    else len = 0;
    if (len == -1) goto end;
    buf += len; size -= len;
    if (lpColumn->mask & LVCF_FMT)
	len = snprintf(buf, size, "fmt=%x, ", lpColumn->fmt);
    else len = 0;
    if (len == -1) goto end;
    buf += len; size -= len;
    if (lpColumn->mask & LVCF_WIDTH)
	len = snprintf(buf, size, "cx=%d, ", lpColumn->cx);
    else len = 0;
    if (len == -1) goto end;
    buf += len; size -= len;
    if (lpColumn->mask & LVCF_TEXT)
	len = snprintf(buf, size, "pszText=%s, cchTextMax=%d, ", debugtext_tn(lpColumn->pszText, isW, 80), lpColumn->cchTextMax);
    else len = 0;
    if (len == -1) goto end;
    buf += len; size -= len;
    if (lpColumn->mask & LVCF_IMAGE)
	len = snprintf(buf, size, "iImage=%d, ", lpColumn->iImage);
    else len = 0;
    if (len == -1) goto end;
    buf += len; size -= len;
    if (lpColumn->mask & LVCF_ORDER)
	len = snprintf(buf, size, "iOrder=%d, ", lpColumn->iOrder);
    else len = 0;
    if (len == -1) goto end;
    buf += len;
    goto undo;
end:
    buf = text + strlen(text);
undo:
    if (buf - text > 2) { buf[-2] = '}'; buf[-1] = 0; }
    return text;
}

const char* debuglvhittestinfo(const LVHITTESTINFO *lpht)
{
    if (!lpht) return "(null)";

    return wine_dbg_sprintf("{pt=%s, flags=0x%x, iItem=%d, iSubItem=%d}",
		 wine_dbgstr_point(&lpht->pt), lpht->flags, lpht->iItem, lpht->iSubItem);
}

/* Return the corresponding text for a given scroll value */
LPCSTR debugscrollcode(int nScrollCode)
{
  switch(nScrollCode)
  {
  case SB_LINELEFT: return "SB_LINELEFT";
  case SB_LINERIGHT: return "SB_LINERIGHT";
  case SB_PAGELEFT: return "SB_PAGELEFT";
  case SB_PAGERIGHT: return "SB_PAGERIGHT";
  case SB_THUMBPOSITION: return "SB_THUMBPOSITION";
  case SB_THUMBTRACK: return "SB_THUMBTRACK";
  case SB_ENDSCROLL: return "SB_ENDSCROLL";
  case SB_INTERNAL: return "SB_INTERNAL";
  default: return "unknown";
  }
}

/* Notification functions */

int get_ansi_notification(UINT unicodeNotificationCode)
{
    switch (unicodeNotificationCode)
    {
        case LVN_BEGINLABELEDITA:
        case LVN_BEGINLABELEDITW:
            return LVN_BEGINLABELEDITA;

        case LVN_ENDLABELEDITA:
        case LVN_ENDLABELEDITW:
            return LVN_ENDLABELEDITA;

        case LVN_GETDISPINFOA:
        case LVN_GETDISPINFOW:
            return LVN_GETDISPINFOA;

        case LVN_SETDISPINFOA:
        case LVN_SETDISPINFOW:
            return LVN_SETDISPINFOA;

        case LVN_ODFINDITEMA:
        case LVN_ODFINDITEMW:
            return LVN_ODFINDITEMA;

        case LVN_GETINFOTIPA:
        case LVN_GETINFOTIPW:
            return LVN_GETINFOTIPA;
        /* header forwards */
        case HDN_TRACKA:
        case HDN_TRACKW:
            return HDN_TRACKA;

        case HDN_ENDTRACKA:
        case HDN_ENDTRACKW:
            return HDN_ENDTRACKA;

        case HDN_BEGINDRAG:
            return HDN_BEGINDRAG;

        case HDN_ENDDRAG:
            return HDN_ENDDRAG;

        case HDN_ITEMCHANGINGA:
        case HDN_ITEMCHANGINGW:
            return HDN_ITEMCHANGINGA;

        case HDN_ITEMCHANGEDA:
        case HDN_ITEMCHANGEDW:
            return HDN_ITEMCHANGEDA;

        case HDN_ITEMCLICKA:
        case HDN_ITEMCLICKW:
            return HDN_ITEMCLICKA;

        case HDN_DIVIDERDBLCLICKA:
        case HDN_DIVIDERDBLCLICKW:
            return HDN_DIVIDERDBLCLICKA;

        default:
            break;
    }

    FIXME("unknown notification %x\n", unicodeNotificationCode);
    return unicodeNotificationCode;
}

/* forwards header notifications to listview parent */
LRESULT notify_forward_header(const LISTVIEW_INFO *infoPtr, NMHEADERW *lpnmhW)
{
    LPCWSTR text = NULL, filter = NULL;
    LRESULT ret;
    NMHEADERA *lpnmh = (NMHEADERA*) lpnmhW;

    /* on unicode format exit earlier */
    if (infoPtr->notifyFormat == NFR_UNICODE)
        return SendMessageW(infoPtr->hwndNotify, WM_NOTIFY, lpnmh->hdr.idFrom,
                            (LPARAM)lpnmh);

    /* header always supplies unicode notifications,
       all we have to do is to convert strings to ANSI */
    if (lpnmh->pitem)
    {
        /* convert item text */
        if (lpnmh->pitem->mask & HDI_TEXT)
        {
            text = (LPCWSTR)lpnmh->pitem->pszText;
            lpnmh->pitem->pszText = NULL;
            Str_SetPtrWtoA(&lpnmh->pitem->pszText, text);
        }
        /* convert filter text */
        if ((lpnmh->pitem->mask & HDI_FILTER) && (lpnmh->pitem->type == HDFT_ISSTRING) &&
             lpnmh->pitem->pvFilter)
        {
            filter = (LPCWSTR)((HD_TEXTFILTERA*)lpnmh->pitem->pvFilter)->pszText;
            ((HD_TEXTFILTERA*)lpnmh->pitem->pvFilter)->pszText = NULL;
            Str_SetPtrWtoA(&((HD_TEXTFILTERA*)lpnmh->pitem->pvFilter)->pszText, filter);
        }
    }
    lpnmh->hdr.code = get_ansi_notification(lpnmh->hdr.code);

    ret = SendMessageW(infoPtr->hwndNotify, WM_NOTIFY, lpnmh->hdr.idFrom,
                       (LPARAM)lpnmh);

    /* cleanup */
    if(text)
    {
        Free(lpnmh->pitem->pszText);
        lpnmh->pitem->pszText = (LPSTR)text;
    }
    if(filter)
    {
        Free(((HD_TEXTFILTERA*)lpnmh->pitem->pvFilter)->pszText);
        ((HD_TEXTFILTERA*)lpnmh->pitem->pvFilter)->pszText = (LPSTR)filter;
    }

    return ret;
}

LRESULT notify_hdr(const LISTVIEW_INFO *infoPtr, INT code, LPNMHDR pnmh)
{
    LRESULT result;

    TRACE("(code=%d)\n", code);

    pnmh->hwndFrom = infoPtr->hwndSelf;
    pnmh->idFrom = GetWindowLongPtrW(infoPtr->hwndSelf, GWLP_ID);
    pnmh->code = code;
    result = SendMessageW(infoPtr->hwndNotify, WM_NOTIFY, pnmh->idFrom, (LPARAM)pnmh);

    TRACE("  <= %ld\n", result);

    return result;
}

BOOL notify(const LISTVIEW_INFO *infoPtr, INT code)
{
    NMHDR nmh;
    HWND hwnd = infoPtr->hwndSelf;
    notify_hdr(infoPtr, code, &nmh);
    return IsWindow(hwnd);
}

void notify_itemactivate(const LISTVIEW_INFO *infoPtr, const LVHITTESTINFO *htInfo)
{
    NMITEMACTIVATE nmia;
    LVITEMW item;

    nmia.uNewState = 0;
    nmia.uOldState = 0;
    nmia.uChanged  = 0;
    nmia.uKeyFlags = 0;

    item.mask = LVIF_PARAM|LVIF_STATE;
    item.iItem = htInfo->iItem;
    item.iSubItem = 0;
    item.stateMask = (UINT)-1;
    if (LISTVIEW_GetItemT(infoPtr, &item, TRUE)) {
        nmia.lParam = item.lParam;
        nmia.uOldState = item.state;
        nmia.uNewState = item.state | LVIS_ACTIVATING;
        nmia.uChanged  = LVIF_STATE;
    }

    nmia.iItem = htInfo->iItem;
    nmia.iSubItem = htInfo->iSubItem;
    nmia.ptAction = htInfo->pt;

    if (GetKeyState(VK_SHIFT) & 0x8000) nmia.uKeyFlags |= LVKF_SHIFT;
    if (GetKeyState(VK_CONTROL) & 0x8000) nmia.uKeyFlags |= LVKF_CONTROL;
    if (GetKeyState(VK_MENU) & 0x8000) nmia.uKeyFlags |= LVKF_ALT;

    notify_hdr(infoPtr, LVN_ITEMACTIVATE, (LPNMHDR)&nmia);
}

LRESULT notify_listview(const LISTVIEW_INFO *infoPtr, INT code, LPNMLISTVIEW plvnm)
{
    TRACE("(code=%d, plvnm=%s)\n", code, debugnmlistview(plvnm));
    return notify_hdr(infoPtr, code, (LPNMHDR)plvnm);
}

/* Handles NM_DBLCLK, NM_CLICK, NM_RDBLCLK, NM_RCLICK. Only NM_RCLICK return value is used. */
BOOL notify_click(const LISTVIEW_INFO *infoPtr, INT code, const LVHITTESTINFO *lvht)
{
    NMITEMACTIVATE nmia;
    LVITEMW item;
    HWND hwnd = infoPtr->hwndSelf;
    LRESULT ret;

    TRACE("code=%d, lvht=%s\n", code, debuglvhittestinfo(lvht)); 
    ZeroMemory(&nmia, sizeof(nmia));
    nmia.iItem = lvht->iItem;
    nmia.iSubItem = lvht->iSubItem;
    nmia.ptAction = lvht->pt;
    item.mask = LVIF_PARAM;
    item.iItem = lvht->iItem;
    item.iSubItem = 0;
    if (LISTVIEW_GetItemT(infoPtr, &item, TRUE)) nmia.lParam = item.lParam;
    ret = notify_hdr(infoPtr, code, (NMHDR*)&nmia);
    return IsWindow(hwnd) && ((UINT)code == NM_RCLICK ? !ret : TRUE);
}

BOOL notify_deleteitem(const LISTVIEW_INFO *infoPtr, INT nItem)
{
    NMLISTVIEW nmlv;
    LVITEMW item;
    HWND hwnd = infoPtr->hwndSelf;

    ZeroMemory(&nmlv, sizeof (NMLISTVIEW));
    nmlv.iItem = nItem;
    item.mask = LVIF_PARAM;
    item.iItem = nItem;
    item.iSubItem = 0;
    if (LISTVIEW_GetItemT(infoPtr, &item, TRUE)) nmlv.lParam = item.lParam;
    notify_listview(infoPtr, LVN_DELETEITEM, &nmlv);
    return IsWindow(hwnd);
}

/*
  Send notification. depends on dispinfoW having same
  structure as dispinfoA.
  infoPtr : listview struct
  code : *Unicode* notification code
  pdi : dispinfo structure (can be unicode or ansi)
  isW : TRUE if dispinfo is Unicode
*/
BOOL notify_dispinfoT(const LISTVIEW_INFO *infoPtr, UINT code, LPNMLVDISPINFOW pdi, BOOL isW)
{
    INT length = 0, ret_length;
    LPWSTR buffer = NULL, ret_text;
    BOOL return_ansi = FALSE;
    BOOL return_unicode = FALSE;
    BOOL ret;

    if ((pdi->item.mask & LVIF_TEXT) && is_text(pdi->item.pszText))
    {
    return_unicode = ( isW && infoPtr->notifyFormat == NFR_ANSI);
    return_ansi    = (!isW && infoPtr->notifyFormat == NFR_UNICODE);
    }

    ret_length = pdi->item.cchTextMax;
    ret_text = pdi->item.pszText;

    if (return_unicode || return_ansi)
    {
        if (code != LVN_GETDISPINFOW)
        {
            length = return_ansi ?
               MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pdi->item.pszText, -1, NULL, 0):
               WideCharToMultiByte(CP_ACP, 0, pdi->item.pszText, -1, NULL, 0, NULL, NULL);
        }
        else
        {
            length = pdi->item.cchTextMax;
            *pdi->item.pszText = 0; /* make sure we don't process garbage */
        }

        buffer = (LPWSTR)Alloc((return_ansi ? sizeof(WCHAR) : sizeof(CHAR)) * length);
        if (!buffer) return FALSE;

        if (return_ansi)
            MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pdi->item.pszText, -1,
                            buffer, length);
        else
            WideCharToMultiByte(CP_ACP, 0, pdi->item.pszText, -1, (LPSTR) buffer,
                            length, NULL, NULL);

        pdi->item.pszText = buffer;
        pdi->item.cchTextMax = length;
    }

    if (infoPtr->notifyFormat == NFR_ANSI)
        code = get_ansi_notification(code);

    TRACE(" pdi->item=%s\n", debuglvitem_t(&pdi->item, infoPtr->notifyFormat != NFR_ANSI));
    ret = notify_hdr(infoPtr, code, &pdi->hdr);
    TRACE(" resulting code=%d\n", pdi->hdr.code);

    if (return_ansi || return_unicode)
    {
        if (return_ansi && (pdi->hdr.code == LVN_GETDISPINFOA))
        {
            strcpy((char*)ret_text, (char*)pdi->item.pszText);
        }
        else if (return_unicode && (pdi->hdr.code == LVN_GETDISPINFOW))
        {
            lstrcpyW(ret_text, pdi->item.pszText);
        }
        else if (return_ansi) /* note : pointer can be changed by app ! */
        {
        WideCharToMultiByte(CP_ACP, 0, pdi->item.pszText, -1, (LPSTR) ret_text,
                ret_length, NULL, NULL);
        }
        else
            MultiByteToWideChar(CP_ACP, 0, (LPSTR) pdi->item.pszText, -1,
                ret_text, ret_length);

        pdi->item.pszText = ret_text; /* restores our buffer */
        pdi->item.cchTextMax = ret_length;

        Free(buffer);
        return ret;
    }

    /* if dispinfo holder changed notification code then convert */
    if (!isW && (pdi->hdr.code == LVN_GETDISPINFOW) && (pdi->item.mask & LVIF_TEXT))
    {
        length = WideCharToMultiByte(CP_ACP, 0, pdi->item.pszText, -1, NULL, 0, NULL, NULL);

        buffer = (LPWSTR)Alloc(length * sizeof(CHAR));
        if (!buffer) return FALSE;

        WideCharToMultiByte(CP_ACP, 0, pdi->item.pszText, -1, (LPSTR) buffer,
                ret_length, NULL, NULL);

        strcpy((LPSTR)pdi->item.pszText, (LPSTR)buffer);
        Free(buffer);
    }

    return ret;
}

void customdraw_fill(NMLVCUSTOMDRAW *lpnmlvcd, const LISTVIEW_INFO *infoPtr, HDC hdc,
                const RECT *rcBounds, const LVITEMW *lplvItem)
{
    ZeroMemory(lpnmlvcd, sizeof(NMLVCUSTOMDRAW));
    lpnmlvcd->nmcd.hdc = hdc;
    lpnmlvcd->nmcd.rc = *rcBounds;
    lpnmlvcd->clrTextBk = infoPtr->clrTextBk;
    lpnmlvcd->clrText   = infoPtr->clrText;
    if (!lplvItem) return;
    lpnmlvcd->nmcd.dwItemSpec = lplvItem->iItem + 1;
    lpnmlvcd->iSubItem = lplvItem->iSubItem;
    if (lplvItem->state & LVIS_SELECTED) lpnmlvcd->nmcd.uItemState |= CDIS_SELECTED;
    if (lplvItem->state & LVIS_FOCUSED) lpnmlvcd->nmcd.uItemState |= CDIS_FOCUS;
    if (lplvItem->iItem == infoPtr->nHotItem) lpnmlvcd->nmcd.uItemState |= CDIS_HOT;
    lpnmlvcd->nmcd.lItemlParam = lplvItem->lParam;
}

DWORD notify_customdraw(const LISTVIEW_INFO *infoPtr, DWORD dwDrawStage, NMLVCUSTOMDRAW *lpnmlvcd)
{
    BOOL isForItem = (lpnmlvcd->nmcd.dwItemSpec != 0);
    DWORD result;

    lpnmlvcd->nmcd.dwDrawStage = dwDrawStage;
    if (isForItem) lpnmlvcd->nmcd.dwDrawStage |= CDDS_ITEM;
    if (lpnmlvcd->iSubItem) lpnmlvcd->nmcd.dwDrawStage |= CDDS_SUBITEM;
    if (isForItem) lpnmlvcd->nmcd.dwItemSpec--;
    result = notify_hdr(infoPtr, NM_CUSTOMDRAW, &lpnmlvcd->nmcd.hdr);
    if (isForItem) lpnmlvcd->nmcd.dwItemSpec++;
    return result;
}

void prepaint_setup(const LISTVIEW_INFO *infoPtr, HDC hdc, NMLVCUSTOMDRAW *lpnmlvcd, BOOL SubItem)
{
    COLORREF backcolor, textcolor;

    /* apparently, for selected items, we have to override the returned values */
    if (!SubItem || (infoPtr->dwLvExStyle & LVS_EX_FULLROWSELECT))
    {
        if (lpnmlvcd->nmcd.uItemState & CDIS_SELECTED)
        {
            if (infoPtr->bFocus)
            {
                lpnmlvcd->clrTextBk = comctl32_color.clrHighlight;
                lpnmlvcd->clrText   = comctl32_color.clrHighlightText;
            }
            else if (infoPtr->dwStyle & LVS_SHOWSELALWAYS)
            {
                lpnmlvcd->clrTextBk = comctl32_color.clr3dFace;
                lpnmlvcd->clrText   = comctl32_color.clrBtnText;
            }
        }
    }

    backcolor = lpnmlvcd->clrTextBk;
    textcolor = lpnmlvcd->clrText;

    if (backcolor == CLR_DEFAULT)
        backcolor = comctl32_color.clrWindow;
    if (textcolor == CLR_DEFAULT)
        textcolor = comctl32_color.clrWindowText;

    /* Set the text attributes */
    if (backcolor != CLR_NONE)
    {
    SetBkMode(hdc, OPAQUE);
    SetBkColor(hdc, backcolor);
    }
    else
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, textcolor);
}

DWORD notify_postpaint (const LISTVIEW_INFO *infoPtr, NMLVCUSTOMDRAW *lpnmlvcd)
{
    return notify_customdraw(infoPtr, CDDS_POSTPAINT, lpnmlvcd);
}

/* returns TRUE when repaint needed, FALSE otherwise */
BOOL notify_measureitem(LISTVIEW_INFO *infoPtr)
{
    MEASUREITEMSTRUCT mis;
    mis.CtlType = ODT_LISTVIEW;
    mis.CtlID = GetWindowLongPtrW(infoPtr->hwndSelf, GWLP_ID);
    mis.itemID = -1;
    mis.itemWidth = 0;
    mis.itemData = 0;
    mis.itemHeight= infoPtr->nItemHeight;
    SendMessageW(infoPtr->hwndNotify, WM_MEASUREITEM, mis.CtlID, (LPARAM)&mis);
    if ((UINT)infoPtr->nItemHeight != max(mis.itemHeight, 1))
    {
        infoPtr->nMeasureItemHeight = infoPtr->nItemHeight = max(mis.itemHeight, 1);
        return TRUE;
    }
    return FALSE;
}

/* Iterator functions */
/******** Item iterator functions **********************************/

BOOL ranges_additem(RANGES ranges, INT nItem)
{
    RANGE range = { nItem, nItem + 1 };

    return ranges_add(ranges, range);
}

BOOL ranges_delitem(RANGES ranges, INT nItem)
{
    RANGE range = { nItem, nItem + 1 };

    return ranges_del(ranges, range);
}

/***
 * ITERATOR DOCUMENTATION
 *
 * The iterator functions allow for easy, and convenient iteration
 * over items of interest in the list. Typically, you create an
 * iterator, use it, and destroy it, as such:
 *   ITERATOR i;
 *
 *   iterator_xxxitems(&i, ...);
 *   while (iterator_{prev,next}(&i)
 *   {
 *       //code which uses i.nItem
 *   }
 *   iterator_destroy(&i);
 *
 *   where xxx is either: framed, or visible.
 * Note that it is important that the code destroys the iterator
 * after it's done with it, as the creation of the iterator may
 * allocate memory, which thus needs to be freed.
 *
 * You can iterate both forwards, and backwards through the list,
 * by using iterator_next or iterator_prev respectively.
 *
 * Lower numbered items are draw on top of higher number items in
 * LVS_ICON, and LVS_SMALLICON (which are the only modes where
 * items may overlap). So, to test items, you should use
 *    iterator_next
 * which lists the items top to bottom (in Z-order).
 * For drawing items, you should use
 *    iterator_prev
 * which lists the items bottom to top (in Z-order).
 * If you keep iterating over the items after the end-of-items
 * marker (-1) is returned, the iterator will start from the
 * beginning. Typically, you don't need to test for -1,
 * because iterator_{next,prev} will return TRUE if more items
 * are to be iterated over, or FALSE otherwise.
 *
 * Note: the iterator is defined to be bidirectional. That is,
 *       any number of prev followed by any number of next, or
 *       five versa, should leave the iterator at the same item:
 *           prev * n, next * n = next * n, prev * n
 *
 * The iterator has a notion of an out-of-order, special item,
 * which sits at the start of the list. This is used in
 * LVS_ICON, and LVS_SMALLICON mode to handle the focused item,
 * which needs to be first, as it may overlap other items.
 *
 * The code is a bit messy because we have:
 *   - a special item to deal with
 *   - simple range, or composite range
 *   - empty range.
 * If you find bugs, or want to add features, please make sure you
 * always check/modify *both* iterator_prev, and iterator_next.
 */

/****
 * This function iterates through the items in increasing order,
 * but prefixed by the special item, then -1. That is:
 *    special, 1, 2, 3, ..., n, -1.
 * Each item is listed only once.
 */
BOOL iterator_next(ITERATOR* i)
{
    if (i->nItem == -1)
    {
    i->nItem = i->nSpecial;
    if (i->nItem != -1) return TRUE;
    }
    if (i->nItem == i->nSpecial)
    {
    if (i->ranges) i->index = 0;
    goto pickarange;
    }

    i->nItem++;
testitem:
    if (i->nItem == i->nSpecial) i->nItem++;
    if (i->nItem < i->range.upper) return TRUE;

pickarange:
    if (i->ranges)
    {
    if (i->index < DPA_GetPtrCount(i->ranges->hdpa))
        i->range = *(RANGE*)DPA_GetPtr(i->ranges->hdpa, i->index++);
    else goto end;
    }
    else if (i->nItem >= i->range.upper) goto end;

    i->nItem = i->range.lower;
    if (i->nItem >= 0) goto testitem;
end:
    i->nItem = -1;
    return FALSE;
}

/****
 * This function iterates through the items in decreasing order,
 * followed by the special item, then -1. That is:
 *    n, n-1, ..., 3, 2, 1, special, -1.
 * Each item is listed only once.
 */
BOOL iterator_prev(ITERATOR* i)
{
    BOOL start = FALSE;

    if (i->nItem == -1)
    {
    start = TRUE;
    if (i->ranges) i->index = DPA_GetPtrCount(i->ranges->hdpa);
    goto pickarange;
    }
    if (i->nItem == i->nSpecial)
    {
    i->nItem = -1;
    return FALSE;
    }

testitem:
    i->nItem--;
    if (i->nItem == i->nSpecial) i->nItem--;
    if (i->nItem >= i->range.lower) return TRUE;

pickarange:
    if (i->ranges)
    {
    if (i->index > 0)
        i->range = *(RANGE*)DPA_GetPtr(i->ranges->hdpa, --i->index);
    else goto end;
    }
    else if (!start && i->nItem < i->range.lower) goto end;

    i->nItem = i->range.upper;
    if (i->nItem > 0) goto testitem;
end:
    return (i->nItem = i->nSpecial) != -1;
}

RANGE iterator_range(const ITERATOR *i)
{
    RANGE range;

    if (!i->ranges) return i->range;

    if (DPA_GetPtrCount(i->ranges->hdpa) > 0)
    {
        range.lower = (*(RANGE*)DPA_GetPtr(i->ranges->hdpa, 0)).lower;
        range.upper = (*(RANGE*)DPA_GetPtr(i->ranges->hdpa, DPA_GetPtrCount(i->ranges->hdpa) - 1)).upper;
    }
    else range.lower = range.upper = 0;

    return range;
}

/***
 * Releases resources associated with this iterator.
 */
void iterator_destroy(const ITERATOR *i)
{
    ranges_destroy(i->ranges);
}

/***
 * Create an empty iterator.
 */
void iterator_empty(ITERATOR* i)
{
    ZeroMemory(i, sizeof(*i));
    i->nItem = i->nSpecial = i->range.lower = i->range.upper = -1;
}

/***
 * Create an iterator over a range.
 */
void iterator_rangeitems(ITERATOR* i, RANGE range)
{
    iterator_empty(i);
    i->range = range;
}

/***
 * Create an iterator over a bunch of ranges.
 * Please note that the iterator will take ownership of the ranges,
 * and will free them upon destruction.
 */
void iterator_rangesitems(ITERATOR* i, RANGES ranges)
{
    iterator_empty(i);
    i->ranges = ranges;
}

/***
 * Creates an iterator over the items which intersect frame.
 * Uses absolute coordinates rather than compensating for the current offset.
 */
BOOL iterator_frameditems_absolute(ITERATOR* i, const LISTVIEW_INFO* infoPtr, const RECT *frame)
{
    RECT rcItem, rcTemp;
    RANGES ranges;

    TRACE("(frame=%s)\n", wine_dbgstr_rect(frame));

    /* in case we fail, we want to return an empty iterator */
    iterator_empty(i);

    if (infoPtr->nItemCount == 0)
        return TRUE;

    if (infoPtr->uView == LV_VIEW_ICON || infoPtr->uView == LV_VIEW_SMALLICON)
    {
    INT nItem;

    if (infoPtr->uView == LV_VIEW_ICON && infoPtr->nFocusedItem != -1)
    {
        LISTVIEW_GetItemBox(infoPtr, infoPtr->nFocusedItem, &rcItem);
        if (IntersectRect(&rcTemp, &rcItem, frame))
        i->nSpecial = infoPtr->nFocusedItem;
    }
    if (!(ranges = ranges_create(50))) return FALSE;
    iterator_rangesitems(i, ranges);
    /* to do better here, we need to have PosX, and PosY sorted */
    TRACE("building icon ranges:\n");
    for (nItem = 0; nItem < infoPtr->nItemCount; nItem++)
    {
            rcItem.left = (LONG_PTR)DPA_GetPtr(infoPtr->hdpaPosX, nItem);
        rcItem.top = (LONG_PTR)DPA_GetPtr(infoPtr->hdpaPosY, nItem);
        rcItem.right = rcItem.left + infoPtr->nItemWidth;
        rcItem.bottom = rcItem.top + infoPtr->nItemHeight;
        if (IntersectRect(&rcTemp, &rcItem, frame))
        ranges_additem(i->ranges, nItem);
    }
    return TRUE;
    }
    else if (infoPtr->uView == LV_VIEW_DETAILS)
    {
    RANGE range;

    if (frame->left >= infoPtr->nItemWidth) return TRUE;
    if (frame->top >= infoPtr->nItemHeight * infoPtr->nItemCount) return TRUE;

    range.lower = max(frame->top / infoPtr->nItemHeight, 0);
    range.upper = min((frame->bottom - 1) / infoPtr->nItemHeight, infoPtr->nItemCount - 1) + 1;
    if (range.upper <= range.lower) return TRUE;
    iterator_rangeitems(i, range);
    TRACE("    report=%s\n", debugrange(&i->range));
    }
    else
    {
    INT nPerCol = max((infoPtr->rcList.bottom - infoPtr->rcList.top) / infoPtr->nItemHeight, 1);
    INT nFirstRow = max(frame->top / infoPtr->nItemHeight, 0);
    INT nLastRow = min((frame->bottom - 1) / infoPtr->nItemHeight, nPerCol - 1);
    INT nFirstCol;
    INT nLastCol;
    INT lower;
    RANGE item_range;
    INT nCol;

    if (infoPtr->nItemWidth)
    {
        nFirstCol = max(frame->left / infoPtr->nItemWidth, 0);
            nLastCol  = min((frame->right - 1) / infoPtr->nItemWidth, (infoPtr->nItemCount + nPerCol - 1) / nPerCol);
    }
    else
    {
        nFirstCol = max(frame->left, 0);
            nLastCol  = min(frame->right - 1, (infoPtr->nItemCount + nPerCol - 1) / nPerCol);
    }

    lower = nFirstCol * nPerCol + nFirstRow;

    TRACE("nPerCol=%d, nFirstRow=%d, nLastRow=%d, nFirstCol=%d, nLastCol=%d, lower=%d\n",
          nPerCol, nFirstRow, nLastRow, nFirstCol, nLastCol, lower);

    if (nLastCol < nFirstCol || nLastRow < nFirstRow) return TRUE;

    if (!(ranges = ranges_create(nLastCol - nFirstCol + 1))) return FALSE;
    iterator_rangesitems(i, ranges);
    TRACE("building list ranges:\n");
    for (nCol = nFirstCol; nCol <= nLastCol; nCol++)
    {
        item_range.lower = nCol * nPerCol + nFirstRow;
        if(item_range.lower >= infoPtr->nItemCount) break;
        item_range.upper = min(nCol * nPerCol + nLastRow + 1, infoPtr->nItemCount);
        TRACE("   list=%s\n", debugrange(&item_range));
        ranges_add(i->ranges, item_range);
    }
    }

    return TRUE;
}

/***
 * Creates an iterator over the items which intersect lprc.
 */
BOOL iterator_frameditems(ITERATOR* i, const LISTVIEW_INFO* infoPtr, const RECT *lprc)
{
    RECT frame = *lprc;
    POINT Origin;

    TRACE("(lprc=%s)\n", wine_dbgstr_rect(lprc));

    LISTVIEW_GetOrigin(infoPtr, &Origin);
    OffsetRect(&frame, -Origin.x, -Origin.y);

    return iterator_frameditems_absolute(i, infoPtr, &frame);
}

/***
 * Creates an iterator over the items which intersect the visible region of hdc.
 */
BOOL iterator_visibleitems(ITERATOR *i, const LISTVIEW_INFO *infoPtr, HDC  hdc)
{
    POINT Origin, Position;
    RECT rcItem, rcClip;
    INT rgntype;

    rgntype = GetClipBox(hdc, &rcClip);
    if (rgntype == NULLREGION)
    {
        iterator_empty(i);
        return TRUE;
    }
    if (!iterator_frameditems(i, infoPtr, &rcClip)) return FALSE;
    if (rgntype == SIMPLEREGION) return TRUE;

    /* first deal with the special item */
    if (i->nSpecial != -1)
    {
    LISTVIEW_GetItemBox(infoPtr, i->nSpecial, &rcItem);
    if (!RectVisible(hdc, &rcItem)) i->nSpecial = -1;
    }

    /* if we can't deal with the region, we'll just go with the simple range */
    LISTVIEW_GetOrigin(infoPtr, &Origin);
    TRACE("building visible range:\n");
    if (!i->ranges && i->range.lower < i->range.upper)
    {
    if (!(i->ranges = ranges_create(50))) return TRUE;
    if (!ranges_add(i->ranges, i->range))
        {
        ranges_destroy(i->ranges);
        i->ranges = 0;
        return TRUE;
        }
    }

    /* now delete the invisible items from the list */
    while(iterator_next(i))
    {
    LISTVIEW_GetItemOrigin(infoPtr, i->nItem, &Position);
    rcItem.left = (infoPtr->uView == LV_VIEW_DETAILS) ? Origin.x : Position.x + Origin.x;
    rcItem.top = Position.y + Origin.y;
    rcItem.right = rcItem.left + infoPtr->nItemWidth;
    rcItem.bottom = rcItem.top + infoPtr->nItemHeight;
    if (!RectVisible(hdc, &rcItem))
        ranges_delitem(i->ranges, i->nItem);
    }
    /* the iterator should restart on the next iterator_next */
    TRACE("done\n");

    return TRUE;
}

/* Remove common elements from two iterators */
/* Passed iterators have to point on the first elements */
BOOL iterator_remove_common_items(ITERATOR *iter1, ITERATOR *iter2)
{
    if(!iter1->ranges || !iter2->ranges) {
        int lower, upper;

        if(iter1->ranges || iter2->ranges ||
                (iter1->range.lower<iter2->range.lower && iter1->range.upper>iter2->range.upper) ||
                (iter1->range.lower>iter2->range.lower && iter1->range.upper<iter2->range.upper)) {
            ERR("result is not a one range iterator\n");
            return FALSE;
        }

        if(iter1->range.lower==-1 || iter2->range.lower==-1)
            return TRUE;

        lower = iter1->range.lower;
        upper = iter1->range.upper;

        if(lower < iter2->range.lower)
            iter1->range.upper = iter2->range.lower;
        else if(upper > iter2->range.upper)
            iter1->range.lower = iter2->range.upper;
        else
            iter1->range.lower = iter1->range.upper = -1;

        if(iter2->range.lower < lower)
            iter2->range.upper = lower;
        else if(iter2->range.upper > upper)
            iter2->range.lower = upper;
        else
            iter2->range.lower = iter2->range.upper = -1;

        return TRUE;
    }

    iterator_next(iter1);
    iterator_next(iter2);

    while(1) {
        if(iter1->nItem==-1 || iter2->nItem==-1)
            break;

        if(iter1->nItem == iter2->nItem) {
            int delete1 = iter1->nItem;

            iterator_prev(iter1);
            iterator_prev(iter2);
            ranges_delitem(iter1->ranges, delete1);
            ranges_delitem(iter2->ranges, delete1);
            iterator_next(iter1);
            iterator_next(iter2);
        } else if(iter1->nItem > iter2->nItem)
            iterator_next(iter2);
        else
            iterator_next(iter1);
    }

    iter1->nItem = iter1->range.lower = iter1->range.upper = -1;
    iter2->nItem = iter2->range.lower = iter2->range.upper = -1;
    return TRUE;
}

/* Ranges functions */
/***
 * DESCRIPTION:
 * A compare function for ranges
 *
 * PARAMETER(S)
 * [I] range1 : pointer to range 1;
 * [I] range2 : pointer to range 2;
 * [I] flags : flags
 *
 * RETURNS:
 * > 0 : if range 1 > range 2
 * < 0 : if range 2 > range 1
 * = 0 : if range intersects range 2
 */
INT CALLBACK ranges_cmp(LPVOID range1, LPVOID range2, LPARAM flags)
{
    INT cmp;

    if (((RANGE*)range1)->upper <= ((RANGE*)range2)->lower)
    cmp = -1;
    else if (((RANGE*)range2)->upper <= ((RANGE*)range1)->lower)
    cmp = 1;
    else
    cmp = 0;

    TRACE("range1=%s, range2=%s, cmp=%d\n", debugrange((RANGE*)range1), debugrange((RANGE*)range2), cmp);

    return cmp;
}

void ranges_assert(RANGES ranges, LPCSTR desc, const char *file, int line)
{
    INT i;
    RANGE *prev, *curr;

    TRACE("*** Checking %s:%d:%s ***\n", file, line, desc);
    assert (ranges);
    assert (DPA_GetPtrCount(ranges->hdpa) >= 0);
    ranges_dump(ranges);
    if (DPA_GetPtrCount(ranges->hdpa) > 0)
    {
    prev = (RANGE*)DPA_GetPtr(ranges->hdpa, 0);
    assert (prev->lower >= 0 && prev->lower < prev->upper);
    for (i = 1; i < DPA_GetPtrCount(ranges->hdpa); i++)
    {
        curr = (RANGE*)DPA_GetPtr(ranges->hdpa, i);
        assert (prev->upper <= curr->lower);
        assert (curr->lower < curr->upper);
        prev = curr;
    }
    }
    TRACE("--- Done checking---\n");
}

RANGES ranges_create(int count)
{
    RANGES ranges = (RANGES)Alloc(sizeof(struct tagRANGES));
    if (!ranges) return NULL;
    ranges->hdpa = DPA_Create(count);
    if (ranges->hdpa) return ranges;
    Free(ranges);
    return NULL;
}

void ranges_clear(RANGES ranges)
{
    INT i;

    for(i = 0; i < DPA_GetPtrCount(ranges->hdpa); i++)
    Free(DPA_GetPtr(ranges->hdpa, i));
    DPA_DeleteAllPtrs(ranges->hdpa);
}

void ranges_destroy(RANGES ranges)
{
    if (!ranges) return;
    ranges_clear(ranges);
    DPA_Destroy(ranges->hdpa);
    Free(ranges);
}

RANGES ranges_clone(RANGES ranges)
{
    RANGES clone;
    INT i;

    if (!(clone = ranges_create(DPA_GetPtrCount(ranges->hdpa)))) goto fail;

    for (i = 0; i < DPA_GetPtrCount(ranges->hdpa); i++)
    {
        RANGE *newrng = (RANGE*)Alloc(sizeof(RANGE));
    if (!newrng) goto fail;
    *newrng = *((RANGE*)DPA_GetPtr(ranges->hdpa, i));
        if (!DPA_SetPtr(clone->hdpa, i, newrng))
        {
            Free(newrng);
            goto fail;
        }
    }
    return clone;

fail:
    TRACE ("clone failed\n");
    ranges_destroy(clone);
    return NULL;
}

RANGES ranges_diff(RANGES ranges, RANGES sub)
{
    INT i;

    for (i = 0; i < DPA_GetPtrCount(sub->hdpa); i++)
    ranges_del(ranges, *((RANGE *)DPA_GetPtr(sub->hdpa, i)));

    return ranges;
}

void ranges_dump(RANGES ranges)
{
    INT i;
    for (i = 0; i < DPA_GetPtrCount(ranges->hdpa); i++)
        TRACE("   %s\n", debugrange((RANGE*)DPA_GetPtr(ranges->hdpa, i)));
}

BOOL ranges_contain(RANGES ranges, INT nItem)
{
    RANGE srchrng = { nItem, nItem + 1 };

    TRACE("(nItem=%d)\n", nItem);
    ranges_check(ranges, "before contain");
    return DPA_Search(ranges->hdpa, &srchrng, 0, ranges_cmp, 0, DPAS_SORTED) != -1;
}

INT ranges_itemcount(RANGES ranges)
{
    INT i, count = 0;

    for (i = 0; i < DPA_GetPtrCount(ranges->hdpa); i++)
    {
    RANGE *sel = (RANGE*)DPA_GetPtr(ranges->hdpa, i);
    count += sel->upper - sel->lower;
    }

    return count;
}

BOOL ranges_shift(RANGES ranges, INT nItem, INT delta, INT nUpper)
{
    RANGE srchrng = { nItem, nItem + 1 }, *chkrng;
    INT index;

    index = DPA_Search(ranges->hdpa, &srchrng, 0, ranges_cmp, 0, DPAS_SORTED | DPAS_INSERTAFTER);
    if (index == -1) return TRUE;

    for (; index < DPA_GetPtrCount(ranges->hdpa); index++)
    {
    chkrng = (RANGE*)DPA_GetPtr(ranges->hdpa, index);
        if (chkrng->lower >= nItem)
        chkrng->lower = max(min(chkrng->lower + delta, nUpper - 1), 0);
        if (chkrng->upper > nItem)
        chkrng->upper = max(min(chkrng->upper + delta, nUpper), 0);
    }
    return TRUE;
}

BOOL ranges_add(RANGES ranges, RANGE range)
{
    RANGE srchrgn;
    INT index;

    TRACE("(%s)\n", debugrange(&range));
    ranges_check(ranges, "before add");

    /* try find overlapping regions first */
    srchrgn.lower = range.lower - 1;
    srchrgn.upper = range.upper + 1;
    index = DPA_Search(ranges->hdpa, &srchrgn, 0, ranges_cmp, 0, DPAS_SORTED);

    if (index == -1)
    {
    RANGE *newrgn;

    TRACE("Adding new range\n");

    /* create the brand new range to insert */
        newrgn = (RANGE*)Alloc(sizeof(RANGE));
    if(!newrgn) goto fail;
    *newrgn = range;

    /* figure out where to insert it */
    index = DPA_Search(ranges->hdpa, newrgn, 0, ranges_cmp, 0, DPAS_SORTED | DPAS_INSERTAFTER);
    TRACE("index=%d\n", index);
    if (index == -1) index = 0;

    /* and get it over with */
    if (DPA_InsertPtr(ranges->hdpa, index, newrgn) == -1)
    {
        Free(newrgn);
        goto fail;
    }
    }
    else
    {
    RANGE *chkrgn, *mrgrgn;
    INT fromindex, mergeindex;

    chkrgn = (RANGE*)DPA_GetPtr(ranges->hdpa, index);
    TRACE("Merge with %s @%d\n", debugrange(chkrgn), index);

    chkrgn->lower = min(range.lower, chkrgn->lower);
    chkrgn->upper = max(range.upper, chkrgn->upper);

    TRACE("New range %s @%d\n", debugrange(chkrgn), index);

        /* merge now common ranges */
    fromindex = 0;
    srchrgn.lower = chkrgn->lower - 1;
    srchrgn.upper = chkrgn->upper + 1;

    do
    {
        mergeindex = DPA_Search(ranges->hdpa, &srchrgn, fromindex, ranges_cmp, 0, 0);
        if (mergeindex == -1) break;
        if (mergeindex == index)
        {
        fromindex = index + 1;
        continue;
        }

        TRACE("Merge with index %i\n", mergeindex);

        mrgrgn = (RANGE*)DPA_GetPtr(ranges->hdpa, mergeindex);
        chkrgn->lower = min(chkrgn->lower, mrgrgn->lower);
        chkrgn->upper = max(chkrgn->upper, mrgrgn->upper);
        Free(mrgrgn);
        DPA_DeletePtr(ranges->hdpa, mergeindex);
        if (mergeindex < index) index --;
    } while(1);
    }

    ranges_check(ranges, "after add");
    return TRUE;

fail:
    ranges_check(ranges, "failed add");
    return FALSE;
}

BOOL ranges_del(RANGES ranges, RANGE range)
{
    RANGE *chkrgn;
    INT index;

    TRACE("(%s)\n", debugrange(&range));
    ranges_check(ranges, "before del");

    /* we don't use DPAS_SORTED here, since we need *
     * to find the first overlapping range          */
    index = DPA_Search(ranges->hdpa, &range, 0, ranges_cmp, 0, 0);
    while(index != -1)
    {
    chkrgn = (RANGE*)DPA_GetPtr(ranges->hdpa, index);

    TRACE("Matches range %s @%d\n", debugrange(chkrgn), index);

    /* case 1: Same range */
    if ( (chkrgn->upper == range.upper) &&
         (chkrgn->lower == range.lower) )
    {
        DPA_DeletePtr(ranges->hdpa, index);
        Free(chkrgn);
        break;
    }
    /* case 2: engulf */
    else if ( (chkrgn->upper <= range.upper) &&
          (chkrgn->lower >= range.lower) )
    {
        DPA_DeletePtr(ranges->hdpa, index);
        Free(chkrgn);
    }
    /* case 3: overlap upper */
    else if ( (chkrgn->upper <= range.upper) &&
          (chkrgn->lower < range.lower) )
    {
        chkrgn->upper = range.lower;
    }
    /* case 4: overlap lower */
    else if ( (chkrgn->upper > range.upper) &&
          (chkrgn->lower >= range.lower) )
    {
        chkrgn->lower = range.upper;
        break;
    }
    /* case 5: fully internal */
    else
    {
        RANGE *newrgn;

        if (!(newrgn = (RANGE*)Alloc(sizeof(RANGE)))) goto fail;
        newrgn->lower = chkrgn->lower;
        newrgn->upper = range.lower;
        chkrgn->lower = range.upper;
        if (DPA_InsertPtr(ranges->hdpa, index, newrgn) == -1)
        {
        Free(newrgn);
        goto fail;
        }
        break;
    }

    index = DPA_Search(ranges->hdpa, &range, index, ranges_cmp, 0, 0);
    }

    ranges_check(ranges, "after del");
    return TRUE;

fail:
    ranges_check(ranges, "failed del");
    return FALSE;
}

/* Misc utility functions */
/******** Misc helper functions ************************************/

LRESULT CallWindowProcT(WNDPROC proc, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL isW)
{
    if (isW) return CallWindowProcW(proc, hwnd, uMsg, wParam, lParam);
    else return CallWindowProcA(proc, hwnd, uMsg, wParam, lParam);
}

BOOL is_autoarrange(const LISTVIEW_INFO *infoPtr)
{
    return (infoPtr->dwStyle & LVS_AUTOARRANGE) &&
        (infoPtr->uView == LV_VIEW_ICON || infoPtr->uView == LV_VIEW_SMALLICON);
}

void toggle_checkbox_state(LISTVIEW_INFO *infoPtr, INT nItem)
{
    DWORD state = STATEIMAGEINDEX(LISTVIEW_GetItemState(infoPtr, nItem, LVIS_STATEIMAGEMASK));
    if(state == 1 || state == 2)
    {
        LVITEMW lvitem;
        state ^= 3;
        lvitem.state = INDEXTOSTATEIMAGEMASK(state);
        lvitem.stateMask = LVIS_STATEIMAGEMASK;
        LISTVIEW_SetItemState(infoPtr, nItem, &lvitem);
    }
}

/* this should be called after window style got updated,
   it used to reset view state to match current window style */
void map_style_view(LISTVIEW_INFO *infoPtr)
{
    switch (infoPtr->dwStyle & LVS_TYPEMASK)
    {
    case LVS_ICON:
        infoPtr->uView = LV_VIEW_ICON;
        break;
    case LVS_REPORT:
        infoPtr->uView = LV_VIEW_DETAILS;
        break;
    case LVS_SMALLICON:
        infoPtr->uView = LV_VIEW_SMALLICON;
        break;
    case LVS_LIST:
        infoPtr->uView = LV_VIEW_LIST;
    }
}

/* computes next item id value */
DWORD get_next_itemid(const LISTVIEW_INFO *infoPtr)
{
    INT count = DPA_GetPtrCount(infoPtr->hdpaItemIds);

    if (count > 0)
    {
        ITEM_ID *lpID = (ITEM_ID*)DPA_GetPtr(infoPtr->hdpaItemIds, count - 1);
        return lpID->id + 1;
    }
    return 0;
}