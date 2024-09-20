/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: himage.c 1.1 1997/12/10 16:41:12 jon Exp $
 * $Locker: $
 *
 * Using images from host environment
 */
#include "brender.h"

#include "host.h"
#include "host_ip.h"

#ifdef __DOS__

void * BR_RESIDENT_ENTRY HostImageLoad(char *name)
{
	return NULL;
}

void BR_RESIDENT_ENTRY HostImageUnload(void *image)
{
}

void * BR_RESIDENT_ENTRY HostImageLookupName(void *img, char *name, br_uint_32 hint)
{
	return NULL;
}

void * BR_RESIDENT_ENTRY HostImageLookupOrdinal(void *img, br_uint_32 ordinal)
{
	return NULL;
}
#endif

#if defined(_WIN32) || defined(_WIN64)

#include <windows.h>

void * BR_RESIDENT_ENTRY HostImageLoad(char *name)
{
	return (void *)LoadLibrary(name);
}

void BR_RESIDENT_ENTRY HostImageUnload(void *image)
{
	FreeLibrary((HMODULE)image);
}

void * BR_RESIDENT_ENTRY HostImageLookupName(void *img, char *name, br_uint_32 hint)
{
	return (void *)GetProcAddress((HMODULE)img, name);
}

void * BR_RESIDENT_ENTRY HostImageLookupOrdinal(void *img, br_uint_32 ordinal)
{
	return NULL;
}
#endif

#ifdef __OS2__

#include <os2.h>

void * BR_RESIDENT_ENTRY HostImageLoad(char *name)
{
	return NULL;
}

void BR_RESIDENT_ENTRY HostImageUnload(void *image)
{
}

void * BR_RESIDENT_ENTRY HostImageLookupName(void *img, char *name, br_uint_32 hint)
{
	return NULL;
}

void * BR_RESIDENT_ENTRY HostImageLookupOrdinal(void *img, br_uint_32 ordinal)
{
	return NULL;
}
#endif

#if defined(__unix__) || defined(__linux__) || (defined (__APPLE__) && defined (__MACH__)) || defined(__AMIGAOS4__)

#include <dlfcn.h>

void *BR_RESIDENT_ENTRY HostImageLoad(char *name)
{
    return dlopen(name, RTLD_NOW);
}

void BR_RESIDENT_ENTRY HostImageUnload(void *image)
{
    dlclose(image);
}

void *BR_RESIDENT_ENTRY HostImageLookupName(void *img, char *name, br_uint_32 hint)
{
    (void)hint;
    return dlsym(img, name);
}

void *BR_RESIDENT_ENTRY HostImageLookupOrdinal(void *img, br_uint_32 ordinal)
{
    (void)img;
    (void)ordinal;
    return NULL;
}

#endif
