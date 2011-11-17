#!/bin/sh

rm -f HGE.h

cat > HGE <<EOF
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SDL.h"
#include "SDL_endian.h"
#include "SDL_getenv.h"
#include "SDL_opengl.h"
#include "SDL_mixer.h"
#include "SDL_image.h"

EOF

for x in `echo *.h`
do
    echo "#include \"$x\"" >> HGE
done

mv HGE HGE.h