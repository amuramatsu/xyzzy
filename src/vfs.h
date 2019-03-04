#ifndef _vfs_h_
#define _vfs_h_

class WINFS
{
protected:
  typedef BOOL (WINAPI *GETDISKFREESPACEEX)(LPCTSTR, PULARGE_INTEGER,
                                            PULARGE_INTEGER, PULARGE_INTEGER);
  static const GETDISKFREESPACEEX GetDiskFreeSpaceEx;

  static DWORD WINAPI internal_GetFullPathName (LPCWSTR lpFileName, DWORD nBufferLength,
                                                LPWSTR lpBuffer, LPWSTR *lpFilePart);
  static DWORD WINAPI internal_GetFileAttributes (LPCWSTR lpFileName);
public:
  static wchar_t wfs_share_cache[MAX_PATH * 2];

  static void clear_share_cache () {*wfs_share_cache = 0;}

  static BOOL WINAPI CreateDirectory (LPCSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);
  static BOOL WINAPI CreateDirectory (LPCWSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);
  static HANDLE WINAPI CreateFile (LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
                                   LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition,
                                   DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
  static HANDLE WINAPI CreateFile (LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
                                   LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition,
                                   DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
  static BOOL WINAPI DeleteFile (LPCSTR lpFileName);
  static BOOL WINAPI DeleteFile (LPCWSTR lpFileName);
  static HANDLE WINAPI FindFirstFile (LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData);
  static HANDLE WINAPI FindFirstFile (LPCWSTR lpFileName, LPWIN32_FIND_DATAW lpFindFileData);
  static BOOL WINAPI FindNextFile (HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData);
  static BOOL WINAPI FindNextFile (HANDLE hFindFile, LPWIN32_FIND_DATAW lpFindFileData);
  static BOOL WINAPI GetDiskFreeSpace (LPCSTR lpRootPathName, LPDWORD lpSectorsPerCluster,
                                       LPDWORD lpBytesPerSector, LPDWORD lpNumberOfFreeClusters,
                                       LPDWORD lpTotalNumberOfClusters);
  static BOOL WINAPI GetDiskFreeSpace (LPCWSTR lpRootPathName, LPDWORD lpSectorsPerCluster,
                                       LPDWORD lpBytesPerSector, LPDWORD lpNumberOfFreeClusters,
                                       LPDWORD lpTotalNumberOfClusters);
  static DWORD WINAPI GetFileAttributes (LPCSTR lpFileName);
  static DWORD WINAPI GetFileAttributes (LPCWSTR lpFileName);
  static DWORD WINAPI GetFullPathName (LPCSTR lpFileName, DWORD nBufferLength, LPSTR lpBuffer, LPSTR *lpFilePart);
  static DWORD WINAPI GetFullPathName (LPCWSTR lpFileName, DWORD nBufferLength, LPWSTR lpBuffer, LPWSTR *lpFilePart);
  static UINT WINAPI GetTempFileName (LPCSTR lpPathName, LPCSTR lpPrefixString, UINT uUnique, LPSTR lpTempFileName);
  static UINT WINAPI GetTempFileName (LPCWSTR lpPathName, LPCWSTR lpPrefixString, UINT uUnique, LPWSTR lpTempFileName);
  static BOOL WINAPI GetVolumeInformation (LPCSTR lpRootPathName, LPSTR lpVolumeNameBuffer,
                                           DWORD nVolumeNameSize, LPDWORD lpVolumeSerialNumber,
                                           LPDWORD lpMaximumComponentLength, LPDWORD lpFileSystemFlags,
                                           LPSTR lpFileSystemNameBuffer, DWORD nFileSystemNameSize);
  static BOOL WINAPI GetVolumeInformation (LPCWSTR lpRootPathName, LPWSTR lpVolumeNameBuffer,
                                           DWORD nVolumeNameSize, LPDWORD lpVolumeSerialNumber,
                                           LPDWORD lpMaximumComponentLength, LPDWORD lpFileSystemFlags,
                                           LPWSTR lpFileSystemNameBuffer, DWORD nFileSystemNameSize);
  static HMODULE WINAPI LoadLibrary (LPCSTR lpLibFileName);
  static HMODULE WINAPI LoadLibrary (LPCWSTR lpLibFileName);
  static BOOL WINAPI MoveFile (LPCSTR lpExistingFileName, LPCSTR lpNewFileName);
  static BOOL WINAPI MoveFile (LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName);
  static BOOL WINAPI RemoveDirectory (LPCSTR lpPathName);
  static BOOL WINAPI RemoveDirectory (LPCWSTR lpPathName);
  static BOOL WINAPI SetCurrentDirectory (LPCSTR lpPathName);
  static BOOL WINAPI SetCurrentDirectory (LPCWSTR lpPathName);
  static BOOL WINAPI SetFileAttributes (LPCSTR lpFileName, DWORD dwFileAttributes);
  static BOOL WINAPI SetFileAttributes (LPCWSTR lpFileName, DWORD dwFileAttributes);
  static DWORD WINAPI WNetOpenEnum (DWORD dwScope, DWORD dwType, DWORD dwUsage,
                                    LPNETRESOURCEW lpNetResource, LPHANDLE lphEnum);

  static int WINAPI get_file_data (const char *, WIN32_FIND_DATAA &);
  static int WINAPI get_file_data (const wchar_t *, WIN32_FIND_DATAW &);
};

#endif /* _vfs_h_ */
