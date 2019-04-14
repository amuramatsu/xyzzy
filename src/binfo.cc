#include "stdafx.h"
#include "ed.h"
#include "binfo.h"
#include <shlwapi.h>

const wchar_t *const buffer_info::b_eol_name[] = {L"lf", L"crlf", L"cr"};

wchar_t *
buffer_info::modified (wchar_t *b, int pound) const
{
  if (!pound)
    {
      int c1 = '-', c2 = '-';
      if (b_bufp->b_modified)
        c1 = c2 = '*';
      if (b_bufp->read_only_p ())
        {
          c1 = '%';
          if (c2 == '-')
            c2 = c1;
        }
      if (b_bufp->b_truncated)
        c2 = '#';
      *b++ = (wchar_t)c1;
      *b++ = (wchar_t)c2;
    }
  else
    *b++ = b_bufp->b_modified ? L'*' : L' ';
  return b;
}

wchar_t *
buffer_info::read_only (wchar_t *b, int pound) const
{
  if (b_bufp->read_only_p ())
    *b++ = L'%';
  else if (!pound && b_bufp->b_truncated)
    *b++ = L'#';
  else
    *b++ = L' ';
  return b;
}

wchar_t *
buffer_info::buffer_name (wchar_t *b, wchar_t *be) const
{
  b = b_bufp->buffer_name (b, be);
  if (b == be - 1)
    *b++ = L' ';
  return b;
}

wchar_t *
buffer_info::file_name (wchar_t *b, wchar_t *be, int pound) const
{
  lisp name;
  if (stringp (name = b_bufp->lfile_name)
      || stringp (name = b_bufp->lalternate_file_name))
    {
      if (!pound)
        b = stpncpy (b, L"File: ", be - b);
      b = w2u (b, be, name);
      if (b == be - 1)
        *b++ = L' ';
    }
  return b;
}

wchar_t *
buffer_info::file_or_buffer_name (wchar_t *b, wchar_t *be, int pound) const
{
  wchar_t *bb = b;
  b = file_name (b, be, pound);
  if (b == bb)
    b = buffer_name (b, be);
  return b;
}

static wchar_t *
docopy (wchar_t *d, wchar_t *de, const wchar_t *s, int &f)
{
  *d++ = f ? L' ' : L':';
  f = 1;
  return stpncpy (d, s, de - d);
}

wchar_t *
buffer_info::minor_mode (lisp x, wchar_t *b, wchar_t *be, int &f) const
{
  for (int i = 0; i < 10; i++)
    if (consp (x) && symbolp (xcar (x))
        && symbol_value (xcar (x), b_bufp) != Qnil)
      {
        x = xcdr (x);
        if (symbolp (x))
          {
            x = symbol_value (x, b_bufp);
            if (!stringp (x))
              break;
          }
        if (stringp (x))
          {
            *b++ = f ? L' ' : L':';
            f = 1;
            return w2u (b, be, x);
          }
      }
    else
      break;
  return b;
}

wchar_t *
buffer_info::mode_name (wchar_t *b, wchar_t *be, int c) const
{
  int f = 0;
  lisp mode = symbol_value (Vmode_name, b_bufp);
  if (stringp (mode))
    b = w2u (b, be, mode);

  if (c == 'M')
    {
      if (b_bufp->b_narrow_depth)
        b = docopy (b, be, L"Narrow", f);
      if (Fkbd_macro_saving_p () != Qnil)
        b = docopy (b, be, L"Def", f);
      for (lisp al = xsymbol_value (Vminor_mode_alist);
           consp (al); al = xcdr (al))
        b = minor_mode (xcar (al), b, be, f);
    }

  if (processp (b_bufp->lprocess))
    switch (xprocess_status (b_bufp->lprocess))
      {
      case PS_RUN:
        b = stpncpy (b, L":Run", be - b);
        break;

      case PS_EXIT:
        b = stpncpy (b, L":Exit", be - b);
        break;
      }
  return b;
}

wchar_t *
buffer_info::ime_mode (wchar_t *b, wchar_t *be) const
{
  if (!b_ime)
    return b;
  *b_ime = 1;
  return stpncpy (b, (active_app_frame().ime_open_mode == kbd_queue::IME_MODE_ON
                      ? L"‚ " : L"--"),
                  be - b);
}

wchar_t *
buffer_info::position (wchar_t *b, wchar_t *be) const
{
  if (b_posp)
    *b_posp = b;
  else if (b_wp)
    {
      wchar_t tem[64];
      _swprintf (tem, L"%d:%d", b_wp->w_plinenum, b_wp->w_column);
      b = stpncpy (b, tem, be - b);
    }
  return b;
}

wchar_t *
buffer_info::version (wchar_t *b, wchar_t *be, int pound) const
{
  return stpncpy (b, tmpwstr(pound ? DisplayVersionString : VersionString), be - b);
}

wchar_t *
buffer_info::host_name (wchar_t *b, wchar_t *be, int pound) const
{
  if (*sysdep.host_name)
    {
      if (pound)
        *b++ = L'@';
      b = stpncpy (b, tmpwstr(sysdep.host_name), be - b);
    }
  return b;
}

wchar_t *
buffer_info::process_id (wchar_t *b, wchar_t *be) const
{
  wchar_t tem[64];
  wnsprintfW (tem, 64, L"%d", sysdep.process_id);
  tem[63] = 0;
  return stpncpy (b, tem, be - b);
}

wchar_t *
buffer_info::admin_user (wchar_t *b, wchar_t *be) const
{
  if (Fadmin_user_p () == Qt)
    {
      int f = 0;
      b = stpncpy (b, L"ŠÇ—ŽÒ: ", be - b);
    }
  return b;
}

wchar_t *
buffer_info::percent (wchar_t *b, wchar_t *be) const
{
  if (b_percentp)
    *b_percentp = b;
  else if (b_bufp && b_wp)
    {
      wchar_t tem[64];
      wnsprintfW(tem, 64, L"%d", mode_line_percent_painter::calc_percent(b_bufp, b_wp->w_point.p_point));
      tem[63] = 0;
      b = stpncpy (b, tem, be - b);
    }
  return b;
}

wchar_t *
buffer_info::frame_index (wchar_t *b, wchar_t *be, const ApplicationFrame* app1) const
{
  if (app1)
    {
      wchar_t tem[64];
      wnsprintfW (tem, 64, L"%d", app1->frame_index);
      tem[63] = 0;
      b = stpncpy (b, tem, be - b);
    }
  return b;
}

wchar_t *
buffer_info::format (lisp fmt, wchar_t *b, wchar_t *be) const
{
  if (b_posp)
    *b_posp = 0;
  if (b_ime)
    *b_ime = 0;
  if (b_percentp)
	*b_percentp = 0;

  const Char *p = xstring_contents (fmt);
  const Char *const pe = p + xstring_length (fmt);

  while (p < pe && b < be)
    {
      Char c = *p++;
      if (c != '%')
        {
        normal_char:
          if (DBCP (c))
            *b++ = c >> 8;
          *b++ = char (c);
        }
      else
        {
          if (p == pe)
            break;

          c = *p++;
          int pound = 0;
          if (c == '#')
            {
              pound = 1;
              if (p == pe)
                break;
              c = *p++;
            }

          switch (c)
            {
            default:
              goto normal_char;

            case '*':
              b = modified (b, pound);
              break;

            case 'r':
              b = read_only (b, pound);
              break;

            case 'p':
              b = progname (b, be);
              break;

            case 'v':
              b = version (b, be, pound);
              break;

            case 'h':
              b = host_name (b, be, pound);
              break;

            case 'b':
              b = buffer_name (b, be);
              break;

            case 'f':
              b = file_name (b, be, pound);
              break;

            case 'F':
              b = file_or_buffer_name (b, be, pound);
              break;

            case 'M':
            case 'm':
              b = mode_name (b, be, c);
              break;

            case 'k':
              b = encoding (b, be);
              break;

            case 'l':
              b = eol_code (b, be);
              break;

            case 'i':
              b = ime_mode (b, be);
              break;

            case 'P':
              b = position (b, be);
              break;

            case '/':
              b = percent (b, be);
              break;

            case '$':
              b = process_id (b, be);
              break;

            case 'x':
              b = frame_index (b, be, b_app);
              break;

            case '!':
              b = admin_user (b, be);
              break;
            }
        }
    }

  return b;
}
