


//instead of sprites class i ve created this class to draw line between two points

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

        glVertexAttribPointer(attribute_pos, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)points);
        glEnableVertexAttribArray(attribute_pos);

        glDrawArrays(GL_LINES, 0, 2);
    }
  };
  class lsystems : public app {
    // scene for drawing box

    mat4t modelToWorld;
    mat4t cameraToWorld;
    std::vector<branch>branches;
    


    color_shader color_shader_; //initilaze color shader

    float branchLength; //size of branch
    int iterations;//number of iterations
    float angle; //angle for trees
    std::string beginIter;//begin iterations 
    std::string endIter;//end iterations
  

  public:
    
    
    /// this is called when we construct the class before everything is initialised.
    lsystems(int argc, char **argv) : app(argc, argv) {
    }
    
    vec4 treeColour;
    
 
  

    /// this is called once OpenGL is initialized
 
    void readfile(std::string filename) {
      std::ifstream myFile;
      std::string line;
      myFile.open(filename);
      if(myFile.is_open()){
      while (std::getline(myFile, line)) {
      std:: cout<<"reading file";
        }
      }else std::cout<<"not open file";
    }

    void app_init() {
    //initilize the shader
    color_shader_.init();
    branchLength = 5;
    treeColour = vec4(0,1,0,1);
    
    makestick();

    }

    void makestick() {
      
      modelToWorld.loadIdentity();
      branch stick;
      stick.init(modelToWorld, (0, 1, 0, 1), branchLength);
      branches.push_back(stick);
    }
  
  //using turtle graphics
    void generateTree() {

     /* for (int i = 0; i <  ; i++) {
        switch ([i])
        {
        case 'F': makestick();
          break;

        case '+': modelToWorld.rotateZ(angle);
          break;

        case '-': modelToWorld.rotateZ(-angle);
          break;

        case '[': 
          break;

        case ']':
          break;

        case 'X':
          break;
          }*/
   }
  //hotkeys
    void hotkey() {
      if (is_key_going_down(key_f1)) {
     // cameraToWorld.loadIdentity();
        readfile("rule1.txt");
        angle=25.7f;
        generateTree();
        
      }
      if (is_key_going_down(key_f2)) {
       // cameraToWorld.loadIdentity();
        readfile("rule2.txt");
        angle=20.0f;
        generateTree();

      }
      if (is_key_going_down(key_f3)) {
      //  cameraToWorld.loadIdentity();
        readfile("rule3.txt");
        angle=22.5f;
        generateTree();
      }
      if (is_key_going_down(key_f4)) {
      //  cameraToWorld.loadIdentity();
        readfile("rule4.txt");
        angle=20.0f;
        generateTree();
      }
      if (is_key_going_down(key_f5)) {
      //  cameraToWorld.loadIdentity();
        readfile("rule5.txt");
        angle=25.7f;
        generateTree();
      }
      if (is_key_going_down(key_f6)) {
     //   cameraToWorld.loadIdentity();
        readfile("rule6.txt");
        angle=22.5f;
        generateTree();
      }
      
  }
    //set the camera
    void camera() {
      cameraToWorld.loadIdentity();
      cameraToWorld.translate(0, 0, 20);
    }
    /// this is called to draw the world

    void draw_world(int x, int y, int w, int h) {
     
     
    hotkey();
    camera();
    glViewport(x, y, w, h);
    glClearColor(0.2f, 0, 0.3f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (int i = 0; i < branches.size(); i++) {
      branches[i].render(color_shader_, cameraToWorld);
    }
    }
  };
}
