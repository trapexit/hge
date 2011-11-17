#ifndef _HGE_TIME_H_
#define _HGE_TIME_H_

#define HGE_time_getf()    (((GLfloat)SDL_GetTicks())*0.001f)
#define HGE_time_geti()    (SDL_GetTicks())
#define HGE_time_deltai(x) (HGE_time_geti() - x)
#define HGE_time_deltaf(x) (HGE_time_getf() - x)

#endif /* _HGE_TIME_H_ */
