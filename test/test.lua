HGE.initialize_video();
HGE.initialize_timer();

surface = SDL.IMG_Load("icon.png");
print(surface);
tex = HGE.surface_make_texture(surface, 0);
print(tex);

gl.BindTexture(gl.TEXTURE_2D, tex);
gl.Begin(gl.QUADS);
gl.TexCoord2f(0,0); gl.Vertex2f(0, 0);
gl.TexCoord2f(1,0); gl.Vertex2f(100,0);
gl.TexCoord2f(1,1); gl.Vertex2f(100,100);
gl.TexCoord2f(0,1); gl.Vertex2f(0, 100);
gl.End();

--img = HGE.image_alloc();
--img:set_w(32);
--img:set_h(32);	
--print(img);
--for n in pairs(getmetatable(img))
--do
--  print(n, type(n))
--end

--HGE.image_draw(img, 100, 100);

HGE.swapbuffers();

SDL.Delay(2000);

HGE.quit();

