#include "HGE.h"

void print_usage(char** argv)
{
  printf("Usage: %s\n\n"
	 "-i <file>  : input filename\n"
	 "-o <file>  : output filename\n"
	 "-a         : add\n"
	 "-d         : delete\n"
	 "-p <ptype> : primary type\n"
	 "-s <stype> : secondary type\n"
	 "-n <name>  : name\n\n", argv[0]);
  exit(-1);
}

void find_type(char* filename, HGE_obj_info_t* obj_info)
{
  int i;
  char* extention;
  static char* extarray[] = {"ogg","mp3","wav","mod","mid", 
			     "bmp","pnm","xpm","xcf","pcx","gif","jpg","tif","png","tga","lbm", NULL};
  static int ptypearray[] = {HGE_AUDIO,HGE_AUDIO,HGE_AUDIO,HGE_AUDIO,HGE_AUDIO,
			     HGE_IMAGE,HGE_IMAGE,HGE_IMAGE,HGE_IMAGE,HGE_IMAGE,HGE_IMAGE,HGE_IMAGE,HGE_IMAGE,HGE_IMAGE,HGE_IMAGE,HGE_IMAGE};
  static int stypearray[] = {HGE_AUDIO_OGG,HGE_AUDIO_MP3,HGE_AUDIO_WAV,HGE_AUDIO_MOD,HGE_AUDIO_MID,
			     HGE_IMAGE_BMP,HGE_IMAGE_PNM,HGE_IMAGE_XPM,HGE_IMAGE_XCF,HGE_IMAGE_PCX,
			     HGE_IMAGE_GIF,HGE_IMAGE_JPG,HGE_IMAGE_TIF,HGE_IMAGE_PNG,HGE_IMAGE_TGA,HGE_IMAGE_LBM};
  
  extention = strchr(filename, '.');
  if(extention == NULL)
    {
      obj_info->primary_type   = HGE_DATA;
      obj_info->secondary_type = 0;
    }
  else
    {
      extention++;
      i = 0;
      while(extarray[i])
	{
	  if(!strcasecmp(extarray[i], extention))
	    {
	      obj_info->primary_type   = ptypearray[i];
	      obj_info->secondary_type = stypearray[i];
	    }
	  i++;
	}
    }
}

char* find_arg(int argc, char** argv, char* arg)
{
  int i;
  
  i = 1;
  while(i < argc)
    {
      if(!strcmp(arg, argv[i]))
	return argv[i];
      i++;
    }

  return NULL;
}

char* get_arg(int argc, char** argv, char* arg)
{
  int i;

  i = 1;
  while(i < argc)
    {
      if(!strcmp(arg, argv[i]))
	return argv[i+1];
      i++;
    }

  return NULL;
}

#define MIN(x,y) ((x) < (y) ? (x) : (y))

int main(int argc, char** argv)
{
  int i;
  char* tmp;
  char* ifilename;
  char* ofilename;
  char* name;
  SDL_RWops* src;
  SDL_RWops* dest;
  HGE_obj_info_t info;
  
  if(argc <= 1)
    print_usage(argv);

  tmp = find_arg(argc, argv, "-a");
  if(tmp == NULL)
    print_usage(argv);

  ifilename = get_arg(argc, argv, "-i");
  if(ifilename == NULL)
    print_usage(argv);

  ofilename = get_arg(argc, argv, "-o");
  if(ofilename == NULL)
    print_usage(argv);
    
  find_type(ifilename, &info);
  name = get_arg(argc, argv, "-n");
  if(name == NULL)
    print_usage(argv);

  i = strlen(name);
  i = MIN(i, HGE_NAME_LENGTH-1);
  memset(info.name, '\0', HGE_NAME_LENGTH);
  memcpy(info.name, name, i);
  info.name[i] = '\0';

  tmp = get_arg(argc, argv, "-p");
  if(tmp)
    info.primary_type = atoi(tmp);
  tmp = get_arg(argc, argv, "-s");
  if(tmp)
    info.secondary_type = atoi(tmp);

  dest = SDL_RWFromFile(ofilename, "r+b");
  if(dest == NULL)
    {
      dest = SDL_RWFromFile(ofilename, "w+b");
      HGE_file_add_magic(dest);
      HGE_file_set_num_of_obj(dest, 0);
   } 

  src  = SDL_RWFromFile(ifilename, "rb");
  if(!dest || !src)
    print_usage(argv);

  info.length = SDL_RWseek(src, 0, SEEK_END);
  SDL_RWseek(src, 0, SEEK_SET);
  HGE_file_add_object(dest, &info, src);

  SDL_RWclose(src);
  SDL_RWclose(dest);

  return 0;
}
