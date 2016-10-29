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
	  material *yellow = new material(vec4(1, 1, 0, 1));
	  material *mehblue = new material(vec4(0, 1, 1, 1));
	  

	  mat4t mat;
	  //banks
	  mat.loadIdentity();
	  mat.translate(14, 0, 1);
	  mesh_instance* meshA = app_scene->add_shape(mat, new mesh_box(vec3(5, 2, 200)), mehblue, false);
	  btRigidBody *rbA = meshA->get_node()->get_rigid_body();

	  mat.loadIdentity();
	  mat.translate(-14, 0, 1);
	  mesh_instance* meshB = app_scene->add_shape(mat, new mesh_box(vec3(5, 2,200)), mehblue, false);
	  btRigidBody *rbB = meshB->get_node()->get_rigid_body();

	//bridge
	  mat.loadIdentity();
	  mat.translate(-5, 4, 1);
	  mesh_instance* mesh1 = app_scene->add_shape(mat, new mesh_box(vec3(4.2f, 0.1f, 2)), red, true);
	  btRigidBody *rb1 = mesh1->get_node()->get_rigid_body();

	  mat.loadIdentity();
	  mat.translate(5, 4, 1);
	  mesh_instance* mesh2 = app_scene->add_shape(mat, new mesh_box(vec3(4.2f, 0.1f, 2)), yellow, true);
	  btRigidBody *rb2 = mesh2->get_node()->get_rigid_body();

      // ground
      mat.loadIdentity();
      mat.translate(0, -3, 0);
	  mesh_instance* meshC=app_scene->add_shape(mat, new mesh_box(vec3(200, 1, 200)), green, false);
	  btRigidBody *rbC = meshC->get_node()->get_rigid_body();

	  //constraints
	/* btHingeConstraint*hinge12;
	 btVector3 axisA(1.0f, 0.0f, 0.0f);
	 btVector3 axisB(1.0f, 0.0f, 0.0f);
	 btVector3 pivot1(10.0f, 0.0f, 0.0f);
	 btVector3 pivot2(7.0f, 0.0f, 0.0f);
	 hinge12 = new btHingeConstraint(*rb1, *rb2,pivot1,pivot2, axisA, axisB);
	 hinge12->setLimit(-SIMD_PI , SIMD_PI );
	 app_scene->get_world()->addConstraint(hinge12, true);*/

	 btHingeConstraint*hinge1B;
	 btVector3 axis1B(1.0f, 0.0f, 0.0f);
	 btVector3 axis1(1.0f, 0.0f, 0.0f);
	 btVector3 pivotB(15.0f, 0.0f, 0.0f);
	 btVector3 pivot11(5.0f, 0.0f, 0.0f);
	 hinge1B = new btHingeConstraint(*rbB, *rb1, pivotB, pivot11, axis1B, axis1);
	 hinge1B->setLimit(-SIMD_PI, SIMD_PI);
	 app_scene->get_world()->addConstraint(hinge1B, true);

	 btHingeConstraint*hinge2A;
	 btVector3 axis2A(1.0f, 0.0f, 0.0f);
	 btVector3 axis2(1.0f, 0.0f, 0.0f);
	 btVector3 pivot2A(5.0f, 0.0f, 0.0f);
	 btVector3 pivot22(15.0f, 0.0f, 0.0f);
	 hinge2A = new btHingeConstraint(*rbA, *rb2, pivot2A, pivot22, axis2A, axis2);
	 hinge2A->setLimit(-SIMD_PI, SIMD_PI);
	 app_scene->get_world()->addConstraint(hinge2A, true);


	 //mat.loadIdentity();
	 //mat.translate(-14, 6, 1);
	 //mesh_instance* meshF = app_scene->add_shape(mat, new mesh_sphere(vec3(2, 2, 2), 2), blue, true);
	 //btRigidBody *rbF = meshF->get_node()->get_rigid_body();

	


	 btTransform frameInA, frameInB;
	 frameInA = btTransform::getIdentity();
	 frameInA.setOrigin(btVector3(btScalar(0.), btScalar(0.), btScalar(0.)));
	 frameInB = btTransform::getIdentity();
	 frameInB.setOrigin(btVector3(btScalar(0.), btScalar(0.), btScalar(0.)));

	 btGeneric6DofSpringConstraint* pGen6DOFSpring;
	 pGen6DOFSpring = new btGeneric6DofSpringConstraint(*rb1, *rbC, frameInA, frameInB, true);
	 pGen6DOFSpring->setLinearUpperLimit(btVector3(0., 10., 0.));
	 pGen6DOFSpring->setLinearLowerLimit(btVector3(0., -10., 0.));

	 pGen6DOFSpring->setAngularLowerLimit(btVector3(0.f, -1.5f, 0.0f));
	 pGen6DOFSpring->setAngularUpperLimit(btVector3(0.f, 1.5f, 0.0f));

	 app_scene->get_world()->addConstraint(pGen6DOFSpring, true);
	 pGen6DOFSpring->setDbgDrawSize(btScalar(5.f));

	 pGen6DOFSpring->enableSpring(0, true);
	 pGen6DOFSpring->setStiffness(0, 39.478f);
	 pGen6DOFSpring->setDamping(0, 0.5f);
	 //pGen6DOFSpring->enableSpring(0, true);
	 //pGen6DOFSpring->setStiffness(0, 39.478f);
	 //pGen6DOFSpring->setDamping(0, 0.5f);
	 pGen6DOFSpring->setEquilibriumPoint();

	 btTransform frameInD, frameInE;
	 frameInD = btTransform::getIdentity();
	 frameInD.setOrigin(btVector3(btScalar(0.), btScalar(0.), btScalar(0.)));
	 frameInE = btTransform::getIdentity();
	 frameInE.setOrigin(btVector3(btScalar(0.), btScalar(0.), btScalar(0.)));

	 btGeneric6DofSpringConstraint* bGen6DOFSpring;
	 bGen6DOFSpring = new btGeneric6DofSpringConstraint(*rb2, *rbC, frameInD, frameInE, true);
	 bGen6DOFSpring->setLinearUpperLimit(btVector3(5., 0., 0.));
	 bGen6DOFSpring->setLinearLowerLimit(btVector3(1., 0., 0.));

	 bGen6DOFSpring->setAngularLowerLimit(btVector3(0.f, 0.f, 0.0f));
	 bGen6DOFSpring->setAngularUpperLimit(btVector3(0.f, 0.f, 0.0f));

	 app_scene->get_world()->addConstraint(bGen6DOFSpring, true);
	 bGen6DOFSpring->setDbgDrawSize(btScalar(5.f));

	 bGen6DOFSpring->enableSpring(0, true);
	 bGen6DOFSpring->setStiffness(0, 39.478f);
	 bGen6DOFSpring->setDamping(0, 0.5f);
	 bGen6DOFSpring->enableSpring(0, true);
	 bGen6DOFSpring->setStiffness(0, 39.478f);
	 bGen6DOFSpring->setDamping(0, 0.5f);
	 bGen6DOFSpring->setEquilibriumPoint();

	
	 ///////////////////////////////////////////////////////////////////////////////
	 mat.loadIdentity();
	 mat.translate(0, 6, 1);
	 mesh_instance* meshF = app_scene->add_shape(mat, new mesh_sphere(vec3(2, 2, 2), 2), blue, true);
	 btRigidBody *rbF = meshF->get_node()->get_rigid_body();


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
