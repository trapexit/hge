#ifndef _HGE_ERROR_H_
#define _HGE_ERROR_H_

typedef enum HGE_error_t HGE_error_t;
enum HGE_error_t
  {
    HGE_SUCCESS           = 0x0000,
    HGE_OK                = 0x0000,
    HGE_NOERROR           = 0x0000,
    HGE_FAILURE           = 0x0001,
    HGE_NOTOK             = 0x0001,
    HGE_HASH_INIT_FAILED  = 0x0A00,
    HGE_HASH_ALLOC_FAILED = 0x0A01,
    HGE_HASH_IN_USE       = 0x0A02,
    HGE_LOOP_INIT_FAILED  = 0x0200,
    HGE_LOOP_ALLOC_FAILED = 0x0201,

    HGE_LAST_ERROR
  };

#endif /* _HGE_ERROR_H_ */
