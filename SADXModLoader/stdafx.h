// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// Windows Header Files:
#include <Windows.h>

// C
#include <stdlib.h>

// C (C++ namespace)
#include <cstdint>
#include <cstdio>

// C++
#include <algorithm>
#include <deque>
#include <stack>
#include <fstream>
#include <sstream>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>

#include "direct3d.h"
#include "ninja.h"
#include "SADXModLoader.h"
#include "Events.h"
#include "FileReplacement.h"
#include "Trampoline.h"
#include "FileSystem.h"
#include "pvmx.h"
#include "prs.h"

#if defined(_MSC_VER) && _MSC_VER < 1900
#define snprintf _snprintf
#endif
