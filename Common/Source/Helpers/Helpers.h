//
// Functionality that is used a lot and should really be a part of some sort of library
//
#ifndef _HELPERS_H
#define _HELPERS_H
#ifdef __cplusplus
extern "C" {
#endif

//#ifndef uint
    typedef unsigned int uint;

//#endif

char* textFileToCString(const char*);
char* retrieveXMLElementById(const char* source, const char* id);
char* cStringAdd2(const char* firstString, const char* secondString);
char* cStringAdd3(const char* firstString, const char* secondString, const char* thirdString);

#ifdef __cplusplus
}
#endif
#endif
