


//instead of sprites class this class to draw line between two points

namespace octet {

  class lsystems : public app {
    

    mat4t modelToWorld;
    mat4t cameraToWorld;
    std::vector<branch>branches;
    std::vector<mat4t>stack; //for pushing and poping stacks [ and ]
    std::string curInput,curOutput,axiom;
    //std::string constants = "[]+-";
    
    color_shader color_shader_; //initilaze color shader

    float branchLength; //size of branch
    int iterations;//number of iterations
    float angle; //angle for trees
    
     struct rule {
        char head;
        std::string body;

    };
    
    std::vector<rule>rules;
    //std::vector<std::string>rules1;
   // std::string rules;
    



  public:
    
    
    /// this is called when we construct the class before everything is initialised.
    lsystems(int argc, char **argv) : app(argc, argv) {
    }
    
    vec4 treeColour;
    

    //void readfile(std::string filename) {
    //  std::ifstream myFile;
    //  std::string line;
    //  std::string id;

    //  myFile.open(filename);
    //  if (myFile.is_open()) {
    // 
  

    //  std::cout << "reading your file\n";
    //  
    //    while (myFile >> axiom >> rules>>iterations>> angle) {
    //      std::cout << axiom << "," << rules<<","<<iterations<<","<<angle <<std::endl;

    //      std::getline(myFile,axiom);
    //      axiom1=axiom;

    //      std::getline(myFile, rules);
    //      rule2=rules;

    //     
    //     //rules1.push_back(line);

    //    }


    //  }
    //  else {
    //    std::cout << "not open file\n";
    //  }
    //   /*myFile.close();
    //   std::cout<<"file closed\n";*/
    //}
    
 

    void app_init() {
      //initilize the shader
      color_shader_.init();
      treeColour = vec4(0, 1, 0, 1);
 
     
    // readfile("rule1.txt");
 
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


    void makestick() {
      branch stick;
      stick.init(modelToWorld, treeColour, branchLength);
      branches.push_back(stick);
      modelToWorld.translate(0, branchLength, 0);
    }

    void movewithoutdraw() {
      modelToWorld.translate(0, branchLength, 0);
    }

  void evolveTree()  {

    curInput.clear();
    curInput.clear(); 
    curInput = axiom;

    for (int i = 0; i < iterations; i++) {
      for (int j = 0; j < curInput.length(); j++) {

        for (int k = 0; k<rules.size(); k++) {
          if (curInput[j] == rules[k].head)
          {
            curOutput.append(rules[k].body);

          }
        } 
      }
      curInput = curOutput;
      curOutput=""; 
    }
  }

   
    void generateTree() {
     
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

 

  
  //using turtle graphics
   
  //hotkeys
    void hotkey() {
    
      if (is_key_going_down(key_f1)) {
      
 
       // readfile("rule1.txt");
      branches.resize(0);
      rules.resize(0);
      branchLength = 1.0f;
      axiom = "F";
      rules = {
        { 'F',"F[+F]F[-F]F" },
        { '[',"[" },
        { ']',"]" },
        { '+',"+" },
        { '-',"-" }
      };
      angle = 27.5f;
      iterations = 5;
        generateTree();
 
        
      }
      if (is_key_going_down(key_f2)) {
    
        //readfile("rule2.txt");
       branches.resize(0);
       rules.resize(0);
       branchLength = 4.0f;

        axiom = "F";
        rules = {
          { 'F',"F[+F]F[-F][F]" },
          { '[',"[" },
          { ']',"]" }, 
          { '+',"+" },
          { '-',"-" }
        };
        angle = 20.0f;
        iterations = 5;
  
        generateTree();
        

      }
      if (is_key_going_down(key_f3)) {
      
       // readfile("rule3.txt");
        branches.resize(0);
        rules.resize(0);
        branchLength = 4.0f;
        axiom = "F";
        rules = {
          { 'F',"FF-[-F+F+F]+[+F-F-F]" },
          { '[',"[" },
          { ']',"]" },
          { '+',"+" },
          { '-',"-" }
        };
        angle = 22.5f;
        iterations = 4;
 
        generateTree();
       
      }
      if (is_key_going_down(key_f4)) {
      
        //readfile("rule4.txt");
        branches.resize(0);
        rules.resize(0);
        branchLength = 1.0f;
        axiom = "X";
        rules = {
          {'X',"F[+X]F[-X]+X"},
          { 'F',"FF" },
          { '[',"[" },
          { ']',"]" },
          { '+',"+" },
          { '-',"-" }
        };
        angle = 20.0f;
        iterations = 7;

        generateTree();
       
      }
      if (is_key_going_down(key_f5)) {
     
        branches.resize(0);
        rules.resize(0);
        branchLength = 1.0f;
       // readfile("rule5.txt");
        axiom = "X";
        rules = {
          {'X', "F[+X][-X]FX"},
          { 'F',"FF" },
          { '[',"[" },
          { ']',"]" },
          { '+',"+" },
          { '-',"-" }
        };
        angle = 25.7f;
        iterations = 7;
   
        generateTree();
       
      }
      if (is_key_going_down(key_f6)) {

        //readfile("rule6.txt");
        branches.resize(0);
        rules.resize(0);
        branchLength=4.0f;
        axiom = "X";
        rules = {
        {'X',"F-[[X]+X]+F[+FX]-X"},
          { 'F',"FF" },
          { '[',"[" },
          { ']',"]" },
          { '+',"+" },
          { '-',"-" }
        };
        angle = 22.5f;
        iterations = 5;
  
        generateTree();
        
      }
      if (is_key_going_down(key_f7)) {

        //readfile("rule6.txt");
        branches.resize(0);
        rules.resize(0);
        branchLength = 4.0f;
        axiom = "F+F+F+F";
        rules = {
          { 'F',"F+f-FF+F+FF+Ff+FF-f+FF-F-FF-Ff-FFF" },
          {'f',"ffffff"},
          { '[',"[" },
          { ']',"]" },
          { '+',"+" },
          { '-',"-" }
        };
        angle = 90.0f;
        iterations = 2;

        generateTree();

      }

      if (is_key_going_down(key_f8)) {

        //readfile("rule6.txt");
        branches.resize(0);
        rules.resize(0);
        branchLength = 3.0f;
        axiom = "-F";
        rules = {
          { 'F',"F+F-F-F+F" },
          { '[',"[" },
          { ']',"]" },
          { '+',"+" },
          { '-',"-" }
        };
        angle = 90.0f;
        iterations = 4;
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

    /// this is called to draw the world

    void draw_world(int x, int y, int w, int h) {
    
    camera();
    hotkey();
    
    glViewport(x, y, w, h);
    glClearColor(0.2f, 0, 0.3f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (int i = 0; i < branches.size(); i++) {
      branches[i].render(color_shader_, cameraToWorld);
    }
    }
  };
}
