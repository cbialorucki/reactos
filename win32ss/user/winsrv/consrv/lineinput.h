/*
 * LICENSE:         GPL - See /docs/licenses/COPYING
 * PROJECT:         ReactOS Console Server DLL
 * FILE:            win32ss/user/winsrv/consrv/lineinput.h
 * PURPOSE:         Console line input functions
 * PROGRAMMERS:     Jeffrey Morlan
 */

#pragma once

VOID
LineInputKeyDown(PCONSRV_CONSOLE Console,
                 PUNICODE_STRING ExeName,
                 KEY_EVENT_RECORD *KeyEvent);
