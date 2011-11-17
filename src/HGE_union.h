#ifndef _HGE_UNION_H_
#define _HGE_UNION_H_

typedef struct HGE_12_4_t HGE_12_4_t;
struct HGE_12_4_t
{
  Uint16 u12:12;
  Uint16 u4:4;
};

typedef struct HGE_12_12_8_t HGE_12_12_8_t;
struct HGE_12_12_8_t
{
  Uint32 u12_0:12;
  Uint32 u12_1:12;
  Uint32 u8:8;
};

typedef union HGE_union16_t HGE_union16_t;
union HGE_union16_t
{
  Uint8      u8[2];
  Sint8      s8[2];
  HGE_12_4_t u12;
  Uint16     u16;
  Sint16     s16;
};

typedef union HGE_union32_t HGE_union32_t;
union HGE_union32_t
{
  Uint8         u8[4];
  Sint8         s8[4];
  HGE_12_12_8_t u12;
  Uint16        u16[2];
  Sint16        s16[2];
  Uint32        u32;
  Sint32        s32;
  float         f;
};

typedef union 
{
  Uint8  u8;
  Sint8  s8;
  Uint16 u16;
  Sint16 s16;
  Uint32 u32;
  Sint32 s32;
  Uint64 u64;
  Sint64 s64;
  float  f;
  double d;
  void*  p;
} _FUT_;

#define HGE_UNION_UINT8s  (sizeof(_FUT_)/sizeof(Uint8))
#define HGE_UNION_SINT8s  (sizeof(_FUT_)/sizeof(Sint8))
#define HGE_UNION_UINT16s (sizeof(_FUT_)/sizeof(Uint16))
#define HGE_UNION_SINT16s (sizeof(_FUT_)/sizeof(Sint16))
#define HGE_UNION_UINT32s (sizeof(_FUT_)/sizeof(Uint32))
#define HGE_UNION_SINT32s (sizeof(_FUT_)/sizeof(Sint32))
#define HGE_UNION_UINT64s (sizeof(_FUT_)/sizeof(Uint64))
#define HGE_UNION_SINT64s (sizeof(_FUT_)/sizeof(Sint64))
#define HGE_UNION_FLOATs  (sizeof(_FUT_)/sizeof(float))
#define HGE_UNION_DOUBLEs (sizeof(_FUT_)/sizeof(double))
#define HGE_UNION_VOIDPs  (sizeof(_FUT_)/sizeof(void*))

typedef union HGE_union_t HGE_union_t;
union HGE_union_t
{
  Uint8  u8;
  Uint8  u8a[HGE_UNION_UINT8s];
  Sint8  s8;
  Sint8  s8a[HGE_UNION_SINT8s];
  Uint16 u16;
  Uint16 u16a[HGE_UNION_UINT16s];
  Sint16 s16;
  Sint16 s16a[HGE_UNION_SINT16s];
  Uint32 u32;
  Uint32 u32a[HGE_UNION_UINT32s];
  Sint32 s32;
  Sint32 s32a[HGE_UNION_SINT32s];
  Uint64 u64;
  Uint64 u64a[HGE_UNION_UINT64s];
  Sint64 s64;
  Sint64 s64a[HGE_UNION_SINT64s];
  float  f;
  float  fa[HGE_UNION_FLOATs];
  double d;
  double da[HGE_UNION_DOUBLEs];
  void*  p;
  void*  pa[HGE_UNION_VOIDPs];
};

#endif /* _HGE_UNION_H_ */
