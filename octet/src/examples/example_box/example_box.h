
namespace octet {

  class lsystems : public app {
    
    mat4t modelToWorld;
    mat4t cameraToWorld;
    std::vector<branch>branches;
    std::vector<mat4t>stack; //for pushing and poping stacks [ and ]
    std::string curInput,curOutput,axiom; //string for parameters

    color_shader color_shader_; //initilaze color shader

    float branchLength; //size of branch
    int iterations;//number of iterations
    float angle; //angle for trees
    //rule structure
     struct rule {
        char inChar; //input characters
        std::string inString; //input string
    };
    
    std::vector<rule>rules; //vector for rules
 

  public:
    
    
    /// this is called when we construct the class before everything is initialised.
    lsystems(int argc, char **argv) : app(argc, argv) {
    }
    
    vec4 treeColour;
    
    //reading file
    void readfile(std::string filename) {
      std::ifstream myFile;
      myFile.open(filename);

      if (myFile.is_open()) {
     
      std::cout << "reading your file\n";
      
        while (myFile >> iterations>> angle>>axiom>>branchLength) {
          std::cout <<iterations<<","<<angle <<","<<axiom<<","<<branchLength<< std::endl;
        }
      }
      else {
        std::cout << "not open file\n";
      }
       myFile.close();
       std::cout<<"file closed\n";
    }
    
 
    void app_init() {
      //initilize the shader
      color_shader_.init();
      treeColour = vec4(0, 1, 0, 1); //tree colour
      generateTree();
    }

   
    //push the current state
    void push_stack() {
      stack.push_back(modelToWorld);
    }
    //pop the current state
    void pop_stack() {
    modelToWorld=stack[stack.size()-1];
    stack.pop_back();
    }
    //function for drawing a line
    void makestick() {
      branch stick;
      stick.init(modelToWorld, treeColour, branchLength);
      branches.push_back(stick);
      modelToWorld.translate(0, branchLength, 0);
    }
    //for Koch curve
    void movewithoutdraw() {
      modelToWorld.translate(0, branchLength, 0);
    }
    //loop for rules and next generation
  void evolveTree()  {
    curInput.clear();
    curInput.clear(); 
    curInput = axiom;

    for (int i = 0; i < iterations; i++) {
      for (int j = 0; j < curInput.length(); j++) {

        for (int k = 0; k<rules.size(); k++) {
          if (curInput[j] == rules[k].inChar)
          {
            curOutput.append(rules[k].inString);

          }
        } 
      }
      curInput = curOutput;
      curOutput=""; 
    }
  }

   
    void generateTree() {
     //switch case for input characters
      evolveTree();
      modelToWorld.loadIdentity();
      
      for (int j = 0; j <curInput.length(); j++) {

        switch (curInput[j]) 
        {
        case 'F': makestick();
          break;

        case '+': modelToWorld.rotateZ(angle);
          break;

        case '-': modelToWorld.rotateZ(-angle);
          break;

        case '[': push_stack();
          break;

        case ']': pop_stack();
          break;

         case 'X': makestick();
          break;

          case'f': movewithoutdraw();
          break;
        }

      }
   }


  //hotkeys for trees iterations angle color and length
    void hotkey() {
    
      if (is_key_going_down(key_f1)) {
      
 
      readfile("rule1.txt");
      branches.resize(0);
      rules.resize(0);
      rules = {
        { 'F',"F[+F]F[-F]F" },
        { '[',"[" },
        { ']',"]" },
        { '+',"+" },
        { '-',"-" }
      };

        generateTree();
 
        
      }
      if (is_key_going_down(key_f2)) {
    
        readfile("rule2.txt");
       branches.resize(0);
       rules.resize(0);
        rules = {
          { 'F',"F[+F]F[-F][F]" },
          { '[',"[" },
          { ']',"]" }, 
          { '+',"+" },
          { '-',"-" }
        };
  
        generateTree();
        

      }
      if (is_key_going_down(key_f3)) {
      
        readfile("rule3.txt");
        branches.resize(0);
        rules.resize(0);
        rules = {
          { 'F',"FF-[-F+F+F]+[+F-F-F]" },
          { '[',"[" },
          { ']',"]" },
          { '+',"+" },
          { '-',"-" }
        };
       
        generateTree();
       
      }
      if (is_key_going_down(key_f4)) {
      
        readfile("rule4.txt");
        branches.resize(0);
        rules.resize(0);
        rules = {
          {'X',"F[+X]F[-X]+X"},
          { 'F',"FF" },
          { '[',"[" },
          { ']',"]" },
          { '+',"+" },
          { '-',"-" }
        };
     
        generateTree();
       
      }
      if (is_key_going_down(key_f5)) {
     
        branches.resize(0);
        rules.resize(0);
        readfile("rule5.txt");
        rules = {
          {'X', "F[+X][-X]FX"},
          { 'F',"FF" },
          { '[',"[" },
          { ']',"]" },
          { '+',"+" },
          { '-',"-" }
        };
      
        generateTree();
       
      }
      if (is_key_going_down(key_f6)) {

        readfile("rule6.txt");
        branches.resize(0);
        rules.resize(0);
        rules = {
        {'X',"F-[[X]+X]+F[+FX]-X"},
          { 'F',"FF" },
          { '[',"[" },
          { ']',"]" },
          { '+',"+" },
          { '-',"-" }
        };
       
        generateTree();
        
      }
      if (is_key_going_down(key_f7)) {

        readfile("rule7.txt");
        branches.resize(0);
        rules.resize(0);
        rules = {
          { 'F',"F+f-FF+F+FF+Ff+FF-f+FF-F-FF-Ff-FFF" },
          {'f',"ffffff"},
          { '[',"[" },
          { ']',"]" },
          { '+',"+" },
          { '-',"-" }
        };
       
        generateTree();

      }

      if (is_key_going_down(key_f8)) {

        readfile("rule8.txt");
        branches.resize(0);
        rules.resize(0);
        rules = {
          { 'F',"F+F-F-F+F" },
          { '[',"[" },
          { ']',"]" },
          { '+',"+" },
          { '-',"-" }
        };
   
        generateTree();

      }
      

      if (is_key_down(key_left)) {
        branches.resize(0);
        angle-=1.0f;
        generateTree();
      }

      if (is_key_down(key_right)) {
        branches.resize(0);
        angle += 1.0f;
        generateTree();
      }
      
      if (is_key_going_down(key_w)) {
        branches.resize(0);
        branchLength+=1.0f;
        generateTree();
      }
      if (is_key_going_down(key_s)) {
        branches.resize(0);
        branchLength -= 1.0f;
        generateTree();
      }
      if (is_key_going_down(key_a)) {
        branches.resize(0);
        iterations-=1.0f;
        generateTree();
      }
      if (is_key_going_down(key_d)) {
        branches.resize(0);
        iterations += 1.0f;
        generateTree();
      }
      if (is_key_going_down(key_1)) {
      treeColour=vec4(1,0,0,1);
      generateTree();
      }
      if (is_key_going_down(key_2)) {
        treeColour = vec4(0, 0, 1, 1);
        generateTree();
      }
      if (is_key_going_down(key_3)) {
        treeColour = vec4(1, 1, 1, 1);
        generateTree();
      }

  }
    //set the camera
    void camera() {
      cameraToWorld.loadIdentity();
      cameraToWorld.translate(0, 150.0f, 200.0f);
    }

    // this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
    camera();
    hotkey();
    //the box and background color
    glViewport(x, y, w, h);
    glClearColor(0.2f, 0, 0.3f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //render the branches
    for (int i = 0; i < branches.size(); i++) {
      branches[i].render(color_shader_, cameraToWorld);
    }
    }
  };
}
