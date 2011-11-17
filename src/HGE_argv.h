#ifndef _HGE_ARG_H_
#define _HGE_ARG_H_

#include <stdarg.h>
#include "SDL.h"
#include "HGE_basetypes.h"
#include "HGE_types.h"
#include "HGE_union.h"         

enum
  {
    HGE_INVALID_OFFSET = -2,
    HGE_INCORRECT_TYPE = -1,
  };

enum
  {
    HGE_ARG_TYPE_NULL,
    HGE_ARG_TYPE_Uint8,
    HGE_ARG_TYPE_Sint8,
    HGE_ARG_TYPE_Uint16,
    HGE_ARG_TYPE_Sint16,
    HGE_ARG_TYPE_Uint32,
    HGE_ARG_TYPE_Sint32,
    HGE_ARG_TYPE_Uint64,
    HGE_ARG_TYPE_Sint64,
    HGE_ARG_TYPE_float,
    HGE_ARG_TYPE_double,
    HGE_ARG_TYPE_voidp,
    HGE_ARG_TYPE_voidpf,
    HGE_ARG_TYPE_hgerc,
    HGE_ARG_TYPE_INVALID
  };

typedef struct HGE_arg_t HGE_arg_t;
struct HGE_arg_t
{
  Uint8 type;
  HGE_union_t u;
};

typedef struct HGE_argv_t HGE_argv_t;
struct HGE_argv_t
{
  unsigned int count;
  HGE_arg_t*   args;
};

extern HGE_typeinfo_t HGE_argv[];

HGE_error_t  HGE_argv_init(void* ptr);
void         HGE_argv_clean(void* ptr);
void*        HGE_argv_alloc(void);
void         HGE_argv_free(void* ptr);
HGE_argv_t*  HGE_argv_set_from_va_list(HGE_argv_t* argv, const char* str, va_list ap);
HGE_argv_t*  HGE_argv_set_from_string(HGE_argv_t* argv, const char* str, ...);
void         HGE_argv_set_length(HGE_argv_t* argv, unsigned int len);
unsigned int HGE_argv_get_length(HGE_argv_t* argv);
Uint8        HGE_argv_get_type(HGE_argv_t* argv, int offset);
void         HGE_argv_set(HGE_argv_t* argv, int offset, Uint8 type, void* data);
int          HGE_argv_get(HGE_argv_t* argv, int offset, Uint8 type, void* data);
void         HGE_argv_set_uint8(HGE_argv_t* argv, int offset, Uint8 u8);
void         HGE_argv_set_sint8(HGE_argv_t* argv, int offset, Sint8 s8);
void         HGE_argv_set_uint16(HGE_argv_t* argv, int offset, Uint16 u16);
void         HGE_argv_set_sint16(HGE_argv_t* argv, int offset, Sint16 s16);
void         HGE_argv_set_uint32(HGE_argv_t* argv, int offset, Uint32 u32);
void         HGE_argv_set_sint32(HGE_argv_t* argv, int offset, Sint32 s32);
void         HGE_argv_set_uint64(HGE_argv_t* argv, int offset, Uint64 u64);
void         HGE_argv_set_sint64(HGE_argv_t* argv, int offset, Sint64 s64);
void         HGE_argv_set_float(HGE_argv_t* argv, int offset, float f);
void         HGE_argv_set_double(HGE_argv_t* argv, int offset, double d);
void         HGE_argv_set_voidp(HGE_argv_t* argv, int offset, void* p);
void         HGE_argv_set_voidpf(HGE_argv_t* argv, int offset, void* p);
void         HGE_argv_set_hgerc(HGE_argv_t* argv, int offset, void* p);
int          HGE_argv_get_uint8(HGE_argv_t* argv, int offset, Uint8* u8);
int          HGE_argv_get_sint8(HGE_argv_t* argv, int offset, Sint8* s8);
int          HGE_argv_get_uint16(HGE_argv_t* argv, int offset, Uint16* u16);
int          HGE_argv_get_sint16(HGE_argv_t* argv, int offset, Sint16* s16);
int          HGE_argv_get_uint32(HGE_argv_t* argv, int offset, Uint32* u32);
int          HGE_argv_get_sint32(HGE_argv_t* argv, int offset, Sint32* s32);
int          HGE_argv_get_float(HGE_argv_t* argv, int offset, float* f);
int          HGE_argv_get_double(HGE_argv_t* argv, int offset, double* d);
int          HGE_argv_get_voidp(HGE_argv_t* argv, int offset, void** p);
int          HGE_argv_get_hgerc(HGE_argv_t* argv, int offset, void** p);

#endif /* _HGE_ARG_H_ */
