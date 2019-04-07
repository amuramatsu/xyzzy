// -*-C++-*-
#ifndef _string_h_
# define _string_h_

/* string */

# include "array.h"

# define MAX_STRING_LENGTH (INT_MAX / sizeof (Char))

class lsimple_string: public lbase_vector
{
};

class lcomplex_string: public lbase_complex_vector
{
};

# define stringp(X) \
  object_type_mask_p ((X), TAvector | TAstring, TAvector | TAstring)
# define simple_string_p(X) typep ((X), Tsimple_string)
# define complex_string_p(X) typep ((X), Tcomplex_string)

inline void
check_string (lisp x)
{
  if (!stringp (x))
    FEtype_error (x, Qstring);
}

inline void
check_simple_string (lisp x)
{
  check_type (x, Tsimple_string, Qsimple_string);
}

# define xstring_length xvector_length
# define xstring_dimension xvector_dimension

inline Char *&
xstring_contents (lisp x)
{
  assert (stringp (x));
  return (Char *&)xbase_vector_contents (x);
}

inline lsimple_string *
make_simple_string ()
{
  lsimple_string *p = ldata <lsimple_string, Tsimple_string>::lalloc ();
  p->common_init ();
  return p;
}

inline lcomplex_string *
make_complex_string ()
{
  lcomplex_string *p = ldata <lcomplex_string, Tcomplex_string>::lalloc ();
  p->common_init ();
  return p;
}

inline int
string_equal (lisp x, lisp y)
{
  assert (stringp (x));
  assert (stringp (y));
  return (xstring_length (x) == xstring_length (y)
          && !bcmp (xstring_contents (x), xstring_contents (y),
                    xstring_length (x)));
}

int string_equalp (const Char *, int, const char *, int);
int string_equalp (const Char *, int, const Char *, int);

inline int
string_equalp (lisp x, lisp y)
{
  return string_equalp (xstring_contents (x), xstring_length (x),
                        xstring_contents (y), xstring_length (y));
}

int string_equalp (lisp, int, lisp, int, int);

lisp parse_integer (lisp, int, int &, int, int);

int update_column (int, Char);
int update_column (int, const Char *, int);
int update_column (int, Char, int);
size_t s2wl (const char *);
Char *s2w (Char *, size_t, const char **);
Char *s2w (Char *, const char *);
Char *s2w (const char *, size_t);
Char *a2w (Char *, size_t, const char **);
void a2w (Char *, const char *, size_t);
Char *a2w (Char *, const char *);
Char *a2w (const char *, size_t);
size_t w2sl (const Char *, size_t);
char *w2s (char *, const Char *, size_t);
char *w2s (const Char *, size_t);
char *w2s (char *, char *, const Char *, size_t);
char *w2s_quote (char *, char *, const Char *, size_t, int, int);

size_t s2wl (const char *, const char *, int);
Char *s2w (Char *, const char *, const char *, int);
void w2s_chunk (char *, char *, const Char *, size_t);

ucs2_t *i2w (const Char *, int, ucs2_t *);
int i2wl (const Char *, int);

size_t u2wl (const wchar_t *);
Char *u2w (Char *, size_t, const wchar_t **);
Char *u2w (Char *, const wchar_t *);
Char *u2w (const wchar_t *, size_t);
size_t w2ul (const Char *, size_t);
wchar_t *w2u (wchar_t *, const Char *, size_t);
wchar_t *w2u (const Char *, size_t);
wchar_t *w2u (wchar_t *, wchar_t *, const Char *, size_t);
size_t u2wl (const wchar_t *, const wchar_t *, int);
Char *u2w (Char *, const wchar_t *, const wchar_t *, int);


lisp coerce_to_string (lisp, int);

lisp make_string (const char *);
lisp make_string_u (const wchar_t *);
lisp make_string (const u_char *);
lisp make_string (const char *, size_t);
lisp make_string_simple (const char *, size_t);
lisp make_string_w (const Char *, size_t);
lisp make_string_w (Char, size_t);
lisp make_complex_string (Char, int, int, int);
lisp make_string_from_list (lisp);
lisp make_string_from_vector (lisp);
lisp make_string (size_t);
lisp copy_string (lisp);

void string_start_end (lisp, int &, int &, lisp, lisp);
lisp subseq_string (lisp, lisp, lisp);

u_int hashpjw (const Char *, int);
u_int ihashpjw (const Char *, int);

inline u_int
hashpjw (lisp string)
{
  assert (stringp (string));
  return hashpjw (xstring_contents (string), xstring_length (string));
}

inline u_int
ihashpjw (lisp string)
{
  assert (stringp (string));
  return ihashpjw (xstring_contents (string), xstring_length (string));
}

inline u_int
hashpjw (lisp string, u_int prime)
{
  return hashpjw (string) % prime;
}

inline u_int
ihashpjw (lisp string, u_int prime)
{
  return ihashpjw (string) % prime;
}

inline size_t
w2sl (lisp l)
{
  return w2sl (xstring_contents (l), xstring_length (l));
}

inline char *
w2s (char *b, lisp l)
{
  return w2s (b, xstring_contents (l), xstring_length (l));
}

inline char *
w2s (lisp l)
{
  return w2s (xstring_contents (l), xstring_length (l));
}

inline char *
w2s (char *b, char *be, lisp l)
{
  return w2s (b, be, xstring_contents (l), xstring_length (l));
}

inline char *
w2s_quote (char *b, char *be, lisp l, int qc, int qe)
{
  return w2s_quote (b, be, xstring_contents (l), xstring_length (l), qc, qe);
}

inline ucs2_t *
i2w (lisp x, ucs2_t *b)
{
  return i2w (xstring_contents (x), xstring_length (x), b);
}

inline int
i2wl (lisp x)
{
  return i2wl (xstring_contents (x), xstring_length (x));
}

inline wchar_t *
w2u (wchar_t *b, const Char *s, size_t size)
{
  i2w (s, size, (ucs2_t*)b);
  return (wchar_t*)b;
}

inline size_t
w2ul (lisp l)
{
  return w2ul (xstring_contents (l), xstring_length (l));
}

inline wchar_t *
w2u (wchar_t *b, lisp l)
{
  return w2u (b, xstring_contents (l), xstring_length (l));
}

inline wchar_t *
w2u (lisp l)
{
  return w2u (xstring_contents (l), xstring_length (l));
}

inline wchar_t *
w2u (wchar_t *b, wchar_t *be, lisp l)
{
  return w2u (b, be, xstring_contents (l), xstring_length (l));
}

inline size_t
w2ul (const Char *s, size_t size)
{
  return i2wl(s, size);
}

class tmpstr {
private:
  char *ptr;
  void init(const wchar_t *wstr, size_t length) {
    if (!wstr) {
      ptr = 0;
      return;
    }
    if (*wstr == 0) {
      ptr = (char *)xmalloc(1);
      *ptr = 0;
      return;
    }
    size_t n = ::WideCharToMultiByte(CP_ACP, 0, wstr, length, NULL, NULL,
                                     NULL, NULL);
    ptr = (char *)xmalloc(n+1);
    ::WideCharToMultiByte(CP_ACP, 0, wstr, length, ptr, n,
                          NULL, NULL);
    ptr[n] = 0;
  };
public:
  tmpstr(const wchar_t *wstr) {
    init(wstr, -1);
  }
  tmpstr(const wchar_t *wstr, size_t length) {
    init(wstr, length);
  }
  ~tmpstr() {
    if (ptr)
      xfree(ptr);
  }
  const char* get() {
    return ptr;
  }
  operator const char* () {
    return get();
  }
};

class tmpwstr {
private:
  wchar_t *ptr;
  void init(const char *str, size_t length) {
    if (!str) {
      ptr = 0;
      return;
    }
    if (*str == 0) {
      ptr = (wchar_t *)xmalloc(sizeof(wchar_t));
      *ptr = 0;
      return;
    }
    size_t n = ::MultiByteToWideChar(CP_ACP, 0, str, length, NULL, NULL);
    ptr = (wchar_t *)xmalloc((n+1)*sizeof(wchar_t));
    ::MultiByteToWideChar(CP_ACP, 0, str, length, ptr, n);
    ptr[n] = 0;
  }
public:
  tmpwstr(const char *str) {
    init(str, -1);
  }
  tmpwstr(const char *str, size_t length) {
    init(str, length);
  }
  ~tmpwstr() {
    if (ptr)
      xfree(ptr);
  }
  const wchar_t* get() {
    return ptr;
  }
  operator const wchar_t* () {
    return get();
  }
};

#endif
