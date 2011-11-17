#ifndef _HGE_VECTOR_H_
#define _HGE_VECTOR_H_

#define HGE_vector_add2(dest,src) {(dest)->x += (src)->x; (dest)->y += (src)->y;}
#define HGE_vector_sub2(dest,src) {(dest)->x -= (src)->x; (dest)->y -= (src)->y;}
#define HGE_vector_mulf(dest,v) {(dest)->x *= v; (dest)->y *= v;}
#define HGE_vector_divf(dest,v) {float d = (1.0f / v); HGE_vector_mul2(dest,d);}

#endif /* _HGE_VECTOR_H_ */
