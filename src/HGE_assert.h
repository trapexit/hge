#ifndef _HGE_ASSERT_H_
#define _HGE_ASSERT_H_

#define HGE_STATIC_ASSERT(e) \
  do{enum{static_assert__ = 1/(e)};}while(0)

#endif /* _HGE_ASSERT_H_ */
