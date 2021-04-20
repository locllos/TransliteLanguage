#include "../headers/backEndHeader.h"
#include "constants.h"

#define dbg
#define dmp

#ifndef dbg
#define dbg logWrite( "dbg: (func: %s | line: %d)", __func__, __LINE__); 
#endif

#ifndef dmp
#define dmp logWrite("dmp: (func: %s | line: %d)", __func__, __LINE__); logNode(node);
#endif




