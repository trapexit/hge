#include <stdlib.h>
#include <string.h>
#include "HGE_defines.h"
#include "HGE_config.h"
#include "HGE_cmdline.h"

void
HGE_cmdline_parse(int argc, char** argv)
{
  int i;
  int xres;
  int yres;

  HGE_config_get_resolution(&xres, &yres);

  for(i = 0; argc && argv[i] != NULL; argc--,i++)
    {
      if(argv[i][0] == '-')
	{
	  switch(argv[i][1])
	    {
	    case 'a':
	      if(strcmp(argv[i], "-audio") == 0)
		HGE_config_set_audio(HGE_TRUE);
	      break;
	    case 'b':
	      if(strcmp(argv[i], "-buffer") == 0)
		{
		  i++;
		  if(argv[i] != NULL)
		    HGE_config_set_audio_buffer_size(atoi(argv[i]));
		}
	      break;
	    case 'c':
	      if(strcmp(argv[i], "-channels") == 0)
		{
		  i++;
		  if(argv[i] != NULL)
		    HGE_config_set_audio_channels(atoi(argv[i]));
		}
	      break;
	    case 'd':
	      if(strcmp(argv[i], "-doublebuffer") == 0)
		{
		  i++;
		  if(argv[i] != NULL)
		    HGE_config_set_doublebuffer(strtol(argv[i], NULL, 0));
		}
	      break;
	    case 'f':
	      if(strcmp(argv[i], "-fullscreen") == 0 ||
		 strcmp(argv[i], "-fs") == 0)
		{
		  i++;
		  if(argv[i] != NULL)
		    HGE_config_set_fullscreen(strtol(argv[i], NULL, 0));
		}
	      else if(strcmp(argv[i], "-frame") == 0)
		{
		  i++;
		  if(argv[i] != NULL)
		    HGE_config_set_no_frame(!strtol(argv[i], NULL, 0));
		}
	      break;
	    case 'n':
	      if(strcmp(argv[i], "-noframe") == 0)
		{
		  i++;
		  if(argv[i] != NULL)
		    HGE_config_set_no_frame(strtol(argv[i], NULL, 0));
		}
	      break;
	    case 'r':
	      if(strcmp(argv[i], "-resizable") == 0)
		{
		  i++;
		  if(argv[i] != NULL)
		    HGE_config_set_resizable(strtol(argv[i], NULL, 0));
		};
	      break;
	    case 't':
	      if(strcmp(argv[i], "-texture_format") == 0)
		{
		  i++;
		  if(argv[i] != NULL)
		    HGE_config_set_texture_format(strtol(argv[i], NULL, 0));
		}
	      break;
	    case 'x':
	      if(strcmp(argv[i], "-xres") == 0)
		{
		  i++;
		  if(argv[i] != NULL)
		    xres = atoi(argv[i]);
		}
	      break;
	    case 'y':
	      if(strcmp(argv[i], "-yres") == 0)
		{
		  i++;
		  if(argv[i] != NULL)
		    yres = atoi(argv[i]);
		}
	      break;
	    default:
	      break;
	    }
	}
    }

  HGE_config_set_resolution(xres, yres);
}
