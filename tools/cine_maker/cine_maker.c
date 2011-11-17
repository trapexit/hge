#include <ctype.h>
#include "HGE.h"

int
get_image_event_type(const char* name)
{
  if(!strcasecmp("ENABLE", name))
    return HGE_IMAGE_ENABLE_EVENT;
  else if(!strcasecmp("DISABLE", name))
    return HGE_IMAGE_DISABLE_EVENT;
  else if(!strcasecmp("SET", name))
    return HGE_IMAGE_SET_EVENT;
  else if(!strcasecmp("INC", name))
    return HGE_IMAGE_INC_EVENT;

  return HGE_IMAGE_NOP_EVENT;
}

int
find_section(FILE* f, char* category, char* type)
{
  char* tmp;
  char buffer[256];

  rewind(f);
  while(!feof(f))
    {
      fgets(buffer, sizeof(buffer), f);
      if(buffer[0] == '[')
	{
	  tmp = strtok(buffer, "[] :\n");
	  if(!strcmp(tmp, category))
	    {
	      tmp = strtok(NULL, "[] :\n");
	      if(!strcmp(type, tmp))
		return ftell(f);
	    }
	}
    }

  return -1;
}

int
handle_object_images(FILE* infile, HGE_cine_t* c)
{
  char* ptr;
  char* name;
  char buffer[256];
  HGE_object_t o;

  while(!feof(infile))
    {
      fgets(buffer, sizeof(buffer), infile);
      if(buffer[0] == '[')
	break;
      else if(!isalpha(buffer[0]))
	continue;

      HGE_cine_init_object_t(&o);

      printf("%s", buffer);
      name  = strtok(buffer, " \n");
      strncpy(o.image.info.name, name, HGE_NAME_LENGTH);
      o.image.info.name[HGE_NAME_LENGTH] = '\0';
      o.image.info.type = HGE_IMAGE_OBJECT;
      ptr   = strtok(NULL, " \n");
      o.image.x = atoi(ptr ? ptr : "0");
      ptr   = strtok(NULL, " \n");
      o.image.y = atoi(ptr ? ptr : "0");
      ptr                    = strtok(NULL, " \n");
      o.image.transform.scale    = atof(ptr ? ptr : "0.0");
      ptr                    = strtok(NULL, " \n");
      o.image.transform.rotation = atof(ptr ? ptr : "0.0");
      ptr                    = strtok(NULL, " \n");
      o.image.transform.h_flip   = atoi(ptr ? ptr : "0");
      ptr                    = strtok(NULL, " \n");
      o.image.transform.v_flip   = atoi(ptr ? ptr : "0");

      HGE_cine_add_object(c, &o);
    }

  return 0;
}

int
handle_events_image(FILE* infile, HGE_cine_t* c)
{
  char* ptr;
  char buffer[256];
  HGE_event_t e;

  while(!feof(infile))
    {
      fgets(buffer, sizeof(buffer), infile);
      if(buffer[0] == '[')
	break;
      else if(!isalpha(buffer[0]))
	continue;

      printf("%s", buffer);

      HGE_cine_init_event_t(&e);

      ptr = strtok(buffer, " \n");
      strncpy(e.image.info.name, ptr, HGE_NAME_LENGTH);
      e.image.info.name[HGE_NAME_LENGTH] = '\0';

      ptr = strtok(NULL, " \n");
      e.image.info.stype = get_image_event_type(ptr);

      ptr = strtok(NULL, " \n");
      if(ptr && ptr[0] != '-')
	{
	  e.image.data.x = atoi(ptr);
	  e.image.data.fields |= HGE_X_FIELD;
	}

      ptr = strtok(NULL, " \n");
      if(ptr && ptr[0] != '-')
	{
	  e.image.data.y = atoi(ptr);
	  e.image.data.fields |= HGE_Y_FIELD;
	}

      ptr = strtok(NULL, " \n");
      if(ptr && ptr[0] != '-')
	{
	  e.image.data.transform.scale = atof(ptr);
	  e.image.data.fields |= HGE_SCALE_FIELD;
	}

      ptr = strtok(NULL, " \n");
      if(ptr && ptr[0] != '-')
	{
	  e.image.data.transform.rotation = atof(ptr);
	  e.image.data.fields |= HGE_ROTATION_FIELD;
	}

      ptr = strtok(NULL, " \n");
      if(ptr && ptr[0] != '-')
	{
	  e.image.data.transform.h_flip = atoi(ptr);
	  e.image.data.fields |= HGE_H_FLIP_FIELD;
	}

      ptr = strtok(NULL, " \n");
      if(ptr && ptr[0] != '-')
	{
	  e.image.data.transform.v_flip = atoi(ptr);
	  e.image.data.fields |= HGE_V_FLIP_FIELD;
	}     
      
      if(e.image.info.stype != HGE_IMAGE_NOP_EVENT)
	HGE_cine_add_event(c, &e);
    }

  return 0;
}

int
handle_triggers_time(FILE* infile, HGE_cine_t* c)
{
  char* ptr;
  char  buffer[256];
  HGE_trigger_t t;


  while(!feof(infile))
    {
      fgets(buffer, sizeof(buffer), infile);
      if(buffer[0] == '[')
	break;
      else if(!isdigit(buffer[0]))
	continue;

      HGE_cine_init_trigger_t(&t);
      t.time.info.type = HGE_TIME_TRIGGER;

      printf("%s", buffer);
      
      ptr = strtok(buffer, " \n");
      t.time.time = atoi(ptr);
      
      ptr = strtok(NULL, " \n");
      t.time.info.event = HGE_cine_find_event_t(c, ptr);
      
      ptr = strtok(NULL, " \n");
      t.time.info.object = HGE_cine_find_object_t(c, ptr);

      if(t.info.object && t.info.event)
	HGE_cine_add_trigger(c, &t);
    }
	
  return 0;
}

int
main(int argc, char** argv)
{
  char* infilename;
  char* outfilename;
  FILE* infile;
  FILE* outfile;
  HGE_cine_t cine;
  SDL_RWops* rwop;

  infilename  = argv[1];
  outfilename = argv[2];
  infile  = fopen(infilename, "rb");
  outfile = fopen(outfilename, "wb");

  HGE_cine_init(&cine);
  find_section(infile, "objects", "images");
  handle_object_images(infile, &cine);
  find_section(infile, "events", "image");
  handle_events_image(infile, &cine);
  find_section(infile, "triggers", "time");
  handle_triggers_time(infile, &cine);

  rwop = SDL_RWFromFP(outfile, SDL_FALSE);
  HGE_serialize_write_cine_t(rwop, &cine);

  fclose(infile);
  fclose(outfile);

  return 0;
}
