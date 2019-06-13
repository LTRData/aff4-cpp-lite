// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DEVIO_DLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DEVIO_DLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef LIBAFF4DEVIO_EXPORTS
#define DEVIO_DLL_API extern "C" __declspec(dllexport)
#else
#define DEVIO_DLL_API __declspec(dllimport)
#endif

DEVIO_DLL_API dllopen_decl dllopen;

DEVIO_DLL_API dllread_decl dllread;

DEVIO_DLL_API dllclose_decl dllclose;

DEVIO_DLL_API uint32_t __cdecl getsectorsize(void *handle);

DEVIO_DLL_API uint32_t __cdecl getimagecount(const char *file);

DEVIO_DLL_API int __cdecl getlasterrorcode();

DEVIO_DLL_API const char * __cdecl geterrormessage(int errorcode);


