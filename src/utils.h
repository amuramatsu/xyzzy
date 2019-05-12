// -*-C++-*-
#ifndef _utils_h_
# define _utils_h_

# include <stddef.h>
# include "tutils.h"

void *xmalloc (size_t);
void *xrealloc (void *, size_t);
void xfree (void *);
char *xstrdup (const char *);
wchar_t *xstrdup (const wchar_t *);
void *xmemdup (const void *, size_t);
char *stpcpy (char *, const char *);
wchar_t *stpcpy (wchar_t *, const wchar_t *);
char *stpncpy (char *, const char *, int);
wchar_t *stpncpy (wchar_t *, const wchar_t *, int);
long log2 (u_long);

# define NF_BAD 0
# define NF_INTEGER 1
# define NF_INTEGER_DOT 2
# define NF_FRACTION 3
# define NF_FLOAT 0x100
# define  NF_FLOAT_E (NF_FLOAT | 'e')
# define  NF_FLOAT_S (NF_FLOAT | 's')
# define  NF_FLOAT_F (NF_FLOAT | 'f')
# define  NF_FLOAT_D (NF_FLOAT | 'd')
# define  NF_FLOAT_L (NF_FLOAT | 'l')

int parse_number_format (const Char *, const Char *, int);
int check_integer_format (const char *, int *);
int default_float_format ();

int streq (const Char *, int, const char *);
int streq (const Char *, int, const wchar_t *);
int strequal (const char *, const Char *);
int strequal (const char *, const Char *, int);
int strequal (const wchar_t *, const Char *);
int strequal (const wchar_t *, const Char *, int);
int strcasecmp (const char *, const char *);
int strcasecmp (const wchar_t *, const wchar_t *);
static inline int
strcaseeq (const char *s1, const char *s2)
{
  return !strcasecmp (s1, s2);
}
static inline int
strcaseeq (const wchar_t *s1, const wchar_t *s2)
{
  return !strcasecmp (s1, s2);
}

char *jindex (const char *, int);
wchar_t *jindex (const wchar_t *, int);
wchar_t *jindex (const wchar_t *, wchar_t);
char *jrindex (const char *, int);
wchar_t *jrindex (const wchar_t *, int);
wchar_t *jrindex (const wchar_t *, wchar_t);
char *find_last_slash (const char *);
wchar_t *find_last_slash (const wchar_t *);
char *find_slash (const char *);
wchar_t *find_slash (const wchar_t *);
void convert_backsl_with_sl (char *, int, int);
void convert_backsl_with_sl (wchar_t *, int, int);

inline void
map_backsl_to_sl (char *s)
{
  convert_backsl_with_sl (s, '\\', '/');
}
inline void
map_backsl_to_sl (wchar_t *s)
{
  convert_backsl_with_sl (s, L'\\', L'/');
}

inline void
map_sl_to_backsl (char *s)
{
  convert_backsl_with_sl (s, '/', '\\');
}
inline void
map_sl_to_backsl (wchar_t *s)
{
  convert_backsl_with_sl (s, L'/', L'\\');
}

inline char *
strappend (char *d, const char *s)
{
  return stpcpy (d + strlen (d), s);
}
inline wchar_t *
strappend (wchar_t *d, const wchar_t *s)
{
  return stpcpy (d + wcslen (d), s);
}

static inline int
dir_separator_p (Char c)
{
  return c == '/' || c == '\\';
}

static inline int
dir_separator_p (int c)
{
  return c == '/' || c == '\\';
}

//static inline int
//dir_separator_p (wchar_t c)
//{
//  return c == L'/' || c == L'\\';
//}

void paint_button_off (HDC, const RECT &);
void paint_button_on (HDC, const RECT &);
void fill_rect (HDC, int, int, int, int, COLORREF);
void fill_rect (HDC, const RECT &, COLORREF);
void draw_hline (HDC, int, int, int, COLORREF);
void draw_vline (HDC, int, int, int, COLORREF);

class find_handle
{
  HANDLE h;
public:
  find_handle (HANDLE h_) : h (h_) {}
  ~find_handle () {FindClose (h);}
};

class wnet_enum_handle
{
  HANDLE h;
public:
  wnet_enum_handle (HANDLE h_) : h (h_) {}
  ~wnet_enum_handle () {WNetCloseEnum (h);}
};

class frameDC
{
  HWND f_hwnd;
  HDC f_hdc;
  HGDIOBJ f_obr;
  enum {WIDTH = 2};
public:
  frameDC (HWND, int = 0);
  ~frameDC ();
  void frame_rect (const RECT &, int = WIDTH) const;
  void paint (const RECT &r) const
    {PatBlt (f_hdc, r.left, r.top,
             r.right - r.left, r.bottom - r.top, PATINVERT);}
};

#endif
