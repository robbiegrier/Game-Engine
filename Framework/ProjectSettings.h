//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef PROJECT_SETTINGS_H
#define PROJECT_SETTINGS_H

// -----------------------------------------------------------------------------
// DO NOT MODIFY this file
// -----------------------------------------------------------------------------
// Warning levels turned on for Wall... that Should be supressed globally
//     Set here to avoid surpressing them in the settings
// -----------------------------------------------------------------------------


// --------------------------------------
// Prevent Console from opening
// --------------------------------------

//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

// --------------------------------------
// Environment check
// --------------------------------------

// minimum versions
#define TEST_WINSDK_VER "10.0.22621.0"
#define TEST_TOOLS_VER "14.35.32215"
#define TEST_COMPILER_VER "193532217"


#endif 

// ---  End of File ---------------
