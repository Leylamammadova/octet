////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//
// invaderer example: simple game with sprites and sounds
//
// Level: 1
//
// Demonstrates:
//   Basic framework app
//   Shaders
//   Basic Matrices
//   Simple game mechanics
//   Texture loaded from GIF file
//   Audio
//

namespace octet {
  class sprite {
    // where is our sprite (overkill for a 2D game!)
    mat4t modelToWorld;

    // half the width of the sprite
    float halfWidth;

    // half the height of the sprite
    float halfHeight;

    // what texture is on our sprite
    int texture;

    // true if this sprite is enabled.
    bool enabled;
  public:
    sprite() {
      texture = 0;
      enabled = true;
    }

    void init(int _texture, float x, float y, float w, float h) {
      modelToWorld.loadIdentity();
      modelToWorld.translate(x, y, 0);
      halfWidth = w * 0.5f;
      halfHeight = h * 0.5f;
      texture = _texture;
      enabled = true;
    }

    void render(texture_shader &shader, mat4t &cameraToWorld) {
      
      if (!texture) return;

      // build a projection matrix: model -> world -> camera -> projection
      // the projection space is the cube -1 <= x/w, y/w, z/w <= 1
      mat4t modelToProjection = mat4t::build_projection_matrix(modelToWorld, cameraToWorld);

      // set up opengl to draw textured triangles using sampler 0 (GL_TEXTURE0)
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture);

      // use "old skool" rendering
      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      shader.render(modelToProjection, 0);

      // this is an array of the positions of the corners of the sprite in 3D
      // a straight "float" here means this array is being generated here at runtime.
      float vertices[] = {
        -halfWidth, -halfHeight, 0,
         halfWidth, -halfHeight, 0,
         halfWidth,  halfHeight, 0,
        -halfWidth,  halfHeight, 0,
      };

      // attribute_pos (=0) is position of each corner
      // each corner has 3 floats (x, y, z)
      // there is no gap between the 3 floats and hence the stride is 3*sizeof(float)
      glVertexAttribPointer(attribute_pos, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)vertices );
      glEnableVertexAttribArray(attribute_pos);
    
      // this is an array of the positions of the corners of the texture in 2D
      static const float uvs[] = {
         0,  0,
         1,  0,
         1,  1,
         0,  1,
      };

      // attribute_uv is position in the texture of each corner
      // each corner (vertex) has 2 floats (x, y)
      // there is no gap between the 2 floats and hence the stride is 2*sizeof(float)
      glVertexAttribPointer(attribute_uv, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)uvs );
      glEnableVertexAttribArray(attribute_uv);
    
      // finally, draw the sprite (4 vertices)
      glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }

    // move the object
    void translate(float x, float y) {
      modelToWorld.translate(x, y, 0);
    }

    // position the object relative to another.
    void set_relative(sprite &rhs, float x, float y) {
      modelToWorld = rhs.modelToWorld;
      modelToWorld.translate(x, y, 0);
    }

    // return true if this sprite collides with another.
    // note the "const"s which say we do not modify either sprite
    bool collides_with(const sprite &rhs) const {
      float dx = rhs.modelToWorld[3][0] - modelToWorld[3][0];
      float dy = rhs.modelToWorld[3][1] - modelToWorld[3][1];

      // both distances have to be under the sum of the halfwidths
      // for a collision
      return
        (fabsf(dx) < halfWidth + rhs.halfWidth) &&
        (fabsf(dy) < halfHeight + rhs.halfHeight)
      ;
    }

    bool is_above(const sprite &rhs, float margin) const {
      float dx = rhs.modelToWorld[3][0] - modelToWorld[3][0];

      return
        (fabsf(dx) < halfWidth + margin)
      ;
    }

    bool &is_enabled() {
      return enabled;
    }
  };

  class invaderers_app : public octet::app {
    // Matrix to transform points in our camera space to the world.
    // This lets us move our camera
    mat4t cameraToWorld;

    // shader to draw a textured triangle
    texture_shader texture_shader_;

    enum {
      num_sound_sources = 40,
	  num_rows = 108,
      num_cols = 16,
      clouds_sprites,
      num_borders = 10,
      num_invaderers = num_rows * num_cols,
	  num_bricks= num_rows * num_cols,
	  game_over_sprite,
	  you_won_sprite,
      ship_sprite,
      wall_sprites,
      first_invaderer_sprite,
      last_invaderer_sprite = first_invaderer_sprite + num_invaderers - 1,
	  first_border_sprite,
      last_border_sprite = first_border_sprite + num_borders - 1,

      num_sprites,

    };

    // accounting for bad guys
    int live_invaderers;
    int num_lives;

    // game state
    bool game_over;
	int frames;

    // speed of enemy
    float invader_velocity;

    // sounds
   /* ALuint whoosh;
    ALuint bang;*/
	ALuint m83;
    unsigned cur_source;
    ALuint sources[num_sound_sources];

    // big array of sprites
    sprite sprites[num_sprites];

    // random number generator
    class random randomizer;

    // a texture for our text
    GLuint font_texture;


    // information for our text
    bitmap_font font;

    ALuint get_sound_source() { return sources[cur_source++ % num_sound_sources]; }

    // called when we hit an enemy
    /*void on_hit_invaderer() {
      ALuint source = get_sound_source();
      alSourcei(source, AL_BUFFER, bang);
      alSourcePlay(source);

      
      if (live_invaderers == 4) {
        invader_velocity *= 4;
      } else if (live_invaderers == 0) {
        game_over = true;
        sprites[game_over_sprite].translate(-20, 0);
      }
    }*/

    // called when we are hit
   /* void on_hit_ship() {
      ALuint source = get_sound_source();
      alSourcei(source, AL_BUFFER, bang);
      alSourcePlay(source);

    }*/

    // use the wasd to move the ship
    void move_ship() {

      const float ship_speed = 0.1f;
      //if you stuck you die
	    if (sprites[ship_sprite].collides_with(sprites[first_border_sprite])) {
		   if (--num_lives == 0) {
			   game_over = true;
			   sprites[game_over_sprite].translate(-20, 0);
		   }
	    } 

	    if(is_key_down(key_w)) {
		  sprites[ship_sprite].translate(0, +ship_speed);
		  for (int i = 0; i < num_invaderers; i++) {
			  if (sprites[ship_sprite].collides_with(sprites[wall_sprites+i])) {
				  sprites[ship_sprite].translate(0, -ship_speed);
			  }
		    }
	      }
	         else if (is_key_down(key_s)) {
		  sprites[ship_sprite].translate(0, -ship_speed);
		if (sprites[ship_sprite].collides_with(sprites[first_border_sprite])) {
			  sprites[ship_sprite].translate(0, +ship_speed);
		  }
	    for (int i = 0; i < num_invaderers; i++) {
			 if (sprites[ship_sprite].collides_with(sprites[wall_sprites + i])) {
				 sprites[ship_sprite].translate(0, +ship_speed);
			 }

		 }
	  }
               if (is_key_down(key_a)) {
                 sprites[ship_sprite].translate(-ship_speed, 0);
		          for (int i = 0; i < num_invaderers; i++) {
			         if (sprites[ship_sprite].collides_with(sprites[wall_sprites + i])) {
				        sprites[ship_sprite].translate(+ship_speed, 0);
			         }
		            }
                        if (sprites[ship_sprite].collides_with(sprites[first_border_sprite+2])) {
                           sprites[ship_sprite].translate(+ship_speed, 0);
                             }
                   } else if (is_key_down(key_d)) {
                         sprites[ship_sprite].translate(+ship_speed, 0);
		        for (int i = 0; i < num_invaderers; i++) {
			     if (sprites[ship_sprite].collides_with(sprites[wall_sprites + i])) {
				  sprites[ship_sprite].translate(-ship_speed, 0);
		 	       }
		         }
		            if (sprites[ship_sprite].collides_with(sprites[first_border_sprite+3])) {
          sprites[ship_sprite].translate(-ship_speed, 0);
        }
      }
    }


    void draw_text(texture_shader &shader, float x, float y, float scale, const char *text) {
      mat4t modelToWorld;
      modelToWorld.loadIdentity();
      modelToWorld.translate(x, y, 0);
      modelToWorld.scale(scale, scale, 1);
      mat4t modelToProjection = mat4t::build_projection_matrix(modelToWorld, cameraToWorld);

      enum { max_quads = 32 };
      bitmap_font::vertex vertices[max_quads*4];
      uint32_t indices[max_quads*6];
      aabb bb(vec3(0, 0, 0), vec3(256, 256, 0));

      unsigned num_quads = font.build_mesh(bb, vertices, indices, max_quads, text, 0);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, font_texture);

      shader.render(modelToProjection, 0);

      glVertexAttribPointer(attribute_pos, 3, GL_FLOAT, GL_FALSE, sizeof(bitmap_font::vertex), (void*)&vertices[0].x );
      glEnableVertexAttribArray(attribute_pos);
      glVertexAttribPointer(attribute_uv, 3, GL_FLOAT, GL_FALSE, sizeof(bitmap_font::vertex), (void*)&vertices[0].u );
      glEnableVertexAttribArray(attribute_uv);

      glDrawElements(GL_TRIANGLES, num_quads * 6, GL_UNSIGNED_INT, indices);
    }

  public:

    invaderers_app(int argc, char **argv) : app(argc, argv), font(512, 256, "assets/big.fnt") {
    }
	
	
	//big thanks to Robert for explaining me this
	//http://stackoverflow.com/questions/13035674/how-to-read-line-by-line-or-a-whole-text-file-at-once

	 char readfile(int arrayPos) {
		 std::ifstream myFile("borders.txt");
		 std::string text;
		 std::string com_line;
		
		 while (std::getline(myFile, text)) {
			com_line += text;
		}
		
		 // http://www.cplusplus.com/forum/general/100714/
		   char *chars = new char[com_line.length()];
		   std::strcpy(chars, com_line.c_str());

		   return chars[arrayPos];

	}
	
		// this is called once OpenGL is initialized
	void app_init() {

		// set up the shader
		texture_shader_.init();

		font_texture = resource_dict::get_texture_handle(GL_RGBA, "assets/big_0.gif");
		//i do not own this picture
		
		GLuint YouWon = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/youwon.gif");
		sprites[you_won_sprite].init(YouWon, 0, 58, 6, 4);

		GLuint ship = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/ship.gif");
		sprites[ship_sprite].init(ship, 0, -3, 0.5f, 0.5f);
     	//i do not own this picture
		GLuint brick = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/brick.gif");
		
		//GLuint clouds = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/clouds.gif");
		//sprites[clouds_sprites].init(clouds, 0, 58, 5, 6);
    	
		//GLuint invaderer = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/invaderer.gif");
		    for (int j = 0; j != num_rows; ++j) {
			for (int i = 0; i != num_cols; ++i) {
			
				float brick_size = 0.5f;
				float x = ((float)i - num_cols * 0.5f) * brick_size;
				float y = 55.50f - ((float)j * brick_size);
				
				                 switch (readfile(i + j*num_cols)) {
				                 case 'b':
                                 sprites[wall_sprites + i + j*num_cols].init(brick, x, y, brick_size, brick_size);
					             break;
				                        case '.':
                                        break;
	
				                       default:
					                   std::cout << "not reading file";
				}
			}
			
		}

		

			// sounds
		/*	whoosh = resource_dict::get_sound_handle(AL_FORMAT_MONO16, "assets/invaderers/whoosh.wav");
			bang = resource_dict::get_sound_handle(AL_FORMAT_MONO16, "assets/invaderers/bang.wav");*/
			
			m83 = resource_dict::get_sound_handle(AL_FORMAT_MONO16, "assets/invaderers/m83.wav");
			cur_source = 0;
			alGenSources(num_sound_sources, sources);
			ALuint source = get_sound_source();
			alSourcei(source, AL_BUFFER, m83);
			alSourcePlay(source);
		

			// sundry counters and game state.
			invader_velocity = 0.05f;
			live_invaderers = num_invaderers;
			num_lives = 1;
			game_over = false;
			frames = 0;
		
	}


	void move_camera( ) {
		cameraToWorld.loadIdentity();
		cameraToWorld.translate(0.0f, frames/20.0f, 5.0f);
		
	}
	
	void moving_borders() {
		
			// made the bottom border follow the player
	    GLuint grey = resource_dict::get_texture_handle(GL_RGB, "#7a7a7a");
		float m = -5 + frames / 24.0f;
		sprites[first_border_sprite + 0].init(grey, 0, m, 10, 0.5f);

		//made game over appear in front of eyes 
		GLuint GameOver = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/GameOver.gif");
		sprites[game_over_sprite].init(GameOver, 20, frames / 20.0f, 3, 1.5f);
		
       //borders are moving up too
		GLuint wall = resource_dict::get_texture_handle(GL_RGB, "assets/invaderers/wall.gif");
		sprites[first_border_sprite + 2].init(wall, -7, frames / 20.0f, 8, 10);
		sprites[first_border_sprite + 3].init(wall, 7, frames / 20.0f, 8, 10);

		//GLuint clouds = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/clouds.gif");
		//sprites[clouds_sprites].init(clouds, 0, 65, 5, 6);
		
		//GLuint YouWon = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/youwon.gif");
		//sprites[you_won_sprite].init(YouWon, 10, 58, 3, 3);
		
		/*if (sprites[ship_sprite].collides_with(sprites[clouds_sprites])) {
		        you_won = true;
				sprites[you_won_sprite].translate(-10, 0);
				if (you_won) {
					return;
				}
			}*/
		
	}

   /* void pausegame() {
		if (is_key_down(key_p)) {
			game_over = !game_over;
		} 
	}*/

    // called every frame to move things
    void simulate() {
      if (game_over) {
		  return;
      }	   
	  
	

	  moving_borders();

      move_camera();

      move_ship();
	  
	  frames++;
	
	 
	  //restarts the game
	  if (is_key_down(key_shift)) {

		  return app_init();
	   }

    }

	
	 // this is called to draw the world
	void draw_world(int x, int y, int w, int h) {
      simulate();

	  
	   // set a viewport - includes whole window area
      glViewport(x, y, w, h);
	  
      // clear the background to  green
	  
	  
      glClearColor(0, 1, 1, 0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // don't allow Z buffer depth testing (closer objects are always drawn in front of far ones)
	  glDisable(GL_DEPTH_TEST);

      // allow alpha blend (transparency when alpha channel is 0)
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      // draw all the sprites
      for (int i = 0; i != num_sprites; ++i) {
        sprites[i].render(texture_shader_, cameraToWorld);
      }

      char score_text[32];
      sprintf(score_text, "score: %d\n", frames);
	  draw_text(texture_shader_, -3.0f, frames / 20.0f, 1.0f / 256, score_text);
	  
      // move the listener with the camera
      vec4 &cpos = cameraToWorld.w();
      alListener3f(AL_POSITION, cpos.x(), cpos.y(), cpos.z());
    }
  };
}
