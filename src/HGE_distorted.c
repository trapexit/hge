#include "SDL.h"
#include "HGE_types.h"
#include "HGE_gl_funcs.h"
#include "HGE_image.h"
#include "HGE_distorted.h"

#define GET_VERTEX(d,x,y) (&(d)->points[(((d)->grid_w * y) + x)])

HGE_typeinfo_t HGE_distorted[] = {{sizeof(HGE_distorted_t),
				   HGE_distorted_init,
				   HGE_distorted_clean,
				   NULL,
				   NULL}};

HGE_error_t
HGE_distorted_init(void* ptr)
{
  HGE_distorted_t* d = ptr;

  d->img    = NULL;
  d->grid_w = 0;
  d->grid_h = 0;
  d->tex_w  = 0.0;
  d->tex_h  = 0.0;
  d->points = NULL;
  d->flags  = 0;

  return HGE_SUCCESS;
}

void
HGE_distorted_clean(void* ptr)
{
  HGE_distorted_t* d = ptr;

  if(d != NULL)
    {
      if(d->points != NULL)
	HGE_free(d->points);
      HGE_free_by_type(d->img, d->flags);
    }
}

void*
HGE_distorted_alloc(void)
{
  void* ptr;

  if((ptr = HGE_alloc(HGE_distorted_t)) != NULL)
    HGE_distorted_init(ptr);

  return ptr;
}

void
HGE_distorted_free(void* ptr)
{
  HGE_distorted_clean(ptr);
  HGE_free(ptr);
}

void
HGE_distorted_set_image(HGE_distorted_t* d, HGE_image_t* img)
{
  d->img = img;
  if(d->points != NULL)
    {
      d->tex_w = ((img->etwp - img->btwp) / (GLfloat)(d->grid_w - 1));
      d->tex_h = ((img->ethp - img->bthp) / (GLfloat)(d->grid_h - 1));
    }
}

HGE_image_t*
HGE_distorted_get_image(HGE_distorted_t* d)
{
  return d->img;
}

void
HGE_distorted_reset_mesh(HGE_distorted_t* d)
{
  GLfloat i,j;
  GLfloat fw,fh,fy;
  HGE_c4ub_v2f_t* points;

  fw = (GLfloat)(d->grid_w - 1);
  fh = (GLfloat)(d->grid_h - 1);
  points = d->points;
  for(j = 0.0; j <= fh; j++)
    {
      fy = ((j / fh) * d->img->h);
      for(i = 0.0; i <= fw; i++)
	{
	  points->x = ((i / fw) * d->img->w);
	  points->y = fy;
	  points->r = points->g = points->b = points->a = 255;
	  points++;
	}
    }
}

void
HGE_distorted_set_mesh_size(HGE_distorted_t* d, unsigned int w, unsigned int h)
{
  d->grid_w = w;
  d->grid_h = h;
  d->tex_w = ((d->img->etwp - d->img->btwp) / (GLfloat)(w - 1));
  d->tex_h = ((d->img->ethp - d->img->bthp) / (GLfloat)(h - 1));
  d->points = HGE_realloc(d->points, HGE_c4ub_v2f_t, (w * h));

  HGE_distorted_reset_mesh(d);
}

void
HGE_distorted_get_mesh_size(HGE_distorted_t* d, unsigned int* w, unsigned int* h)
{
  if(w != NULL)
    *w = d->grid_w;
  if(h != NULL)
    *h = d->grid_h;
}

void
HGE_distorted_set_color(HGE_distorted_t* d, int x, int y, GLubyte r, GLubyte g, GLubyte b, GLubyte a)
{
  HGE_c4ub_v2f_t* vertex = GET_VERTEX(d,x,y);

  vertex->r = r;
  vertex->g = g;
  vertex->b = b;
  vertex->a = a;
}

void
HGE_distorted_get_color(HGE_distorted_t* d, int x, int y, GLubyte* r, GLubyte* g, GLubyte* b, GLubyte* a)
{
  HGE_c4ub_v2f_t* vertex = GET_VERTEX(d,x,y);

  if(r != NULL)
    *r = vertex->r;
  if(g != NULL)
    *g = vertex->g;
  if(b != NULL)
    *b = vertex->b;
  if(a != NULL)
    *a = vertex->a;
}

void
HGE_distorted_set_displacement(HGE_distorted_t* d, int x, int y, GLfloat dx, GLfloat dy, int ref)
{
  HGE_c4ub_v2f_t* vertex = GET_VERTEX(d,x,y);
  GLfloat fw = (GLfloat)(d->grid_w - 1);
  GLfloat fh = (GLfloat)(d->grid_h - 1);

  switch(ref)
    {
    default:
    case HGE_DISTORT_POINT_ABS:
      dx += (d->img->w * ((GLfloat)x / fw));
      dy += (d->img->h * ((GLfloat)y / fh));      
      break;

    case HGE_DISTORT_POINT_REL:
      dx += vertex->x;
      dy += vertex->y;
      break;

    case HGE_DISTORT_TOPLEFT:
      break;

    case HGE_DISTORT_CENTER:
      dx += (d->img->w * 0.5);
      dy += (d->img->h * 0.5);
      break;
    }

  vertex->x = dx;
  vertex->y = dy;
}

void
HGE_distorted_get_displacement(HGE_distorted_t* d, int x, int y, GLfloat* dx, GLfloat* dy, int ref)
{
  HGE_c4ub_v2f_t* vertex = GET_VERTEX(d,x,y);
  GLfloat fw = (GLfloat)(d->grid_w - 1);
  GLfloat fh = (GLfloat)(d->grid_h - 1);
  
  if(dx != NULL)
    {
      switch(ref)
	{
	default:
	case HGE_DISTORT_POINT_ABS:
	  *dx = vertex->x - (d->img->w * (GLfloat)x / fw);
	  break;
	case HGE_DISTORT_POINT_REL:
	  *dx = vertex->x - (d->img->w * (GLfloat)x / fw);
	  break;
	case HGE_DISTORT_TOPLEFT:
	  *dx = vertex->x;
	  break;
	case HGE_DISTORT_CENTER:
	  *dx = vertex->x - (d->img->w * 0.5);
	  break;
	}
    }

  if(dy != NULL)
    {
      switch(ref)
	{
	default:
	case HGE_DISTORT_POINT_ABS:
	  *dy = vertex->y - (d->img->h * (GLfloat)y / fh);
	  break;
	case HGE_DISTORT_POINT_REL:
	  *dy = vertex->y - (d->img->h * (GLfloat)y / fh);
	  break;
	case HGE_DISTORT_TOPLEFT:
	  *dy = vertex->y;
	  break;
	case HGE_DISTORT_CENTER:
	  *dy = vertex->y - (d->img->h * 0.5);
	  break;
	}
    }
}

void
HGE_distorted_draw_mesh(HGE_distorted_t* d, HGE_spfloat_t x, HGE_spfloat_t y)
{
  
}

void
HGE_distorted_draw(HGE_distorted_t* d, GLfloat x, GLfloat y)
{
  int i,j;
  HGE_c4ub_v2f_t* vpy0;
  HGE_c4ub_v2f_t* vpy1;
  GLfloat  tex_x;
  GLfloat  tex_y0;
  GLfloat  tex_y1;

  HGE_glPushMatrix();

  HGE_glTranslatef(x,y,0.0);
  HGE_glBindTexture(GL_TEXTURE_2D, d->img->texture);

  vpy0 = d->points;
  vpy1 = vpy0 + d->grid_w;
  tex_y0 = d->img->bthp;
  tex_y1 = tex_y0 + d->tex_h;
  for(j = 0; j < d->grid_h - 1; j++)
    {
      tex_x  = d->img->btwp;
      HGE_glBegin(GL_TRIANGLE_STRIP);
      for(i = 0; i < d->grid_w; i++)
	{
	  HGE_glTexCoord2f(tex_x, tex_y0);
	  HGE_glColor4ubv((GLubyte*)&vpy0->r);
	  HGE_glVertex2fv((GLfloat*)&vpy0->x);
	  HGE_glTexCoord2f(tex_x, tex_y1);
	  HGE_glColor4ubv((GLubyte*)&vpy1->r);
	  HGE_glVertex2fv((GLfloat*)&vpy1->x);
	  tex_x += d->tex_w;
	  vpy0++;
	  vpy1++;
	}
      HGE_glEnd();

      tex_y0 = tex_y1;
      tex_y1 += d->tex_h;
    }

  HGE_glPopMatrix();
}

void
HGE_distorted_draw_transformed(HGE_distorted_t* d, GLfloat x, GLfloat y, HGE_transform_t* t)
{
  GLfloat x0,y0,x1,y1;
  int i,j;
  HGE_c4ub_v2f_t* vpy0;
  HGE_c4ub_v2f_t* vpy1;
  GLfloat  tex_x;
  GLfloat  tex_y0;
  GLfloat  tex_y1;

  x1 = (d->img->w * 0.5);
  x0 = -x1;
  y1 = (d->img->h * 0.5);
  y0 = -y1;

  HGE_glPushMatrix();

  HGE_glTranslatef((x + x1), (y + y1), 0.0);
  
  if(t->rotation != 0.0)
    HGE_glRotatef(t->rotation, 0.0, 0.0, 1.0);
  if(t->flags & TRANSFORM_H_FLIP)
    HGE_glRotatef(180.0, 0.0, 1.0, 0.0);
  if(t->flags & TRANSFORM_V_FLIP)
    HGE_glRotatef(180.0, 1.0, 0.0, 0.0);
  if(t->scale != 1.0)
    HGE_glScalef(t->scale, t->scale, 1.0);

  HGE_glBindTexture(GL_TEXTURE_2D, d->img->texture);

  vpy0 = d->points;
  vpy1 = vpy0 + d->grid_w;
  tex_y0 = d->img->bthp;
  tex_y1 = tex_y0 + d->tex_h;
  for(j = 0; j < d->grid_h - 1; j++)
    {
      tex_x  = d->img->btwp;
      HGE_glBegin(GL_TRIANGLE_STRIP);
      for(i = 0; i < d->grid_w; i++)
	{
	  HGE_glTexCoord2f(tex_x, tex_y0);
	  HGE_glColor4ubv((GLubyte*)&vpy0->r);
	  HGE_glVertex2f(vpy0->x + x0, vpy0->y + y0);
	  HGE_glTexCoord2f(tex_x, tex_y1);
	  HGE_glColor4ubv((GLubyte*)&vpy1->r);
	  HGE_glVertex2f(vpy1->x + x0, vpy1->y + y0);
	  tex_x += d->tex_w;
	  vpy0++;
	  vpy1++;
	}
      HGE_glEnd();

      tex_y0 = tex_y1;
      tex_y1 += d->tex_h;
    }

  HGE_glPopMatrix();
}
