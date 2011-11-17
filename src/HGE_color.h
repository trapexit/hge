#ifndef _HGE_COLOR_H_
#define _HGE_COLOR_H_

#include "SDL.h"
#include "SDL_opengl.h"

#define HGE_color_set3(r,g,b)   HGE_glColor3ub((r),(g),(b))
#define HGE_color_set4(r,g,b,a) HGE_glColor4ub((r),(g),(b),(a))
#define HGE_color_set3v(c)      HGE_glColor3ubv((c))
#define HGE_color_set4v(c)      HGE_glColor4ubv((c))
#define HGE_color_reset3()      HGE_color_set3(255,255,255)
#define HGE_color_reset4()      HGE_color_set4(255,255,255,255)
#define HGE_color_set_blend(x,y) HGE_glBlendFunc((x),(y))
#define HGE_color_default_blend() HGE_color_set_blend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
#define HGE_enable_alpha_channel() HGE_glEnable(GL_BLEND)
#define HGE_disable_alpha_channel() HGE_glDisable(GL_BLEND)

#endif /* _HGE_COLOR_H_ */
