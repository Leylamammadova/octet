////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//
namespace octet {
  /// Scene containing a box with octet.
  class example_shapes : public app {
    // scene for drawing box
    ref<visual_scene> app_scene;

  public:
    example_shapes(int argc, char **argv) : app(argc, argv) {
    }

    ~example_shapes() {
    }
	
	
    /// this is called once OpenGL is initialized
    void app_init() {
      app_scene =  new visual_scene();
      app_scene->create_default_camera_and_lights();
      app_scene->get_camera_instance(0)->get_node()->translate(vec3(0, 8, 0));


	

      material *red = new material(vec4(1, 0, 0, 1));
      material *green = new material(vec4(0, 1, 0, 1));
      material *blue = new material(vec4(0, 0, 1, 1));

	  mat4t mat;
	  mat.loadIdentity();
	  mat.translate(6, 4, 1);
	  mesh_instance* meshA = app_scene->add_shape(mat, new mesh_cylinder(zcylinder(vec3(0, 0, 0), 0.8f, 2.0f)), red, true);
	  btRigidBody *rbA = meshA->get_node()->get_rigid_body();
	  
	  mat.loadIdentity();
	  mat.translate(4, 4, 1);
	  mesh_instance* meshB= app_scene->add_shape(mat, new mesh_cylinder(zcylinder(vec3(0, 0, 0), 0.8f, 2.0f)), red, true);
	  btRigidBody *rbB = meshB->get_node()->get_rigid_body();
	  
	  btHingeConstraint* hingeAB;

	  btVector3 axisA(1.0f, 0.f, 0.0f);
	  btVector3 axisB(1.0f, 0.f, 0.0f);
	  btVector3 pivotA(-1.0f, 0.0f, 0.0f);
	  btVector3 pivotB(1.0f, 0.0f, 0.0f);
	  hingeAB = new btHingeConstraint(*rbA, *rbB, pivotA, pivotB, axisA, axisB);
	  hingeAB->setLimit(-SIMD_HALF_PI * 1.0f, SIMD_HALF_PI * 1.0f);
	  // add constraint to world
	  app_scene->get_world()->addConstraint(hingeAB, true);
	  // draw constraint frames and limits for debugging
	 // spHingeDynAB->setDbgDrawSize(btScalar(5.f));




	  mat.loadIdentity();
	  mat.translate(8, 0, 0);
	  mat.rotateX90();
	  app_scene->add_shape(mat, new mesh_cylinder(zcylinder(vec3(0, 0, 0), 0.8f, 3.0f)), blue, true);

	  mat.loadIdentity();
	  mat.translate(8, 0, 4);
	  mat.rotateX90();
	  app_scene->add_shape(mat, new mesh_cylinder(zcylinder(vec3(0, 0, 0), 0.8f, 3.0f)), blue, true);

	  mat.loadIdentity();
	  mat.translate(-8, 0, 0);
	  mat.rotateX90();
	  app_scene->add_shape(mat, new mesh_cylinder(zcylinder(vec3(0, 0, 0), 0.8f, 3.0f)), blue, true);

	  mat.loadIdentity();
	  mat.translate(-8, 0, 4);
	  mat.rotateX90();
	  app_scene->add_shape(mat, new mesh_cylinder(zcylinder(vec3(0, 0, 0), 0.8f, 3.0f)), blue, true);

	  
      // ground
      mat.loadIdentity();
      mat.translate(0, -1, 0);
      app_scene->add_shape(mat, new mesh_box(vec3(200, 1, 200)), green, false);
    }


    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      app_scene->begin_render(vx, vy);

      // update matrices. assume 30 fps.
      app_scene->update(1.0f/30);

      // draw the scene
      app_scene->render((float)vx / vy);
    }

  };
   
}
