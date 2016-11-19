////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//
// invaderer example: simple game with sprites and sounds
//


namespace octet {
  class sprite {
    
    mat4t modelToWorld;

    float halfWidth;

    float halfHeight;
   
    int texture;

    bool enabled;


  public:
	  //easy to render specific sprites
	  bool invert = false;

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

      mat4t modelToProjection = mat4t::build_projection_matrix(modelToWorld, cameraToWorld);

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture);

      shader.render(modelToProjection, 0);

      float vertices[] = {
        -halfWidth, -halfHeight, 0,
         halfWidth, -halfHeight, 0,
         halfWidth,  halfHeight, 0,
        -halfWidth,  halfHeight, 0,
      };

      glVertexAttribPointer(attribute_pos, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)vertices );
      glEnableVertexAttribArray(attribute_pos);
    
      static const float uvs[] = {
         0,  0,
         1,  0,
         1,  1,
         0,  1,
      };

      glVertexAttribPointer(attribute_uv, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)uvs );
      glEnableVertexAttribArray(attribute_uv);
    
      glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
	//for rendering another shader
	void render(invert_shader &shader, mat4t &cameraToWorld) {

		if (!texture) return;

		mat4t modelToProjection = mat4t::build_projection_matrix(modelToWorld, cameraToWorld);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		shader.render(modelToProjection, 0);

		float vertices[] = {
			-halfWidth, -halfHeight, 0,
			halfWidth, -halfHeight, 0,
			halfWidth,  halfHeight, 0,
			-halfWidth,  halfHeight, 0,
		};

		glVertexAttribPointer(attribute_pos, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)vertices);
		glEnableVertexAttribArray(attribute_pos);

		static const float uvs[] = {
			0,  0,
			1,  0,
			1,  1,
			0,  1,
		};

		glVertexAttribPointer(attribute_uv, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)uvs);
		glEnableVertexAttribArray(attribute_uv);

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}


    void translate(float x, float y) {
      modelToWorld.translate(x, y, 0);
    }

    void set_relative(sprite &rhs, float x, float y) {
      modelToWorld = rhs.modelToWorld;
      modelToWorld.translate(x, y, 0);
    }

    
    bool collides_with(const sprite &rhs) const {
      float dx = rhs.modelToWorld[3][0] - modelToWorld[3][0];
      float dy = rhs.modelToWorld[3][1] - modelToWorld[3][1];

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

    mat4t cameraToWorld;

    texture_shader texture_shader_;
	invert_shader invert_shader_;


    enum {
      num_sound_sources = 8,
	  num_rows = 108,
      num_cols = 16,
      rules_sprite,
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

    int num_lives;

    bool game_over;
	int frames;

	ALuint m83;
    unsigned cur_source;
    ALuint sources[num_sound_sources];

    sprite sprites[num_sprites];

    class random randomizer;

    GLuint font_texture;

    bitmap_font font;

    ALuint get_sound_source() { return sources[cur_source++ % num_sound_sources]; }

    // use the wasd to move the ship
    void move_ship() {

      const float ship_speed = 0.06f;
      //if you stuck game is over
	    if (sprites[ship_sprite].collides_with(sprites[first_border_sprite])) {
		   if (--num_lives == 0) {
			   game_over = true;
			   sprites[game_over_sprite].translate(-20, 0);
		   }
	    } 

	    if(is_key_down(key_w)) {
		  sprites[ship_sprite].translate(0, +ship_speed);
		  for (int i = 0; i < num_bricks; i++) {
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
	    for (int i = 0; i < num_bricks; i++) {
			 if (sprites[ship_sprite].collides_with(sprites[wall_sprites + i])) {
				 sprites[ship_sprite].translate(0, +ship_speed);
			 }

		 }
	  }
               if (is_key_down(key_a)) {
                 sprites[ship_sprite].translate(-ship_speed, 0);
		          for (int i = 0; i <num_bricks; i++) {
			         if (sprites[ship_sprite].collides_with(sprites[wall_sprites + i])) {
				        sprites[ship_sprite].translate(+ship_speed, 0);
			         }
		            }
                        if (sprites[ship_sprite].collides_with(sprites[first_border_sprite+2])) {
                           sprites[ship_sprite].translate(+ship_speed, 0);
                             }
                   } else if (is_key_down(key_d)) {
                         sprites[ship_sprite].translate(+ship_speed, 0);
		        for (int i = 0; i < num_bricks; i++) {
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

		
	void app_init() {

		// set up the shader
		texture_shader_.init();
		invert_shader_.init();

		font_texture = resource_dict::get_texture_handle(GL_RGBA, "assets/big_0.gif");

		
		GLuint YouWon = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/youwon.gif");
		sprites[you_won_sprite].init(YouWon, 0, 58, 6, 4);

		GLuint rules = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/rules.gif");
		sprites[rules_sprite].init(rules, 0, 0, 5, 3);

		GLuint ship = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/ship.gif");
		sprites[ship_sprite].init(ship, 0, -3, 0.5f, 0.5f);
   
		GLuint brick = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/brick.gif");
		

		//big thanks to Robert for explaining me this

		for (int j = 0; j != num_rows; ++j) {
			for (int i = 0; i != num_cols; ++i) {
			
				float brick_size = 0.5f;
				float x = ((float)i - num_cols * 0.5f) * brick_size;
				float y = 55.50f - ((float)j * brick_size);
				
				switch (readfile(i + j*num_cols)) {
				  case 'b':
            sprites[wall_sprites + i + j*num_cols].init(brick, x, y, brick_size, brick_size);
						sprites[wall_sprites + i + j*num_cols].invert = true;
	          break;
				  case '.':
            break;
	
				  default:
    				std::cout << "not reading file";
				}
			}
			
		}

		

		
		    m83 = resource_dict::get_sound_handle(AL_FORMAT_MONO16, "assets/invaderers/m83.wav");
			cur_source = 0;
			alGenSources(num_sound_sources, sources);
			ALuint source = get_sound_source();
			alSourcei(source, AL_BUFFER, m83);
			alSourcePlay(source);
		

			num_lives = 1;
			game_over = false;
			frames = 0;
		
	}


	void move_camera( ) {
		cameraToWorld.loadIdentity();
		float k = frames / 20.0f;
		cameraToWorld.translate(0.0f, k, 5.0f);

	}
	
	void moving_borders() {


		// made the bottom border follow the player
		GLuint grey = resource_dict::get_texture_handle(GL_RGB, "#7a7a7a");
		float m = -5 + frames / 22.0f;
		sprites[first_border_sprite + 0].init(grey, 0, m, 10, 0.5f);

		//made game over appear in front of eyes 
		GLuint GameOver = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/GameOver.gif");
		sprites[game_over_sprite].init(GameOver, 20, frames / 20.0f, 3, 1.5f);

		//borders are moving up too
		GLuint wall = resource_dict::get_texture_handle(GL_RGB, "assets/invaderers/wall.gif");

		sprites[first_border_sprite + 2].init(wall, -7, frames / 20.0f, 8, 10);
		sprites[first_border_sprite + 3].init(wall, 7, frames / 20.0f, 8, 10);

		
		if (sprites[ship_sprite].collides_with(sprites[you_won_sprite])) {
			game_over = true;
		}
	}
	//pause game
    void pausegame() {
		if (is_key_down(key_p)) {
			game_over = !game_over;
		} 
	}

  
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

	  pausegame();
	  
      glViewport(x, y, w, h);
	  
      glClearColor(0, 1, 1, 0);
	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	 
	  glDisable(GL_DEPTH_TEST);

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	  
      for (int i = 0; i != num_sprites; ++i) {
		  if (sprites[i].invert) {
			  sprites[i].render(invert_shader_, cameraToWorld);
		  }
		  else {
			  sprites[i].render(texture_shader_, cameraToWorld);
		  }
      }

      char score_text[32];
      sprintf(score_text, "Score: %d\n", frames);
	  draw_text(texture_shader_, -3.0f, frames / 20.0f, 1.0f / 128, score_text);

      vec4 &cpos = cameraToWorld.w();
      alListener3f(AL_POSITION, cpos.x(), cpos.y(), cpos.z());
    }
  };
}
