#ifndef _HGE_BIT_H_
#define _HGE_BIT_H_

enum HGE_bits_t
  {
    BIT0 = 0x0001,
    BIT1 = 0x0002,
    BIT2 = 0x0004,
    BIT3 = 0x0008,
    BIT4 = 0x0010,
    BIT5 = 0x0020,
    BIT6 = 0x0040,
    BIT7 = 0x0080
  };

#define HGE_bit_isset(v, n) ((v >> n) & BIT0)
#define HGE_bit_set(v, n)   ((v) = ((v) | (BIT0 << n)))
#define HGE_bit_reset(v,n)  ((v) = ((v) & (~0 ^ (BIT0 << n)))

#endif /* _HGE_BIT_H_ */
