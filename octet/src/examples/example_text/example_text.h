////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//
namespace octet {
  /// Scene containing a box with octet.
  class example_text : public app {
    ref<visual_scene> app_scene;
    ref<text_overlay> overlay;
    ref<mesh_text> text;
  public:
    float TreeAngle;
    float TreeRotation;
    float BranchLenght;

    int numIterations;
    int numBranches;
    int numGrow;

    std::vector<std::string>growVector;

    /// this is called when we construct the class before everything is initialised.
    example_text(int argc, char **argv) : app(argc, argv) {
    }

    void app_init() {
      app_scene =  new visual_scene();
      app_scene->create_default_camera_and_lights();
      
      scene_node *node = new scene_node();
      material *red = new material(vec4(1, 0, 0, 1));
      mesh_box *box = new mesh_box(vec3(0.1f,1,0.1f));
      
      app_scene->add_child(node);
      app_scene->add_mesh_instance(new mesh_instance(node, box, red));
      overlay = new text_overlay();
      bitmap_font *font = overlay->get_default_font();
      aabb bb(vec3(64.5f, -200.0f, 0.0f), vec3(256, 64, 0));
      text = new mesh_text(font, "", &bb);
      
      // add the mesh to the overlay.
      overlay->add_mesh_text(text);
      
     
      //turtle interpolation
   // turtle();
  
    
    }

   // void turtle() {
   //// taken from book algoritmic beauty of plants
   //   vec3 startpoint = vec3(0);
   //   vec3 nextpoint;
   //   TreeAngle=20;
   //   nextpoint.x()= startpoint.x()+BranchLenght*cos((TreeAngle+90)*CL_M_PI/180); //alpha is facing angle
   //   nextpoint.y()= startpoint.y()+BranchLenght*sin((TreeAngle + 90)*CL_M_PI / 180);
   //   
   //   mat4t mat;
   //   mat.loadIdentity();
   //   mat.translate(nextpoint);
   //   mat.rotate(TreeAngle,0,0,0);

   //   mesh_box *box = new mesh_box(vec3(0.1f, 1, 0.1f));
   //   app_scene->add_child(node);
   //   app_scene->add_mesh_instance(new mesh_instance(node, box, red));

   // }
    //taken from tinyxml tutorials
   /* void loadxml() {
      TiXmlDocument doc();
    }*/
  
    //void dump_to_stdout(char* pFilename )
    //{
    //  TiXmlDocument doc(pFilename);
    //  bool loadOkay = doc.LoadFile();
    //  if (loadOkay)
    //  {
    //    printf("\n%s:\n", pFilename);
    //    dump_to_stdout( pFilename); // defined later in the tutorial
    //  }
    //  else
    //  {
    //    printf("Failed to load file \"%s\"\n", pFilename);
    //  }
    //}

    void hotkey() {
    //ad hot keys of changing parameters
      if (is_key_going_down(key_f1)) {
        TiXmlDocument doc("tree1.xml");
        doc.LoadFile();
        bool loadOkay=doc.LoadFile();
        if (loadOkay) {
          printf("\n%s:\n", "tree1.xml");
        }
        else {
          printf("Failed to load file \"%s\"\n", "tree1.xml");
        }
    }
      if (is_key_going_down(key_f2)) {
        TiXmlDocument doc("tree2.xml");
        doc.LoadFile();
        bool loadOkay = doc.LoadFile();
        if (loadOkay) {
          printf("\n%s:\n", "tree2.xml");
        }
        else {
          printf("Failed to load file \"%s\"\n", "tree2.xml");
        }
      }
      if (is_key_going_down(key_f3)) {
        TiXmlDocument doc("tree1.xml");
        doc.LoadFile();
        bool loadOkay = doc.LoadFile();
        if (loadOkay) {
          printf("\n%s:\n", "tree3.xml");
        }
        else {
          printf("Failed to load file \"%s\"\n", "tree3.xml");
        }
      }
      if (is_key_going_down(key_f4)) {
        TiXmlDocument doc("tree4.xml");
        doc.LoadFile();
        bool loadOkay = doc.LoadFile();
        if (loadOkay) {
          printf("\n%s:\n", "tree4.xml");
        }
        else {
          printf("Failed to load file \"%s\"\n", "tree4.xml");
        }
      }
      if (is_key_going_down(key_f5)) {
        TiXmlDocument doc("tree5.xml");
        doc.LoadFile();
        bool loadOkay = doc.LoadFile();
        if (loadOkay) {
          printf("\n%s:\n", "tree5.xml");
        }
        else {
          printf("Failed to load file \"%s\"\n", "tree5.xml");
        }
      }
      if (is_key_going_down(key_f6)) {
        TiXmlDocument doc("tree6.xml");
        doc.LoadFile();
        bool loadOkay = doc.LoadFile();
        if (loadOkay) {
          printf("\n%s:\n", "tree6.xml");
        }
        else {
          printf("Failed to load file \"%s\"\n", "tree6.xml");
        }
      }
    }

    void draw_trees() {
    //render trees
    }


    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      app_scene->begin_render(vx, vy);
      
      hotkey();

      // update matrices. assume 30 fps.
      app_scene->update(1.0f/30);

      // draw the scene
      app_scene->render((float)vx / vy);

      // tumble the box  (there is only one mesh instance)
      scene_node *node = app_scene->get_mesh_instance(0)->get_node();
      

      // write some text to the overlay
      char buf[3][256];
      const mat4t &mx = node->access_nodeToParent();

      text->clear();

      text->format(
        "rules: %s\n"
        "angle: %s\n"
        "iterations: %s\n",
        mx.x().toString(buf[0], sizeof(buf[0])),
        mx.y().toString(buf[1], sizeof(buf[1])),
        mx.z().toString(buf[2], sizeof(buf[2]))
      );

      // convert it to a mesh.
      text->update();

      // draw the text overlay
      overlay->render(vx, vy);
    }
  };
}
