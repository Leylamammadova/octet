



namespace octet {
  class branch {
    mat4t modelToWorld;
    float branchLength;
    vec4 colour;

    public: 
      void init(mat4t mat, vec4 branchColour, float length) {
        modelToWorld = mat;
        colour= branchColour;
        branchLength= length;
    }
    //old school rendering for the branch
      void render(color_shader&shader, mat4t &cameraToWorld) {
        mat4t modelToProjection = mat4t::build_projection_matrix(modelToWorld, cameraToWorld);

        shader.render(modelToProjection, colour);

        float points[] = {
          0, 0, 0,
          0, branchLength, 0,
        };

        glVertexAttribPointer(attribute_pos, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)points);
        glEnableVertexAttribArray(attribute_pos);

        glDrawArrays(GL_LINES, 0, 2);
    }
  };
  class lsystems : public app {
    // scene for drawing box

    mat4t modelToWorld;
    mat4t cameraToWorld;

    color_shader color_shader_;

    float branchLength;


  public:
    /// this is called when we construct the class before everything is initialised.
    lsystems(int argc, char **argv) : app(argc, argv) {
    }
    dynarray<branch>branches;

    /// this is called once OpenGL is initialized
    void app_init() {
    //initilize the shader
    color_shader_.init();

    branchLength= 4;


    }
    void hot_keys() {

    }
    //set the camera
    void camera() {
      cameraToWorld.loadIdentity();
      cameraToWorld.translate(0, 200, 200);
    }
    /// this is called to draw the world

    void draw_world(int x, int y, int w, int h) {
      
      for (int i = 0; i < branches.size(); i++) {
        branches[i].render(color_shader_, cameraToWorld);
      }

    camera();
    hot_keys();
    glViewport(x, y, w, h);
    glClearColor(0, 0, 0.3f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    }
  };
}
