#include <stdio.h>
#include <string.h>
#include "SDL.h"
#include "SDL_opengl.h"
#include "HGE_types.h"

int main(int argc, char** argv)
{
  int i;
  size_t rv;
  char copy_buffer[8192];
  FILE* packed_file;
  FILE* gfx_file;
  HGE_pack_gfx_header_t pack_gfx_header;
  HGE_gfx_file_header_t gfx_file_header;

  packed_file = fopen(argv[1], "w+");
  if(packed_file == NULL)
    {
      printf("Error opening %s\n", argv[1]);
      return -1;
    }

  pack_gfx_header.number_of_gfx = 0;
  fseek(packed_file, sizeof(HGE_pack_gfx_header_t), SEEK_SET);
  for(i = 2; i < argc; i++)
    {
      gfx_file = fopen(argv[i], "r");
      if(gfx_file == NULL)
	{
	  printf("Error opening %s: Skipping\n", argv[i]);
	  continue;
	}
      
      fseek(gfx_file, 0, SEEK_END);
      gfx_file_header.length = ftell(gfx_file);
      gfx_file_header.number = (i - 2);
      strncpy(gfx_file_header.name, argv[i], 32);
      fwrite(&gfx_file_header, sizeof(HGE_gfx_file_header_t), 1, packed_file);
      
      rewind(gfx_file);
      while(!feof(gfx_file))
	{
	  rv = fread(copy_buffer, 1, 8192, gfx_file);
	  fwrite(copy_buffer, 1, rv, packed_file);
	}
      
      fclose(gfx_file);
      pack_gfx_header.number_of_gfx++;
    }

  fseek(packed_file, 0, SEEK_SET);
  fwrite(&pack_gfx_header, sizeof(HGE_pack_gfx_header_t), 1, packed_file);

  fclose(packed_file);

  return 0;
}
