// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

#ifndef PCH_H
#define PCH_H

#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0601   
#define _WIN32_WINNT 0x0601  
#endif

// TODO: add headers that you want to pre-compile here
#include <Windows.h>
#endif //PCH_H
