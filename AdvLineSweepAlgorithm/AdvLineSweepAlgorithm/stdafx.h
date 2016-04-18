// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include <afx.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

extern bool g_bOptimizedMode;
extern bool g_bProfileExecutionTime;
extern double g_AggRightNeighborSearchTime;
extern double g_AggLeftNeighborSearchTime;

// TODO: reference additional headers your program requires here
typedef struct tagPoint 
{
	double nXCoordinate;
	double nYCoordinate;

	bool operator==(tagPoint pt)
	{
		if(nXCoordinate == pt.nXCoordinate && nYCoordinate == pt.nYCoordinate)
			return true;
		else
			return false;
	}

  bool operator!=(tagPoint pt)
	{
		if(nXCoordinate == pt.nXCoordinate && nYCoordinate == pt.nYCoordinate)
			return false;
		else
			return true;
	}
}SPACE_POINT;

typedef enum tagComparisonResult
{
	LESS_THAN = 0,
	MORE_THAN = 1,
	EQUAL_TO = 2,
	UNDEFINED = 3
}COMPARISON_RESULT;

typedef enum tagPointPosition
{
	PT_POSITION_UPPER = 0,
	PT_POSITION_MIDDLE = 1,
	PT_POSITION_LOWER = 2
}POINT_POSITION;

#define SAFEDELETE(pData) \
        {\
           if(pData)\
           {\
              delete pData;\
              pData = 0;\
            }\
         }

#define SAFEDELETEARRAY(pArray) \
        {\
            if(pArray)\
            {\
                delete []pArray;\
                pArray = 0;\
            }\
        }

inline void SetQuadPart(LARGE_INTEGER* pUli, __int64 qpart) { pUli->QuadPart = qpart; }
