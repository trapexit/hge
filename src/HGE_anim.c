#include <stdlib.h>
#include <string.h>
#include "HGE_basetypes.h"
#include "HGE_types.h"
#include "HGE_mem.h"
#include "HGE_random.h"
#include "HGE_anim.h"

HGE_typeinfo_t HGE_anim[] = {{sizeof(HGE_anim_t),
			      HGE_anim_init,
			      HGE_anim_clean,
			      NULL,
			      NULL}};

HGE_error_t
HGE_anim_init(void* ptr)
{
  HGE_anim_t* anim = ptr;

  anim->cframe = 0;
  anim->attr   = 0;
  anim->speed  = 0.0f;
  anim->time   = 0.0f;

  return HGE_parray_init(&anim->frames);
}

void
HGE_anim_clean(void* ptr)
{
  HGE_anim_t* anim = ptr;

  if(ptr != NULL)
    {
      HGE_parray_foreach0(&anim->frames, (HGE_func0_t)HGE_unref);
      HGE_parray_clean(&anim->frames);
    }
}

void
HGE_anim_set_frame(HGE_anim_t* anim, HGE_image_t* img, unsigned int frame)
{
  HGE_parray_set(&anim->frames, img, frame);
}

void
HGE_anim_set_frames(HGE_anim_t* anim, HGE_image_t** imgs, unsigned int count)
{
  int i;

  HGE_parray_set_size(&anim->frames, count);

  for(i = 0; i < count; i++)
    HGE_parray_index(&anim->frames, i) = imgs[i];
}

void
HGE_anim_add_frame(HGE_anim_t* anim, HGE_image_t* img)
{
  HGE_parray_add(&anim->frames, img);
}

void
HGE_anim_add_frames(HGE_anim_t* anim, HGE_image_t** imgs, unsigned int count)
{
  while(count--)
    {
      HGE_parray_add(&anim->frames, *imgs);
      imgs++;
    }
}

void
HGE_anim_set_attr(HGE_anim_t* anim, unsigned int attr)
{
  anim->attr |= attr;
}

void
HGE_anim_unset_attr(HGE_anim_t* anim, unsigned int attr)
{
  anim->attr &= ~attr;
}

void
HGE_anim_update(HGE_anim_t* anim, float dt)
{
  anim->time += dt;
  if(anim->time >= anim->speed)
    {
      anim->time -= anim->speed;
      switch(anim->attr & 0x03)
	{
	case HGE_ANIM_FWD:
	  if(anim->cframe < (HGE_parray_length(&anim->frames)-1))
	    anim->cframe++;
	  else
	    {
	      switch(anim->attr)
		{
		case HGE_ANIM_FWD | HGE_ANIM_REPEAT:
		  anim->cframe = 0;
		  break;
		case HGE_ANIM_FWD | HGE_ANIM_BOUNCE:
		case HGE_ANIM_FWD | HGE_ANIM_REPEAT | HGE_ANIM_BOUNCE:
		  if(anim->cframe)
		    anim->cframe--;
		  HGE_anim_set_attr(anim, HGE_ANIM_REV);
		  break;
		default:
		  break;
		}
	    }
	  break;
	case HGE_ANIM_REV:
	  if(anim->cframe)
	    anim->cframe--;
	  else
	    {
	      switch(anim->attr)
		{
		case HGE_ANIM_REV | HGE_ANIM_REPEAT:
		  anim->cframe = HGE_parray_length(&anim->frames);
		  break;
		case HGE_ANIM_REV | HGE_ANIM_BOUNCE:
		  HGE_anim_unset_attr(anim, HGE_ANIM_BOUNCE);
		  break;
		case HGE_ANIM_REV | HGE_ANIM_REPEAT | HGE_ANIM_BOUNCE:
		  if(HGE_parray_length(&anim->frames) > 1)
		    anim->cframe++;
		  HGE_anim_unset_attr(anim, HGE_ANIM_REV);
		  break;
		default:
		  break;
		}
	    }
	  break;

	case HGE_ANIM_RAND:
	  anim->cframe = HGE_random_int(0, HGE_parray_length(&anim->frames));
	  break;
	default:
	  break;
	}
    }
}

void
HGE_anim_updateV(HGE_anim_t* anim, unsigned int c, float dt)
{
  while(c--)
    {
      HGE_anim_update(anim, dt);
      anim++;
    }
}

void
HGE_anim_updatePV(HGE_anim_t** anim, unsigned int c, float dt)
{
  while(c--)
    {
      HGE_anim_update(*anim, dt);
      anim++;
    }
}

void
HGE_anim_draw(HGE_anim_t* anim, float x, float y)
{
  HGE_image_draw(HGE_parray_index(&anim->frames,anim->cframe),x,y);
}
