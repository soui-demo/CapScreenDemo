// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//  are changed infrequently
//

#pragma once

#pragma warning(disable:4482)
#pragma warning(disable:4996)
#pragma warning(disable:4244)
#pragma warning(disable:4018)

#define  _CRT_SECURE_NO_WARNINGS
#define	 DLL_SOUI
#include <souistd.h>
#include <core/SHostDialog.h>
#include <control/SMessageBox.h>
#include <control/souictrls.h>
#include <res.mgr/sobjdefattr.h>
#include <com-cfg.h>
#include "resource.h"
#define R_IN_CPP	//定义这个开关来
#include "res\resource.h"
using namespace SOUI;

#include <string>
#include <map>
#include <vector>
using namespace std;

#include <math.h>

#include "CBitmap.h"
#include "CGdiDc.h"
#include <GdiPlus.h>
using namespace Gdiplus;
#pragma comment(lib, "Gdiplus.lib")

#ifndef _Post_writable_byte_size_
#define _Post_writable_byte_size_(s)
#endif
