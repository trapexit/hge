#include <string.h>
#include <stdarg.h>
#include "SDL.h"
#include "HGE_types.h"
#include "HGE_mem.h"
#include "HGE_argv.h"

HGE_typeinfo_t HGE_argv[] = {{sizeof(HGE_argv_t),
			      HGE_argv_init,
			      HGE_argv_clean,
			      NULL,
			      NULL}};

HGE_error_t
HGE_argv_init(void* ptr)
{
  HGE_argv_t* argv = ptr;

  argv->count = 0;
  argv->args  = NULL;

  return HGE_SUCCESS;
}

void
HGE_argv_clean(void* ptr)
{
  int i;
  HGE_argv_t* argv = ptr;

  if(argv != NULL)
    {
      for(i = 0; i < argv->count; i++)
	{
	  switch(argv->args[i].type)
	    {
	    case HGE_ARG_TYPE_voidpf:
	      HGE_free(argv->args[i].u.p);
	      break;
	    case HGE_ARG_TYPE_hgerc:
	      HGE_unref(argv->args[i].u.p);
	      break;
	    default:
	      break;
	    }
	}

      HGE_free(argv->args);
    }
}

void*
HGE_argv_alloc(void)
{
  void* ptr;

  if((ptr = HGE_alloc(HGE_argv_t)) != NULL)
    {
      if(HGE_argv_init(ptr) != HGE_SUCCESS)
        HGE_free_and_null(ptr);
    }

  return ptr;
}

void
HGE_argv_free(void* ptr)
{
  HGE_argv_clean(ptr);
  HGE_free(ptr);
}

void
HGE_argv_set_length(HGE_argv_t* argv, unsigned int len)
{
  int i;

  if(len != argv->count)
    argv->args = HGE_realloc(argv->args, HGE_arg_t, len);

  for(i = argv->count; i < len; i++)
    argv->args[i].type = HGE_ARG_TYPE_NULL;

  argv->count = len;
}

unsigned int
HGE_argv_get_length(HGE_argv_t* argv)
{
  return argv->count;
}

Uint8
HGE_argv_get_type(HGE_argv_t* argv, int offset)
{
  if(offset > argv->count)
    return HGE_ARG_TYPE_INVALID;
  else
    return argv->args[offset].type;
}

/*
  b  = Uint8
  B  = Sint8
  s  = Uint16
  S  = Sint16
  i  = Uint32
  I  = Sint32
  l  = Uint64
  L  = Sint64
  f  = float
  d  = double
  p  = normal void*
  P  = void* that will be freed on HGE_argv_t free
  h  = void* that was allocated using HGE_new, HGE_unref'ed at HGE_argv_t free
  X  = skips an offset
*/
HGE_argv_t*
HGE_argv_set_from_string(HGE_argv_t* argv, const char* str, ...)
{
  va_list ap;
  HGE_argv_t* rv;

  va_start(ap, str);
  rv = HGE_argv_set_from_va_list(argv, str, ap);
  va_end(ap);

  return rv;
}

HGE_argv_t*
HGE_argv_set_from_va_list(HGE_argv_t* argv, const char* str, va_list ap)
{
  unsigned int offset = 0;

  if(argv == NULL)
    {
      argv = HGE_argv_alloc();
      HGE_argv_set_length(argv, strlen(str));
    }

  while(*str)
    {
      switch(*str++)
	{
	case 'b':
	  HGE_argv_set_uint8(argv, offset++, (Uint8)va_arg(ap, unsigned int));
	  break;
	case 's':
	  HGE_argv_set_uint16(argv, offset++, (Uint16)va_arg(ap, unsigned int));
	  break;
	case 'i':
	  HGE_argv_set_uint32(argv, offset++, (Uint32)va_arg(ap, Uint32));
	  break;
	case 'l':
	  HGE_argv_set_uint64(argv, offset++, (Uint64)va_arg(ap, Uint64));
	  break;
	case 'B':
	  HGE_argv_set_sint8(argv, offset++, (Sint8)va_arg(ap, int));
	  break;
	case 'S':
	  HGE_argv_set_sint16(argv, offset++, (Sint16)va_arg(ap, int));
	  break;
	case 'I':
	  HGE_argv_set_sint32(argv, offset++, (Sint32)va_arg(ap, Sint32));
	  break;
	case 'L':
	  HGE_argv_set_sint64(argv, offset++, (Sint64)va_arg(ap, Sint64));
	  break;
	case 'f':
	  HGE_argv_set_float(argv, offset++, (float)va_arg(ap, double));
	  break;
	case 'd':
	  HGE_argv_set_double(argv, offset++, (double)va_arg(ap, double));
	  break;
	case 'p':
	  HGE_argv_set_voidp(argv, offset++, (void*)va_arg(ap, void*));
	  break;
	case 'P':
	  HGE_argv_set_voidpf(argv, offset++, (void*)va_arg(ap,void*));
	  break;
	case 'h':
	case 'H':
	  HGE_argv_set_hgerc(argv, offset++, (void*)va_arg(ap,void*));
	  break;
	case 'X':
	  offset++;
	  break;
	default:
	  break;
	}
    }

  return argv;
}

void
HGE_argv_set(HGE_argv_t* argv, int offset, Uint8 type, void* data)
{
  if(offset >= argv->count)
    HGE_argv_set_length(argv, offset+1);

  argv->args[offset].type = type;
  switch(type)
    {
    case HGE_ARG_TYPE_Uint8:
      argv->args[offset].u.u8  = *(Uint8*)data;
      break;
    case HGE_ARG_TYPE_Sint8:
      argv->args[offset].u.s8  = *(Sint8*)data;
      break;
    case HGE_ARG_TYPE_Uint16:
      argv->args[offset].u.u16 = *(Uint16*)data;
      break;
    case HGE_ARG_TYPE_Sint16:
      argv->args[offset].u.s16 = *(Sint16*)data;
      break;
    case HGE_ARG_TYPE_Uint32:
      argv->args[offset].u.u32 = *(Uint32*)data;
      break;
    case HGE_ARG_TYPE_Sint32:
      argv->args[offset].u.s32 = *(Sint32*)data;
      break;
    case HGE_ARG_TYPE_Uint64:
      argv->args[offset].u.u64 = *(Uint64*)data;
      break;
    case HGE_ARG_TYPE_Sint64:
      argv->args[offset].u.s64 = *(Sint64*)data;
      break;
    case HGE_ARG_TYPE_float:
      argv->args[offset].u.f   = *(float*)data;
      break;
    case HGE_ARG_TYPE_double:
      argv->args[offset].u.d   = *(double*)data;
      break;
    case HGE_ARG_TYPE_hgerc:
    case HGE_ARG_TYPE_voidp:
    case HGE_ARG_TYPE_voidpf:
      argv->args[offset].u.p   =  data;
      break;
    default:
      argv->args[offset].u.p   =  data;
      break;
    }
}

void
HGE_argv_set_uint8(HGE_argv_t* argv, int offset, Uint8 u8)
{
  if(offset >= argv->count)
    HGE_argv_set_length(argv, offset+1);
  argv->args[offset].type = HGE_ARG_TYPE_Uint8;
  argv->args[offset].u.u8 = u8;
}

void
HGE_argv_set_sint8(HGE_argv_t* argv, int offset, Sint8 s8)
{
  if(offset >= argv->count)
    HGE_argv_set_length(argv, offset+1);
  argv->args[offset].type = HGE_ARG_TYPE_Sint8;
  argv->args[offset].u.s8 = s8;
}

void
HGE_argv_set_uint16(HGE_argv_t* argv, int offset, Uint16 u16)
{
  if(offset >= argv->count)
    HGE_argv_set_length(argv, offset+1);
  argv->args[offset].type  = HGE_ARG_TYPE_Uint16;
  argv->args[offset].u.u16 = u16;
}

void
HGE_argv_set_sint16(HGE_argv_t* argv, int offset, Sint16 s16)
{
  if(offset >= argv->count)
    HGE_argv_set_length(argv, offset+1);
  argv->args[offset].type  = HGE_ARG_TYPE_Sint16;
  argv->args[offset].u.s16 = s16;
}

void
HGE_argv_set_uint32(HGE_argv_t* argv, int offset, Uint32 u32)
{
  if(offset >= argv->count)
    HGE_argv_set_length(argv, offset+1);
  argv->args[offset].type  = HGE_ARG_TYPE_Uint32;
  argv->args[offset].u.u32 = u32;
}

void
HGE_argv_set_sint32(HGE_argv_t* argv, int offset, Sint32 s32)
{
  if(offset >= argv->count)
    HGE_argv_set_length(argv, offset+1);
  argv->args[offset].type  = HGE_ARG_TYPE_Sint32;
  argv->args[offset].u.s32 = s32;
}

void
HGE_argv_set_uint64(HGE_argv_t* argv, int offset, Uint64 u64)
{
  if(offset >= argv->count)
    HGE_argv_set_length(argv, offset+1);
  argv->args[offset].type  = HGE_ARG_TYPE_Uint64;
  argv->args[offset].u.u64 = u64;
}

void
HGE_argv_set_sint64(HGE_argv_t* argv, int offset, Sint64 s64)
{
  if(offset >= argv->count)
    HGE_argv_set_length(argv, offset+1);
  argv->args[offset].type  = HGE_ARG_TYPE_Sint64;
  argv->args[offset].u.s64 = s64;
}

void
HGE_argv_set_float(HGE_argv_t* argv, int offset, float f)
{
  if(offset >= argv->count)
    HGE_argv_set_length(argv, offset+1);
  argv->args[offset].type = HGE_ARG_TYPE_float;
  argv->args[offset].u.f  = f;
}

void
HGE_argv_set_double(HGE_argv_t* argv, int offset, double d)
{ 
  if(offset >= argv->count)
    HGE_argv_set_length(argv, offset+1);
  argv->args[offset].type = HGE_ARG_TYPE_double;
  argv->args[offset].u.d  = d;
}

void
HGE_argv_set_voidp(HGE_argv_t* argv, int offset, void* p)
{
  if(offset >= argv->count)
    HGE_argv_set_length(argv, offset+1);
  argv->args[offset].type = HGE_ARG_TYPE_voidp;
  argv->args[offset].u.p  = p;
}

void
HGE_argv_set_voidpf(HGE_argv_t* argv, int offset, void* p)
{
  if(offset >= argv->count)
    HGE_argv_set_length(argv, offset+1);
  argv->args[offset].type = HGE_ARG_TYPE_voidpf;
  argv->args[offset].u.p  = p;
}

void
HGE_argv_set_hgerc(HGE_argv_t* argv, int offset, void* p)
{
  if(offset >= argv->count)
    HGE_argv_set_length(argv, offset+1);
  argv->args[offset].type = HGE_ARG_TYPE_hgerc;
  argv->args[offset].u.p  = p;
}

int
HGE_argv_get(HGE_argv_t* argv, int offset, Uint8 type, void* data)
{
  if(offset >= argv->count)
    return HGE_INVALID_OFFSET;
  else if(argv->args[offset].type != type)
    return HGE_INCORRECT_TYPE;

  switch(type)
    {
    case HGE_ARG_TYPE_Uint8:
      *(Uint8*)data = argv->args[offset].u.u8;
      break;
    case HGE_ARG_TYPE_Sint8:
      *(Sint8*)data = argv->args[offset].u.s8;
      break;
    case HGE_ARG_TYPE_Uint16:
      *(Uint16*)data = argv->args[offset].u.u16;
      break;
    case HGE_ARG_TYPE_Sint16:
      *(Sint16*)data = argv->args[offset].u.s16;
      break;
    case HGE_ARG_TYPE_Uint32:
      *(Uint32*)data = argv->args[offset].u.u32;
      break;
    case HGE_ARG_TYPE_Sint32:
      *(Sint32*)data = argv->args[offset].u.s32;
      break;
    case HGE_ARG_TYPE_Uint64:
      *(Uint64*)data = argv->args[offset].u.u64;
      break;
    case HGE_ARG_TYPE_Sint64:
      *(Sint64*)data = argv->args[offset].u.s64;
      break;
    case HGE_ARG_TYPE_float:
      *(float*)data = argv->args[offset].u.f;
      break;
    case HGE_ARG_TYPE_double:
      *(double*)data = argv->args[offset].u.d;
      break;
    case HGE_ARG_TYPE_hgerc:
    case HGE_ARG_TYPE_voidp:
    case HGE_ARG_TYPE_voidpf:
      *(void**)data = argv->args[offset].u.p;
      break;
    default:
      break;
    }

  return HGE_OK;
}

int
HGE_argv_get_uint8(HGE_argv_t* argv, int offset, Uint8* u8)
{
  if(offset >= argv->count)
    return HGE_INVALID_OFFSET;
  else if(argv->args[offset].type != HGE_ARG_TYPE_Uint8)
    return HGE_INCORRECT_TYPE;

  *u8 = argv->args[offset].u.u8;

  return HGE_OK;
}

int
HGE_argv_get_sint8(HGE_argv_t* argv, int offset, Sint8* s8)
{
  if(offset >= argv->count)
    return HGE_INVALID_OFFSET;
  else if(argv->args[offset].type != HGE_ARG_TYPE_Sint8)
    return HGE_INCORRECT_TYPE;

  *s8 = argv->args[offset].u.s8;

  return HGE_OK;
}

int
HGE_argv_get_uint16(HGE_argv_t* argv, int offset, Uint16* u16)
{
  if(offset >= argv->count)
    return HGE_INVALID_OFFSET;
  else if(argv->args[offset].type != HGE_ARG_TYPE_Uint16)
    return HGE_INCORRECT_TYPE;

  *u16 = argv->args[offset].u.u16;

  return HGE_OK;
}

int
HGE_argv_get_sint16(HGE_argv_t* argv, int offset, Sint16* s16)
{
  if(offset >= argv->count)
    return HGE_INVALID_OFFSET;
  else if(argv->args[offset].type != HGE_ARG_TYPE_Sint16)
    return HGE_INCORRECT_TYPE;

  *s16 = argv->args[offset].u.s16;

  return HGE_OK;
}

int
HGE_argv_get_uint32(HGE_argv_t* argv, int offset, Uint32* u32)
{
  if(offset >= argv->count)
    return HGE_INVALID_OFFSET;
  else if(argv->args[offset].type != HGE_ARG_TYPE_Uint32)
    return HGE_INCORRECT_TYPE;

  *u32 = argv->args[offset].u.u32;

  return HGE_OK;
}

int
HGE_argv_get_sint32(HGE_argv_t* argv, int offset, Sint32* s32)
{
  if(offset >= argv->count)
    return HGE_INVALID_OFFSET;
  else if(argv->args[offset].type != HGE_ARG_TYPE_Sint16)
    return HGE_INCORRECT_TYPE;

  *s32 = argv->args[offset].u.s32;

  return HGE_OK;
}

int
HGE_argv_get_float(HGE_argv_t* argv, int offset, float* f)
{
  if(offset >= argv->count)
    return HGE_INVALID_OFFSET;
  else if(argv->args[offset].type != HGE_ARG_TYPE_float)
    return HGE_INCORRECT_TYPE;

  *f = argv->args[offset].u.f;

  return HGE_OK;
}

int
HGE_argv_get_double(HGE_argv_t* argv, int offset, double* d)
{
  if(offset >= argv->count)
    return HGE_INVALID_OFFSET;
  else if(argv->args[offset].type != HGE_ARG_TYPE_double)
    return HGE_INCORRECT_TYPE;

  *d = argv->args[offset].u.d;

  return HGE_OK;
}

int
HGE_argv_get_voidp(HGE_argv_t* argv, int offset, void** p)
{
  if(offset >= argv->count)
    return HGE_INVALID_OFFSET;
  else 
    {
      switch(argv->args[offset].type)
	{
	case HGE_ARG_TYPE_voidp:
	case HGE_ARG_TYPE_voidpf:
	case HGE_ARG_TYPE_hgerc:
	  break;
	default:
	  return HGE_INCORRECT_TYPE;
	}
    }

  *p = argv->args[offset].u.p;

  return HGE_OK;
}

int
HGE_argv_get_hgerc(HGE_argv_t* argv, int offset, void** p)
{
  if(offset >= argv->count)
    return HGE_INVALID_OFFSET;
  else if(argv->args[offset].type != HGE_ARG_TYPE_hgerc)
    return HGE_INCORRECT_TYPE;

  *p = argv->args[offset].u.p;

  return HGE_OK;
}
