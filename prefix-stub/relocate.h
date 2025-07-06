#ifndef PREFIX_STUB_RELOCATE_H
#define PREFIX_STUB_RELOCATE_H

#include <prefix-stub/export.h>

#ifdef __cplusplus
extern "C"
{
#endif

  PREFIX_STUB_EXPORT const char *prefix_stub_get_prefix(void);

  PREFIX_STUB_EXPORT const char *prefix_stub_relocate(const char *sub_dir);

#ifdef __cplusplus
}
#endif

#endif // PREFIX_STUB_RELOCATE_H
