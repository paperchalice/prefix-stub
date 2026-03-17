#ifndef PREFIX_STUB_RELOCATE_H
#define PREFIX_STUB_RELOCATE_H

#include <prefix-stub/export.h>

#ifdef __cplusplus
extern "C"
{
#endif

  PREFIX_STUB_EXPORT const char *prefix_stub_get_prefix(void);
  PREFIX_STUB_EXPORT const wchar_t *prefix_stub_get_prefix_w(void);

  PREFIX_STUB_EXPORT void prefix_stub_set_orig_prefix(const char *orig_prefix);
  PREFIX_STUB_EXPORT void
  prefix_stub_set_orig_prefix_w(const wchar_t *orig_prefix);

  PREFIX_STUB_EXPORT const char *prefix_stub_relocate(const char *dir);
  PREFIX_STUB_EXPORT const wchar_t *prefix_stub_relocate_w(const wchar_t *dir);

#ifdef __cplusplus
}
#endif

#ifdef _MSC_VER
#ifndef prefix_stub_EXPORTS
#pragma comment(lib, "prefix-stub.lib")
#endif
#endif

#endif // PREFIX_STUB_RELOCATE_H
