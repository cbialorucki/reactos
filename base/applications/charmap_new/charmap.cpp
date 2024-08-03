/*
* PROJECT:     ReactOS Character Map
* LICENSE:     GPL - See /docs/licenses/COPYING
* FILE:        base/applications/charmap/charmap.cpp
* COPYRIGHT:   Copyright 2006 Ged Murphy <gedmurphy@gmail.com>
*/

#include "precomp.h"
#include "MainWindow.h"

int WINAPI
wWinMain(HINSTANCE hThisInstance,
         HINSTANCE hPrevInstance,
         LPWSTR lpCmdLine,
         int nCmdShow)
{
    CCharMapWindow CharMap;
    return CharMap.Create(hThisInstance, nCmdShow);
}
