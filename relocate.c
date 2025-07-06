#include "prefix-stub/relocate.h"

#include <Windows.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static char *prefix;

static void set_prefix(void)
{
  size_t sz = MAX_PATH;
  char *str = malloc(sz);
  if (!str)
    return;

  SetLastError(ERROR_SUCCESS);
  GetModuleFileName(NULL, str, sz);

  while (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
  {
    char *old_ptr = str;
    sz *= 2;
    str = realloc(old_ptr, sz);
    if (!str)
    {
      free(old_ptr);
      return;
    }
    SetLastError(ERROR_SUCCESS);
    GetModuleFileName(NULL, str, sz);
  }

  char *pos = strrchr(str, '\\');
  if (!pos)
  {
    free(str);
    return;
  }
  *pos = '\0';
  pos = strrchr(str, '\\');
  if (!pos)
  {
    free(str);
    return;
  }
  *pos = '\0';

  prefix = str;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  switch (fdwReason)
  {
  case DLL_PROCESS_ATTACH:
    set_prefix();
    if (!prefix)
      return FALSE;
    break;

  case DLL_PROCESS_DETACH:
    if (lpvReserved != NULL)
    {
      break;
    }
    break;
  }
  return TRUE;
}

const char *prefix_stub_get_prefix() { return prefix; }

const char *prefix_stub_relocate(const char *sub_dir)
{
  size_t sub_dir_len = strlen(sub_dir);
  if (sub_dir_len == 0)
    return strdup(prefix);

  char *dir = malloc(strlen(prefix) + strlen(sub_dir) + 2);
  strcpy(dir, prefix);
  strcat(dir, "\\");
  strcat(dir, sub_dir);
  for (size_t i = 0; dir[i] != '\0'; ++i)
    if (dir[i] == '/')
      dir[i] = '\\';
  return dir;
}
