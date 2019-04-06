#ifndef _binfo_h_
#define _binfo_h_

#include "version.h"

class buffer_info
{
  const Window *const b_wp;
  const Buffer *const b_bufp;
  const ApplicationFrame *const b_app;
  wchar_t **const b_posp;
  wchar_t **const b_percentp;
  int *const b_ime;
  static const wchar_t *const b_eol_name[];

  wchar_t *minor_mode (lisp, wchar_t *, wchar_t *, int &) const;
public:
  buffer_info (const ApplicationFrame *app, const Window *wp, const Buffer *bp, wchar_t **posp, int *ime, wchar_t **percentp)
       : b_wp (wp), b_bufp (bp), b_app(app), b_posp (posp), b_ime (ime), b_percentp(percentp) {}
  wchar_t *format (lisp, wchar_t *, wchar_t *) const;
  wchar_t *modified (wchar_t *, int) const;
  wchar_t *read_only (wchar_t *, int) const;
  wchar_t *progname (wchar_t *b, wchar_t *be) const
    {return stpncpy (b, tmpwstr(ProgramName), be - b);}
  wchar_t *version (wchar_t *, wchar_t *, int) const;
  wchar_t *buffer_name (wchar_t *, wchar_t *) const;
  wchar_t *file_name (wchar_t *, wchar_t *, int) const;
  wchar_t *file_or_buffer_name (wchar_t *, wchar_t *, int) const;
  wchar_t *mode_name (wchar_t *, wchar_t *, int) const;
  wchar_t *encoding (wchar_t *b, wchar_t *be) const
    {return w2u (b, be, xchar_encoding_name (b_bufp->lchar_encoding));}
  wchar_t *eol_code (wchar_t *b, wchar_t *be) const
    {return stpncpy (b, b_eol_name[b_bufp->b_eol_code], be - b);}
  wchar_t *ime_mode (wchar_t *, wchar_t *) const;
  wchar_t *position (wchar_t *, wchar_t *) const;
  wchar_t *host_name (wchar_t *, wchar_t *, int) const;
  wchar_t *process_id (wchar_t *, wchar_t *) const;
  wchar_t *admin_user (wchar_t *, wchar_t *) const;
  wchar_t *percent(wchar_t *, wchar_t *) const;
  wchar_t *frame_index(wchar_t *, wchar_t *, const ApplicationFrame* app1) const;
};

#endif
