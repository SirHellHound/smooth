#ifndef _ICONV_WRAP_
#define _ICONV_WRAP_

#if !defined __WIN32__ || defined _ICONVDLL_
#	include <libiconv/iconv.h>
#else

#undef iconv_t

typedef void* iconv_t;

#if defined __cplusplus
extern "C"
{
#endif

	extern int	 use_iconv;

	extern size_t	 (*iconv)(iconv_t, char **, size_t *, char **, size_t *);
	extern iconv_t	 (*iconv_open)(const char *, const char *);
	extern int	 (*iconv_close)(iconv_t);

#if defined __cplusplus
}
#endif

#endif
#endif