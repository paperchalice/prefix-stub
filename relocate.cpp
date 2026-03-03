#include "prefix-stub/relocate.h"

#include <Windows.h>

import std;

namespace fs = ::std::filesystem;

namespace
{

struct paths
{
  fs::path pth = {};
  std::string str = {};
  template <typename T> paths(T s) : pth{s}, str{pth.string()} {}
  paths(fs::path &&p) : pth{p.lexically_normal()}, str{pth.string()} {}
  paths() = default;
  bool empty() { return pth.empty(); }
  const char *c_str() const { return str.c_str(); }
  const wchar_t *c_wstr() const { return pth.c_str(); }
  template <typename T> const T *get_pchar() const
  {
    if constexpr (std::same_as<T, char>)
      return c_str();
    else
      return c_wstr();
  }
};

paths orig_prefix;    // without trailing slash
paths current_prefix; // without trailing slash

} // namespace

static std::unordered_map<fs::path, paths> path_map;

static constexpr std::size_t init_path_len = 256;

static void set_prefix(void)
{
  std::wstring prefix_str(init_path_len, L'\0');
  GetModuleFileNameW(nullptr, prefix_str.data(), prefix_str.capacity());
  DWORD last_err = GetLastError();
  while (last_err == ERROR_INSUFFICIENT_BUFFER)
  {
    prefix_str.reserve(prefix_str.capacity() * 2);
    GetModuleFileNameW(nullptr, prefix_str.data(), prefix_str.capacity());
  }
  current_prefix = paths(fs::path(prefix_str).parent_path().parent_path());
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  switch (fdwReason)
  {
  case DLL_PROCESS_ATTACH:
    set_prefix();
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

void ::prefix_stub_set_orig_prefix(const char *orig_prefix)
{
  ::orig_prefix = fs::path(orig_prefix).lexically_normal();
}

void ::prefix_stub_set_orig_prefix_w(const wchar_t *orig_prefix)
{
  ::orig_prefix = fs::path(orig_prefix).lexically_normal();
}

const char * ::prefix_stub_get_prefix() { return current_prefix.c_str(); }

const wchar_t * ::prefix_stub_get_prefix_w() { return current_prefix.c_wstr(); }

template <typename T> static const T *relocate_impl(const T *dir)
{
  fs::path pth = fs::path(dir).lexically_normal();
  if (pth == orig_prefix.pth)
    return orig_prefix.get_pchar<T>();

  std::wstring_view orign_prefix_view = orig_prefix.c_wstr();
  std::wstring_view pth_view = pth.c_str();

  if (pth_view.size() < orign_prefix_view.size())
    return dir;

  if (!pth_view.starts_with(orign_prefix_view))
    return dir;

  pth_view.remove_prefix(orign_prefix_view.size());
  if (!pth_view.starts_with(fs::path::preferred_separator))
    return dir;
  pth_view.remove_prefix(1);
  paths &result = path_map[pth];
  if (!result.empty())
    return result.get_pchar<T>();
  result = paths(current_prefix.pth / pth_view);
  return result.get_pchar<T>();
}

const char * ::prefix_stub_relocate(const char *dir)
{
  return relocate_impl(dir);
}

const wchar_t * ::prefix_stub_relocate_w(const wchar_t *dir)
{
  return relocate_impl(dir);
}
