#ifndef TUTILS_H
#define TUTILS_H

#define ARRAYLEN_OF(a)	(sizeof (a) / sizeof((a)[0]))

void *xmalloc (size_t);
void *xrealloc (void *, size_t);
void xfree (void *);

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

#endif // TUTILS_H
