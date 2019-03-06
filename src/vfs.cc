#include "stdafx.h"
#include "ed.h"
#include "dyn-handle.h"
#include "vwin32.h"

class WINWOW64
{
public:
  enum file_path_mode {wow64, native};

protected:
  typedef BOOL (WINAPI *ISWOW64PROCESS)(HANDLE, PBOOL);
  typedef BOOL (WINAPI *WOW64DISABLEWOW64FSREDIRECTION)(PVOID *);
  typedef BOOL (WINAPI *WOW64REVERTWOW64FSREDIRECTION)(PVOID *);

  static const ISWOW64PROCESS fnIsWow64Process;
  static const WOW64DISABLEWOW64FSREDIRECTION fnWow64DisableWow64FsRedirection;
  static const WOW64REVERTWOW64FSREDIRECTION fnWow64RevertWow64FsRedirection;
  static BOOL b_wow64;

public:
  struct WoW64SpecialRedirectionPath
  {
    const wchar_t* name;
    const wchar_t* expStr;
    wchar_t path[MAX_PATH+1];
    WIN32_FIND_DATAW fd;
  };

  static BOOL WINAPI IsWow64Process (HANDLE hProcess, PBOOL Wow64Process);
  static BOOL WINAPI Wow64DisableWow64FsRedirection (PVOID *OldValue);
  static BOOL WINAPI Wow64RevertWow64FsRedirection (PVOID *OldValue);
  static BOOL IsWow64 ();
  static const WoW64SpecialRedirectionPath* GetSpecialRedirectionPathArray ();
  static BOOL IsSpecialRedirectionPath (const wchar_t *path, WIN32_FIND_DATAW &fd);
  static BOOL IsSpecialRedirectionFilename (const wchar_t *path);
  static file_path_mode GetFilePathMode ();
};

const WINWOW64::ISWOW64PROCESS WINWOW64::fnIsWow64Process =
  (WINWOW64::ISWOW64PROCESS)GetProcAddress (GetModuleHandle ("KERNEL32"),
                                            "IsWow64Process");

const WINWOW64::WOW64DISABLEWOW64FSREDIRECTION WINWOW64::fnWow64DisableWow64FsRedirection =
  (WINWOW64::WOW64DISABLEWOW64FSREDIRECTION)GetProcAddress (GetModuleHandle ("KERNEL32"),
                                            "Wow64DisableWow64FsRedirection");

const WINWOW64::WOW64REVERTWOW64FSREDIRECTION WINWOW64::fnWow64RevertWow64FsRedirection =
  (WINWOW64::WOW64REVERTWOW64FSREDIRECTION)GetProcAddress (GetModuleHandle ("KERNEL32"),
                                            "Wow64RevertWow64FsRedirection");

BOOL WINWOW64::b_wow64 = -1;

BOOL WINAPI
WINWOW64::IsWow64Process (HANDLE hProcess, PBOOL Wow64Process)
{
  BOOL r = FALSE;
  if (Wow64Process)
    {
      *Wow64Process = FALSE;
    }
  if (fnIsWow64Process)
    {
      r = fnIsWow64Process (hProcess, Wow64Process);
    }
  return r;
}

BOOL WINAPI
WINWOW64::Wow64DisableWow64FsRedirection (PVOID *OldValue)
{
  BOOL r = FALSE;
  if (OldValue)
    {
      *OldValue = NULL;
    }
  if (fnWow64DisableWow64FsRedirection)
    {
      r = fnWow64DisableWow64FsRedirection (OldValue);
    }
  return r;
}

BOOL WINAPI
WINWOW64::Wow64RevertWow64FsRedirection (PVOID *OldValue)
{
  BOOL r = FALSE;
  if (OldValue)
    {
      *OldValue = NULL;
    }
  if (fnWow64RevertWow64FsRedirection)
    {
      r = fnWow64RevertWow64FsRedirection (OldValue);
    }
  return r;
}

BOOL
WINWOW64::IsWow64 ()
{
  if (b_wow64 < 0)
    {
      IsWow64Process (GetCurrentProcess (), &b_wow64);
    }
  return b_wow64;
}

WINWOW64::file_path_mode
WINWOW64::GetFilePathMode ()
{
  file_path_mode m = wow64;
  if (Vwow64_enable_file_system_redirector && xsymbol_value (Vwow64_enable_file_system_redirector) == Qnil)
    {
      m = native;
    }
  return m;
}

const WINWOW64::WoW64SpecialRedirectionPath*
WINWOW64::GetSpecialRedirectionPathArray ()
{
  // See : http://msdn.microsoft.com/en-us/library/aa384187(v=vs.85).aspx
  static WoW64SpecialRedirectionPath srp[] =
  {
    { L"Sysnative",   L"%windir%\\Sysnative" },
    { L"catroot",     L"%windir%\\System32\\catroot" },
    { L"catroot2",    L"%windir%\\System32\\catroot2" },
    { L"driverstore", L"%windir%\\System32\\driverstore" },
    { L"etc",         L"%windir%\\System32\\drivers\\etc" },
    { L"logfiles",    L"%windir%\\System32\\logfiles" },
    { L"spool",       L"%windir%\\System32\\spool" },
    { NULL, NULL }
  };
  if(srp[0].path[0] == 0)
    {
      PVOID OldValue;
      WINWOW64::Wow64DisableWow64FsRedirection (&OldValue);
      WIN32_FIND_DATAW tfd = { 0 };
      {
        wchar_t path[MAX_PATH+1];
        ExpandEnvironmentStringsW (L"%windir%\\System32", path, _countof (path));
        HANDLE h = FindFirstFileW (path, &tfd);
        FindClose (h);
      }

      for(int i = 0; srp[i].name; i++)
        {
          WoW64SpecialRedirectionPath &s = srp[i];
          ExpandEnvironmentStringsW (s.expStr, s.path, _countof (s.path));
          s.fd = tfd;
          wcscpy (s.fd.cFileName, s.name);
        }
      WINWOW64::Wow64RevertWow64FsRedirection (&OldValue);
    }

  return srp;
}

BOOL
WINWOW64::IsSpecialRedirectionFilename (const wchar_t* filename)
{
  BOOL ret = FALSE;
  wchar_t fname[MAX_PATH+1];
  wcscpy (fname, filename);
  convert_backsl_with_sl (fname, L'/', L'\\');
  const WINWOW64::WoW64SpecialRedirectionPath* srp = WINWOW64::GetSpecialRedirectionPathArray ();
  for (int i = 0; srp[i].name; i++)
    {
      if (_memicmp (fname, srp[i].path, wcslen (srp[i].path)) == 0)
        {
          ret = TRUE;
          break;
        }
    }
  return ret;
}

BOOL
WINWOW64::IsSpecialRedirectionPath (const wchar_t *path, WIN32_FIND_DATAW &fd)
{
  BOOL r = FALSE;
  if (WINWOW64::IsWow64 ())
    {
      wchar_t t[MAX_PATH+1];
      wcscpy (t, path);
      for(wchar_t* p = t; *p != 0; p++)
        {
          if(*p == '/')
            {
              *p = '\\';
            }
        }

      const WoW64SpecialRedirectionPath* srp = GetSpecialRedirectionPathArray ();
      for(int i = 0; srp[i].name; i++)
        {
          const WoW64SpecialRedirectionPath &s = srp[i];
          if(strcasecmp (t, s.path) == 0)
            {
              fd = s.fd;
              r = TRUE;
              break;
            }
        }
    }
  return r;
}

class Wow64FsRedirectionSelector
{
  PVOID OldValue;
public:
  Wow64FsRedirectionSelector ();
  ~Wow64FsRedirectionSelector ();
};

Wow64FsRedirectionSelector::Wow64FsRedirectionSelector ()
     : OldValue (NULL)
{
  if (WINWOW64::IsWow64 () && WINWOW64::GetFilePathMode () == WINWOW64::native)
    {
      WINWOW64::Wow64DisableWow64FsRedirection (&OldValue);
    }
}

Wow64FsRedirectionSelector::~Wow64FsRedirectionSelector ()
{
  if (WINWOW64::IsWow64 () && WINWOW64::GetFilePathMode () == WINWOW64::native)
    {
      WINWOW64::Wow64RevertWow64FsRedirection (&OldValue);
    }
}

lisp
Fsi_wow64_reinterpret_path (lisp string, lisp flag)
{
  assert (stringp (string));
  lisp result = Qnil;
  if (WINWOW64::IsWow64 ())
    {
      const wchar_t* replaceFromExp = NULL;
      const wchar_t* replaceToExp = NULL;
      bool isNativePath = (flag == Qnil);

      wchar_t srcPath[MAX_PATH+1];
      w2u (srcPath, xstring_contents (string), xstring_length (string));

      if (isNativePath && WINWOW64::GetFilePathMode () == WINWOW64::wow64)
        {
		  if (! WINWOW64::IsSpecialRedirectionFilename (srcPath))
            {
              replaceFromExp = L"%windir%\\System32";
              replaceToExp = L"%windir%\\Sysnative";
            }
        }
      else if (!isNativePath && WINWOW64::GetFilePathMode () == WINWOW64::native)
        {
		  if (! WINWOW64::IsSpecialRedirectionFilename (srcPath))
            {
              replaceFromExp = L"%windir%\\System32";
              replaceToExp = L"%windir%\\SysWOW64";
            }
        }

      if (replaceFromExp && replaceToExp)
        {
          wchar_t replaceFrom[MAX_PATH+1];
          ExpandEnvironmentStringsW (replaceFromExp, replaceFrom, _countof (replaceFrom));
          size_t replaceFromLen = wcslen (replaceFrom);
          wchar_t sPath[MAX_PATH+1];
          wcscpy (sPath, srcPath);
          convert_backsl_with_sl (sPath, L'/', L'\\');
          if (_memicmp (sPath, replaceFrom, replaceFromLen) == 0) //XXX
            {
              wchar_t replaceTo[MAX_PATH+1];
              ExpandEnvironmentStringsW (replaceToExp, replaceTo, _countof (replaceTo));
              wchar_t replaceResult[MAX_PATH+1];
              wsprintfW (replaceResult, L"%s%s", replaceTo, &srcPath[replaceFromLen]);
              result = make_string_w (replaceResult);
            }
        }
    }
  if (result == Qnil)
    {
      result = copy_string(string);
    }
  return result;
}

class NetPassDlg
{
  HWND hwnd;
public:
  wchar_t username[256];
  wchar_t passwd[256];
  const wchar_t *remote;

private:
  static BOOL CALLBACK netpass_dlgproc (HWND, UINT, WPARAM, LPARAM);
  BOOL dlgproc (UINT, WPARAM, LPARAM);
  void do_command (int, int);
  void init_dialog ();

public:
  NetPassDlg (const wchar_t *);
  int do_modal ();
};

NetPassDlg::NetPassDlg (const wchar_t *r)
     : remote (r)
{
  *username = 0;
  *passwd = 0;
}

void
NetPassDlg::do_command (int id, int code)
{
  switch (id)
    {
    case IDOK:
      GetDlgItemTextW (hwnd, IDC_USERNAME, username, sizeof username);
      GetDlgItemTextW (hwnd, IDC_PASSWD, passwd, sizeof passwd);
      /* fall thru... */
    case IDCANCEL:
      EndDialog (hwnd, id);
      break;
    }
}

void
NetPassDlg::init_dialog ()
{
  center_window (hwnd);
  set_window_icon (hwnd);
  SetDlgItemTextW (hwnd, IDC_SHARE_NAME, remote);
}

BOOL
NetPassDlg::dlgproc (UINT msg, WPARAM wparam, LPARAM lparam)
{
  switch (msg)
    {
    case WM_INITDIALOG:
      init_dialog ();
      return 1;

    case WM_COMMAND:
      do_command (LOWORD (wparam), HIWORD (wparam));
      return 1;

    default:
      return 0;
    }
}

BOOL CALLBACK
NetPassDlg::netpass_dlgproc (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
  NetPassDlg *p;
  if (msg == WM_INITDIALOG)
    {
      p = (NetPassDlg *)lparam;
      SetWindowLong (hwnd, DWL_USER, lparam);
      p->hwnd = hwnd;
    }
  else
    {
      p = (NetPassDlg *)GetWindowLong (hwnd, DWL_USER);
      if (!p)
        return 0;
    }
  return p->dlgproc (msg, wparam, lparam);
}

int
NetPassDlg::do_modal ()
{
  return DialogBoxParam (active_app_frame().hinst, MAKEINTRESOURCE (IDD_NETPASSWD),
                         get_active_window (), netpass_dlgproc, LPARAM (this)) == IDOK;
}

#define WINFS_CALL1(TYPE, FAILED, PATH, FN) \
  WINFS_CALL (TYPE, FAILED, askpass (PATH), FN)
#define WINFS_CALL2(TYPE, FAILED, PATH1, PATH2, FN) \
  WINFS_CALL (TYPE, FAILED, askpass (PATH1, PATH2), FN)
#define WINFS_CALL(TYPE, FAILED, ASKPASS, FN) \
  { TYPE r = ::FN; \
    if (r == (FAILED) && ASKPASS) \
      r = ::FN; \
    return r; }

#define WINFS_MAPSL(PATH) \
  { char *__path = (char *)alloca (strlen (PATH) + 1); \
    strcpy (__path, (PATH)); \
    map_sl_to_backsl (__path); \
    (PATH) = __path; }
#define WINFS_MAPSL_W(PATH) \
  { wchar_t *__path = (wchar_t *)alloca ((wcslen (PATH) + 1)*sizeof(wchar_t)); \
    wcscpy (__path, (PATH)); \
    map_sl_to_backsl (__path); \
    (PATH) = __path; }

static const wchar_t *
skip_share (const wchar_t *path, int noshare_ok)
{
  const wchar_t *p = path;
  if ((*p != '/' && *p != '\\')
      || (p[1] != '/' && p[1] != '\\'))
    return 0;
  p = find_slash (p + 2);
  if (p)
    {
      const wchar_t *e = find_slash (p + 1);
      return e ? e : p + wcslen (p);
    }
  return noshare_ok ? path + wcslen (path) : 0;
}

static int
try_connect (wchar_t *remote, int e)
{
  NETRESOURCEW nr;
  nr.dwType = RESOURCETYPE_DISK;
  nr.lpLocalName = 0;
  nr.lpRemoteName = remote;
  nr.lpProvider = 0;

  if (e == ERROR_ACCESS_DENIED
      && WNetAddConnection2W (&nr, 0, 0, 0) == NO_ERROR)
    return 1;

  while (1)
    {
      NetPassDlg d (remote);
      if (!d.do_modal ())
        return 0;

      switch (WNetAddConnection2W (&nr, d.passwd, d.username, 0))
        {
        case NO_ERROR:
          return 1;

        case ERROR_INVALID_PASSWORD:
        case ERROR_LOGON_FAILURE:
        case ERROR_ACCESS_DENIED:
          break;

        default:
          return 0;
        }
    }
}

static int
askpass1 (const wchar_t *path, int noshare_ok)
{
  if (!path)
    return 0;

  int e = GetLastError ();
  switch (e)
    {
    default:
      return 0;

    case ERROR_ACCESS_DENIED:
    case ERROR_INVALID_PASSWORD:
    case ERROR_LOGON_FAILURE:
      break;
    }

  const wchar_t *root = skip_share (path, noshare_ok);
  if (!root)
    return 0;
  int l = root - path;
  wchar_t *remote = (wchar_t *)alloca ((l + 1)*sizeof(wchar_t));
  memcpy (remote, path, l*sizeof(wchar_t));
  remote[l] = 0;
  map_sl_to_backsl (remote);
  if (!strcasecmp (WINFS::wfs_share_cache, remote))
    return 0;
  if (try_connect (remote, e))
    {
      *WINFS::wfs_share_cache = 0;
      return 1;
    }
  wcscpy (WINFS::wfs_share_cache, remote);
  SetLastError (e);
  return 0;
}

static inline int
askpass1 (const char *path, int noshare_ok)
{
  wchar_t *p = make_tmpwstr(path);
  int s = askpass1 (p, noshare_ok);
  delete [] p;
  return s;
}

static inline int
askpass (const wchar_t *path)
{
  return askpass1 (path, 0);
}

static inline int
askpass (const char *path)
{
  return askpass1 (path, 0);
}

static inline int
askpass_noshare (const wchar_t *path)
{
  return askpass1 (path, 1);
}

static inline int
askpass_noshare (const char *path)
{
  return askpass1 (path, 1);
}

static inline int
askpass (const wchar_t *path1, const wchar_t *path2)
{
  return askpass1 (path1, 0) || askpass1 (path2, 0);
}

static inline int
askpass (const char *path1, const char *path2)
{
  return askpass1 (path1, 0) || askpass1 (path2, 0);
}

wchar_t WINFS::wfs_share_cache[MAX_PATH * 2];

const WINFS::GETDISKFREESPACEEX WINFS::GetDiskFreeSpaceEx =
  (WINFS::GETDISKFREESPACEEX)GetProcAddress (GetModuleHandle ("KERNEL32"),
                                             "GetDiskFreeSpaceExA");

BOOL WINAPI
WINFS::CreateDirectory (LPCWSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
  Wow64FsRedirectionSelector wow64FsRedirectionSelector;
  WINFS_CALL1 (BOOL, FALSE, lpPathName, CreateDirectoryW (lpPathName, lpSecurityAttributes));
}


BOOL WINAPI
WINFS::CreateDirectory (LPCSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
  Wow64FsRedirectionSelector wow64FsRedirectionSelector;
  WINFS_CALL1 (BOOL, FALSE, lpPathName, CreateDirectoryA (lpPathName, lpSecurityAttributes));
}

HANDLE WINAPI
WINFS::CreateFile (LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
                   LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition,
                   DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
  Wow64FsRedirectionSelector wow64FsRedirectionSelector;
  HANDLE r = ::CreateFileW (lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes,
                            dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
  if (r != INVALID_HANDLE_VALUE)
    return r;
  if (!sysdep.WinNTp () || !(dwFlagsAndAttributes & FILE_FLAG_BACKUP_SEMANTICS))
    {
      int e = GetLastError ();
      if (e == ERROR_ACCESS_DENIED)
        {
          DWORD a = ::GetFileAttributesW (lpFileName);
          SetLastError (e);
          if (a != -1 && a & FILE_ATTRIBUTE_DIRECTORY)
            return r;
        }
    }
  if (askpass (lpFileName))
    r = ::CreateFileW (lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes,
                       dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
  return r;
}

HANDLE WINAPI
WINFS::CreateFile (LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
                   LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition,
                   DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
  wchar_t *p = make_tmpwstr(lpFileName);
  HANDLE s = CreateFile (p, dwDesiredAccess, dwShareMode,
                         lpSecurityAttributes, dwCreationDisposition,
                         dwFlagsAndAttributes, hTemplateFile);
  delete [] p;
  return s;
}

BOOL WINAPI
WINFS::DeleteFile (LPCWSTR lpFileName)
{
  Wow64FsRedirectionSelector wow64FsRedirectionSelector;
  WINFS_CALL1 (BOOL, FALSE, lpFileName, DeleteFileW (lpFileName));
}

BOOL WINAPI
WINFS::DeleteFile (LPCSTR lpFileName)
{
  Wow64FsRedirectionSelector wow64FsRedirectionSelector;
  WINFS_CALL1 (BOOL, FALSE, lpFileName, DeleteFileA (lpFileName));
}

HANDLE WINAPI
WINFS::FindFirstFile (LPCWSTR lpFileName, LPWIN32_FIND_DATAW lpFindFileData)
{
  Wow64FsRedirectionSelector wow64FsRedirectionSelector;
  WINFS_CALL1 (HANDLE, INVALID_HANDLE_VALUE, lpFileName,
               FindFirstFileW (lpFileName, lpFindFileData));
}

HANDLE WINAPI
WINFS::FindFirstFile (LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData)
{
  Wow64FsRedirectionSelector wow64FsRedirectionSelector;
  WINFS_CALL1 (HANDLE, INVALID_HANDLE_VALUE, lpFileName,
               FindFirstFileA (lpFileName, lpFindFileData));
}

BOOL WINAPI
WINFS::FindNextFile (HANDLE hFindFile, LPWIN32_FIND_DATAW lpFindFileData)
{
  Wow64FsRedirectionSelector wow64FsRedirectionSelector;
  *lpFindFileData->cFileName = 0;
  return (::FindNextFileW (hFindFile, lpFindFileData)
          || (GetLastError () == ERROR_MORE_DATA
              && *lpFindFileData->cFileName));
}

BOOL WINAPI
WINFS::FindNextFile (HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData)
{
  Wow64FsRedirectionSelector wow64FsRedirectionSelector;
  *lpFindFileData->cFileName = 0;
  return (::FindNextFileA (hFindFile, lpFindFileData)
          || (GetLastError () == ERROR_MORE_DATA
              && *lpFindFileData->cFileName));
}

static BOOL WINAPI
GetDiskFreeSpaceFAT32 (LPCWSTR lpRootPathName, LPDWORD lpSectorsPerCluster,
                       LPDWORD lpBytesPerSector, LPDWORD lpNumberOfFreeClusters,
                       LPDWORD lpTotalNumberOfClusters)
{
  Wow64FsRedirectionSelector wow64FsRedirectionSelector;
  wchar_t buf[PATH_MAX + 1];
  if (!lpRootPathName)
    {
      if (!GetCurrentDirectoryW (sizeof buf, buf))
        return 0;
      lpRootPathName = root_path_name (buf, buf);
    }

  dyn_handle hvwin32 (CreateFileW (L"\\\\.\\vwin32", 0, 0, 0, 0,
                                   FILE_FLAG_DELETE_ON_CLOSE, 0));
  if (!hvwin32.valid ())
    return 0;

  ExtGetDskFreSpcStruc dfs = {0};
  DIOC_REGISTERS regs = {0};
  regs.reg_EAX = 0x7303;
  regs.reg_ECX = sizeof dfs;
  regs.reg_EDX = DWORD (lpRootPathName);
  regs.reg_EDI = DWORD (&dfs);

  DWORD nbytes;
  if (!DeviceIoControl (hvwin32, VWIN32_DIOC_DOS_DRIVEINFO,
                        &regs, sizeof regs, &regs, sizeof regs,
                        &nbytes, 0)
      || regs.reg_Flags & X86_CARRY_FLAG)
    return 0;

  *lpSectorsPerCluster = dfs.SectorsPerCluster;
  *lpBytesPerSector = dfs.BytesPerSector;
  *lpNumberOfFreeClusters = dfs.AvailableClusters;
  *lpTotalNumberOfClusters = dfs.TotalClusters;

  return 1;
}

BOOL WINAPI
WINFS::GetDiskFreeSpace (LPCWSTR lpRootPathName, LPDWORD lpSectorsPerCluster,
                         LPDWORD lpBytesPerSector, LPDWORD lpNumberOfFreeClusters,
                         LPDWORD lpTotalNumberOfClusters)
{
  Wow64FsRedirectionSelector wow64FsRedirectionSelector;
  BOOL r = ::GetDiskFreeSpaceW (lpRootPathName, lpSectorsPerCluster, lpBytesPerSector,
                                lpNumberOfFreeClusters, lpTotalNumberOfClusters);
  if (!r)
    {
      if (GetLastError () == ERROR_NOT_SUPPORTED)
        {
          *lpSectorsPerCluster = 1;
          *lpBytesPerSector = 4096;
        }
      else
        {
          if (!askpass (lpRootPathName))
            return 0;
          r = ::GetDiskFreeSpaceW (lpRootPathName, lpSectorsPerCluster, lpBytesPerSector,
                                   lpNumberOfFreeClusters, lpTotalNumberOfClusters);
          if (!r)
            return 0;
        }
    }

  if (GetDiskFreeSpaceEx)
    {
      if (!sysdep.WinNTp ()
          && GetDiskFreeSpaceFAT32 (lpRootPathName, lpSectorsPerCluster,
                                    lpBytesPerSector, lpNumberOfFreeClusters,
                                    lpTotalNumberOfClusters))
        return 1;

      uint64_t FreeBytesAvailableToCaller;
      uint64_t TotalNumberOfBytes;
      uint64_t TotalNumberOfFreeBytes;
      if (GetDiskFreeSpaceExW (lpRootPathName,
                               (PULARGE_INTEGER)&FreeBytesAvailableToCaller,
                               (PULARGE_INTEGER)&TotalNumberOfBytes,
                               (PULARGE_INTEGER)&TotalNumberOfFreeBytes))
        {
          DWORD blk = *lpSectorsPerCluster * *lpBytesPerSector;
          if (!blk)
            blk = 512;
          *lpTotalNumberOfClusters = DWORD (TotalNumberOfBytes / blk);
          *lpNumberOfFreeClusters = DWORD (TotalNumberOfFreeBytes / blk);
          r = 1;
        }
    }

  return r;
}

DWORD WINAPI
WINFS::internal_GetFileAttributes (LPCWSTR lpFileName)
{
  Wow64FsRedirectionSelector wow64FsRedirectionSelector;
  WINFS_CALL1 (DWORD, -1, lpFileName, GetFileAttributesW (lpFileName));
}

DWORD WINAPI
WINFS::GetFileAttributes (LPCWSTR lpFileName)
{
  Wow64FsRedirectionSelector wow64FsRedirectionSelector;
  DWORD attr = internal_GetFileAttributes (lpFileName);
  if (attr == DWORD (-1) && GetLastError () != ERROR_INVALID_NAME)
    {
      WIN32_FIND_DATAW fd;
      if (get_file_data (lpFileName, fd))
        attr = fd.dwFileAttributes;
    }
  return attr;
}

DWORD WINAPI
WINFS::GetFileAttributes (LPCSTR lpFileName)
{
  wchar_t *p = make_tmpwstr(lpFileName);
  DWORD s = GetFileAttributes(p);
  delete [] p;
  return s;
}

UINT WINAPI
WINFS::GetTempFileName (LPCWSTR lpPathName, LPCWSTR lpPrefixString, UINT uUnique, LPWSTR lpTempFileName)
{
  Wow64FsRedirectionSelector wow64FsRedirectionSelector;
  WINFS_CALL1 (UINT, 0, lpPathName,
               GetTempFileNameW (lpPathName, lpPrefixString, uUnique, lpTempFileName));
}

UINT WINAPI
WINFS::GetTempFileName (LPCSTR lpPathName, LPCSTR lpPrefixString, UINT uUnique, LPSTR lpTempFileName)
{
  Wow64FsRedirectionSelector wow64FsRedirectionSelector;
  WINFS_CALL1 (UINT, 0, lpPathName,
               GetTempFileNameA (lpPathName, lpPrefixString, uUnique, lpTempFileName));
}

BOOL WINAPI
WINFS::GetVolumeInformation (LPCWSTR lpRootPathName, LPWSTR lpVolumeNameBuffer,
                             DWORD nVolumeNameSize, LPDWORD lpVolumeSerialNumber,
                             LPDWORD lpMaximumComponentLength, LPDWORD lpFileSystemFlags,
                             LPWSTR lpFileSystemNameBuffer, DWORD nFileSystemNameSize)
{
  Wow64FsRedirectionSelector wow64FsRedirectionSelector;
  WINFS_CALL1 (BOOL, FALSE, lpRootPathName,
               GetVolumeInformationW (lpRootPathName, lpVolumeNameBuffer, nVolumeNameSize,
                                      lpVolumeSerialNumber, lpMaximumComponentLength,
                                      lpFileSystemFlags, lpFileSystemNameBuffer, nFileSystemNameSize));
}

BOOL WINAPI
WINFS::GetVolumeInformation (LPCSTR lpRootPathName, LPSTR lpVolumeNameBuffer,
                             DWORD nVolumeNameSize, LPDWORD lpVolumeSerialNumber,
                             LPDWORD lpMaximumComponentLength, LPDWORD lpFileSystemFlags,
                             LPSTR lpFileSystemNameBuffer, DWORD nFileSystemNameSize)
{
  Wow64FsRedirectionSelector wow64FsRedirectionSelector;
  WINFS_CALL1 (BOOL, FALSE, lpRootPathName,
               GetVolumeInformationA (lpRootPathName, lpVolumeNameBuffer, nVolumeNameSize,
                                      lpVolumeSerialNumber, lpMaximumComponentLength,
                                      lpFileSystemFlags, lpFileSystemNameBuffer, nFileSystemNameSize));
}

HMODULE WINAPI
WINFS::LoadLibrary (LPCWSTR lpLibFileName)
{
  WINFS_CALL1 (HMODULE, NULL, lpLibFileName, LoadLibraryW (lpLibFileName));
}

HMODULE WINAPI
WINFS::LoadLibrary (LPCSTR lpLibFileName)
{
  wchar_t *p = make_tmpwstr(lpLibFileName);
  HMODULE s = LoadLibrary(p);
  delete [] p;
  return s;
}

static BOOL
move_file (LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName)
{
  Wow64FsRedirectionSelector wow64FsRedirectionSelector;
  WINFS_CALL2 (BOOL, FALSE, lpExistingFileName, lpNewFileName,
               MoveFileW (lpExistingFileName, lpNewFileName));
}

BOOL WINAPI
WINFS::MoveFile (LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName)
{
  for (int retry = 0;; retry++)
    {
      if (move_file (lpExistingFileName, lpNewFileName))
        return 1;
      if (retry >= 3)
        return 0;
      Sleep (50);
    }
}

static BOOL
move_file (LPCSTR lpExistingFileName, LPCSTR lpNewFileName)
{
  Wow64FsRedirectionSelector wow64FsRedirectionSelector;
  WINFS_CALL2 (BOOL, FALSE, lpExistingFileName, lpNewFileName,
               MoveFileA (lpExistingFileName, lpNewFileName));
}

BOOL WINAPI
WINFS::MoveFile (LPCSTR lpExistingFileName, LPCSTR lpNewFileName)
{
  for (int retry = 0;; retry++)
    {
      if (move_file (lpExistingFileName, lpNewFileName))
        return 1;
      if (retry >= 3)
        return 0;
      Sleep (50);
    }
}

BOOL WINAPI
WINFS::RemoveDirectory (LPCWSTR lpPathName)
{
  Wow64FsRedirectionSelector wow64FsRedirectionSelector;
  WINFS_CALL1 (BOOL, FALSE, lpPathName, RemoveDirectoryW (lpPathName));
}

BOOL WINAPI
WINFS::RemoveDirectory (LPCSTR lpPathName)
{
  Wow64FsRedirectionSelector wow64FsRedirectionSelector;
  WINFS_CALL1 (BOOL, FALSE, lpPathName, RemoveDirectoryA (lpPathName));
}

BOOL WINAPI
WINFS::SetFileAttributes (LPCWSTR lpFileName, DWORD dwFileAttributes)
{
  Wow64FsRedirectionSelector wow64FsRedirectionSelector;
  WINFS_CALL1 (BOOL, FALSE, lpFileName,
               SetFileAttributesW (lpFileName, dwFileAttributes));
}

BOOL WINAPI
WINFS::SetFileAttributes (LPCSTR lpFileName, DWORD dwFileAttributes)
{
  Wow64FsRedirectionSelector wow64FsRedirectionSelector;
  WINFS_CALL1 (BOOL, FALSE, lpFileName,
               SetFileAttributesA (lpFileName, dwFileAttributes));
}

DWORD WINAPI
WINFS::internal_GetFullPathName (LPCWSTR lpFileName, DWORD nBufferLength,
                                 LPWSTR lpBuffer, LPWSTR *lpFilePart)
{
  Wow64FsRedirectionSelector wow64FsRedirectionSelector;
  WINFS_MAPSL_W (lpFileName);
  WINFS_CALL1 (DWORD, 0, lpFileName,
               GetFullPathNameW (lpFileName, nBufferLength, lpBuffer, lpFilePart));
}

DWORD WINAPI
WINFS::internal_GetFullPathName (LPCSTR lpFileName, DWORD nBufferLength,
                                 LPSTR lpBuffer, LPSTR *lpFilePart)
{
  Wow64FsRedirectionSelector wow64FsRedirectionSelector;
  WINFS_MAPSL (lpFileName);
  WINFS_CALL1 (DWORD, 0, lpFileName,
               GetFullPathNameA (lpFileName, nBufferLength, lpBuffer, lpFilePart));
}

BOOL WINAPI
WINFS::SetCurrentDirectory (LPCWSTR lpPathName)
{
  Wow64FsRedirectionSelector wow64FsRedirectionSelector;
  WINFS_MAPSL_W (lpPathName);
  WINFS_CALL1 (BOOL, FALSE, lpPathName, SetCurrentDirectoryW (lpPathName));
}

BOOL WINAPI
WINFS::SetCurrentDirectory (LPCSTR lpPathName)
{
  Wow64FsRedirectionSelector wow64FsRedirectionSelector;
  WINFS_MAPSL (lpPathName);
  WINFS_CALL1 (BOOL, FALSE, lpPathName, SetCurrentDirectoryA (lpPathName));
}

DWORD WINAPI
WINFS::GetFullPathName (LPCWSTR path, DWORD size, LPWSTR buf, LPWSTR *name)
{
  Wow64FsRedirectionSelector wow64FsRedirectionSelector;
  DWORD l = internal_GetFullPathName (path, size, buf, name);
  if (!l || l >= size)
    return l;
  if (!dir_separator_p (*path) || !dir_separator_p (path[1]))
    return l;
  if (alpha_char_p (*buf & 0xff) && buf[1] == L':'
      && dir_separator_p (buf[2]) && dir_separator_p (buf[3]))
    {
      wcscpy (buf, buf + 2);
      l -= 2;
      if (name && *name >= buf + 2)
        *name -= 2;
    }
  return l;
}

DWORD WINAPI
WINFS::GetFullPathName (LPCSTR path, DWORD size, LPSTR buf, LPSTR *name)
{
  Wow64FsRedirectionSelector wow64FsRedirectionSelector;
  DWORD l = internal_GetFullPathName (path, size, buf, name);
  if (!l || l >= size)
    return l;
  if (!dir_separator_p (*path) || !dir_separator_p (path[1]))
    return l;
  if (alpha_char_p (*buf & 0xff) && buf[1] == ':'
      && dir_separator_p (buf[2]) && dir_separator_p (buf[3]))
    {
      strcpy (buf, buf + 2);
      l -= 2;
      if (name && *name >= buf + 2)
        *name -= 2;
    }
  return l;
}

DWORD WINAPI
WINFS::WNetOpenEnum (DWORD dwScope, DWORD dwType, DWORD dwUsage,
                     LPNETRESOURCEW lpNetResource, LPHANDLE lphEnum)
{
  if (!lpNetResource)
    return ::WNetOpenEnumW (dwScope, dwType, dwUsage, lpNetResource, lphEnum);

  DWORD r = ::WNetOpenEnumW (dwScope, dwType, dwUsage, lpNetResource, lphEnum);
  if (r != NO_ERROR && askpass_noshare (lpNetResource->lpRemoteName))
    r = ::WNetOpenEnumW (dwScope, dwType, dwUsage, lpNetResource, lphEnum);
  return r;
}

int WINAPI
WINFS::get_file_data (const wchar_t *path, WIN32_FIND_DATAW &fd)
{
  int r = 0;
  if (WINWOW64::IsSpecialRedirectionPath (path, fd))
    {
      r = 1;
    }
  else
    {
      HANDLE h = FindFirstFileW (path, &fd);
      if (h != INVALID_HANDLE_VALUE)
        {
          r = 1;
          FindClose(h);
        }
    }
  return r;
}
