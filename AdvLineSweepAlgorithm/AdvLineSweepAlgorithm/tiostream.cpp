#pragma once

#include "StdAfx.h"
#include "tiostream.h"
#include <iostream> // for std::[w]cin and std::[w]cout

#ifdef UNICODE
  tostream& tcout = std::wcout;
  tistream& tcin = std::wcin;
#else
  tostream& tcout = std::cout;
  tistream& tcin = std::cin;
#endif
