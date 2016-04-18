// AdvLineSweepAlgorithm.cpp : Defines the entry point for the console application.
//
#pragma once

#include "stdafx.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "tiostream.h"

#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif  // _DEBUG

#include "AdvLineSweepAlgorithm.h"
#include "Algorithm.h"


// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
      _CrtMemState memStateAtBegin;
      _CrtMemState memStateAtEnd;
      _CrtMemState memStateDiff;

      _CrtMemCheckpoint(&memStateAtBegin);

			if(g_bOptimizedMode)
				tcout << _T("The algorithm is running in optimized mode") << endl << endl;
			else
				tcout << _T("The algorithm is running in non-optimized mode") << endl << endl;

			BOOL						ret(TRUE);
			bool            bHasFreq;
			LARGE_INTEGER   freq;
			LARGE_INTEGER		start; 
			LARGE_INTEGER		stop;

			if(g_bProfileExecutionTime)
			{
				::SetQuadPart(&freq, 0);
				ret = ::QueryPerformanceFrequency(&freq);
				ASSERT(ret);
				bHasFreq = (TRUE == ret ? true : false);

				ret = TRUE;

				if(bHasFreq)
				{
					::SetQuadPart(&start, 0);
					::SetQuadPart(&stop, 0);
					ret = ::QueryPerformanceCounter(&start);
					ASSERT(ret);
				}
			}
			
			// TODO: code your application's behavior here.
      CAlgorithm* pObjLineSweepAlgo = new CAlgorithm();
      if(pObjLineSweepAlgo)
      {
			  pObjLineSweepAlgo->Initialize();
			  pObjLineSweepAlgo->FindIntersections();
        
        SAFEDELETE(pObjLineSweepAlgo);
      }

			if(g_bProfileExecutionTime)
			{
				ret = TRUE;
				if(bHasFreq)
				{
					ret = ::QueryPerformanceCounter(&stop);
					ASSERT(ret);
					double elapsed(0.0);
					elapsed = static_cast<double>(stop.QuadPart - start.QuadPart) / static_cast<double>(freq.QuadPart);

					tcout << _T("The algorithm took ") << elapsed << _T(" seconds to complete") << endl;
					tcout << _T("Total Right neighbour search operation finding time : ") << g_AggRightNeighborSearchTime << _T(" seconds") << endl;
					tcout << _T("Total Left neighbour search operation finding time : ") << g_AggLeftNeighborSearchTime << _T(" seconds") << endl;
				}
			}

      _CrtMemCheckpoint(&memStateAtEnd);

      _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE);
      _CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDOUT);
      _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE);
      _CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT);
      _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE);
      _CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDOUT);
  
      int nDiff = _CrtMemDifference(&memStateDiff, &memStateAtBegin, &memStateAtEnd);
      if(nDiff > 0)
      {
        tcout << _T("Memory leak detected. Total Memory leaked : ") << memStateDiff.lTotalCount << _T(" bytes") << endl;
        tcout << endl << _T("Dump statistics -->") << endl;
        _CrtMemDumpStatistics(&memStateDiff);
        tcout << endl << _T("Dump -->") << endl;
        _CrtDumpMemoryLeaks();
      }
      else
        tcout << endl << _T("No memory leaks.") << endl;
    }
    getchar();
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
