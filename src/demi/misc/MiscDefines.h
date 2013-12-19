/********************************************************************
    File:    CommonDefines.h
    Creator: demiwangya
********************************************************************/

#pragma once

#include "PlatformSpecific.h"

#pragma warning(disable:4251)   // dll-interface
#pragma warning(disable:4127)

//-----------------------------------------------------------------------------
#ifdef DEMI_STATIC_API
#    define DI_MISC_API
#else
#    ifdef DI_MISC_EXPORT
#        define DI_MISC_API  __declspec(dllexport)
#    else
#        define DI_MISC_API __declspec(dllimport)
#    endif
#endif

#include "DebugAssert.h"

//-----------------------------------------------------------------------------
#define DI_ERROR(...) \
    Demi::DiLogManager::GetInstancePtr()->Output(Demi::LOG_LEVEL_ERROR, __FILE__, __LINE__, __VA_ARGS__); \
    Demi::DiLogManager::GetInstancePtr()->Error( __FILE__ , __LINE__ , __VA_ARGS__); __asm int 3

#define DI_WARNING(...) Demi::DiLogManager::GetInstancePtr()->Output( Demi::LOG_LEVEL_WARNING,\
    __FILE__ , __LINE__ , __VA_ARGS__)

#define DI_LOG(...) Demi::DiLogManager::GetInstancePtr()->Output( Demi::LOG_LEVEL_LOG,\
    __FILE__ , __LINE__ , __VA_ARGS__)

#ifdef _DEBUG
#    define DI_DEBUG(...) Demi::DiLogManager::GetInstancePtr()->Output( Demi::LOG_LEVEL_DEBUG,\
        __FILE__ , __LINE__ , __VA_ARGS__)
#else
#    define DI_DEBUG(...)
#endif
    
//-----------------------------------------------------------------------------
#ifdef _DEMI_RELEASE
#define DISABLE_MEMORY_TRACE
#endif

#ifndef SAFE_DELETE
#    define SAFE_DELETE(P)       if(P) { delete (P);         (P) = NULL; }
#endif

#ifndef SAFE_ARRAY_DELETE
#    define SAFE_ARRAY_DELETE(P) if(P) { delete[] (P);       (P) = NULL; }
#endif

#ifndef SAFE_RELEASE
#    define SAFE_RELEASE(P)      if(P) { (P)->Release();     (P) = NULL; }
#endif

#define PROFILE(name) Demi::DiProfileSample kSample(name)
#define PROFILE_BEGINE(name) Demi::DiProfileManager::GetInstancePtr()->Start(name);
#define PROFILE_END() Demi::DiProfileManager::GetInstancePtr()->Stop();

typedef int IndexT;
typedef int SizeT;
const int InvalidIndex = -1;

#define THREADLOCAL __declspec(thread)
#define PREFAST_SUPPRESS_WARNING(W) __pragma(warning(suppress:W))

typedef void *THREAD_HANDLE;
typedef void *EVENT_HANDLE;

#define BIT(index) (1 << (index))

