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
		app_scene = new visual_scene();
		app_scene->create_default_camera_and_lights();
		app_scene->get_camera_instance(0)->get_node()->translate(vec3(0, 8, 0));

		material *red = new material(vec4(1, 0, 0, 1));
		material *green = new material(vec4(0, 1, 0, 1));
		material *blue = new material(vec4(0, 0, 1, 1));
		material *yellow = new material(vec4(1, 1, 0, 1));
		material *mehblue = new material(vec4(0, 1, 1, 1));

		mat4t mat;
	     
     	mat.loadIdentity();
		mat.translate(-7, 2.2f, 1);
		mat.rotate(15.0f, 0, 0, 1);
		mesh_instance* mesh1 = app_scene->add_shape(mat, new mesh_box(vec3(1.2f, 0.1f, 3)), red, false);
		btRigidBody *plank1 = mesh1->get_node()->get_rigid_body();
	
	
		mat.loadIdentity();
		mat.translate(-4, 4, 1);
		mesh_instance* mesh2 = app_scene->add_shape(mat, new mesh_box(vec3(1.2f, 0.1f, 3)), yellow, true);
		btRigidBody *plank2 = mesh2->get_node()->get_rigid_body();

		mat.loadIdentity();
		mat.translate(-1, 4, 1);
		mesh_instance* mesh3 = app_scene->add_shape(mat, new mesh_box(vec3(1.2f, 0.1f, 3)), yellow, true);
		btRigidBody *plank3 = mesh3->get_node()->get_rigid_body();

		mat.loadIdentity();
		mat.translate(2, 4, 1);
		mesh_instance* mesh4 = app_scene->add_shape(mat, new mesh_box(vec3(1.2f, 0.1f, 3)), yellow, true);
		btRigidBody *plank4 = mesh4->get_node()->get_rigid_body();

		mat.loadIdentity();
		mat.translate(5, 4, 1);
		mesh_instance* mesh5 = app_scene->add_shape(mat, new mesh_box(vec3(1.2f, 0.1f, 3)), yellow, true);
		btRigidBody *plank5 = mesh5->get_node()->get_rigid_body();
		

		mat.loadIdentity();
		mat.translate(8, 2.2f, 1);
		mat.rotate(-15.0f, 0, 0, 1);
		mesh_instance* mesh6 = app_scene->add_shape(mat, new mesh_box(vec3(1.2f, 0.1f, 3)), red, false);
		btRigidBody *plank6 = mesh6->get_node()->get_rigid_body();

	//point2pointconstraint

		btPoint2PointConstraint* leftSpring = new btPoint2PointConstraint(*plank1, *plank2, btVector3(1.5f, 1.5f, 0), btVector3(-1.5f, 2.0f, 0));
		app_scene->get_world()->addConstraint(leftSpring);

		btPoint2PointConstraint* rightSpring = new btPoint2PointConstraint(*plank6, *plank5, btVector3(-1.5f, 1.5f, 0), btVector3(1.5f, 2.0f, 0));
		app_scene->get_world()->addConstraint(rightSpring);

		btPoint2PointConstraint* leftSpring1 = new btPoint2PointConstraint(*plank2, *plank3, btVector3(1.5f, 1.0f, 0), btVector3(-1.5f, 1.0f, 0));
		app_scene->get_world()->addConstraint(leftSpring1);

		btPoint2PointConstraint* rightSpring1 = new btPoint2PointConstraint(*plank5, *plank4, btVector3(-1.5f, 1.0f, 0), btVector3(1.5f, 1.0f, 0));
		app_scene->get_world()->addConstraint(rightSpring1);


	/*	 btHingeConstraint*hinge;
		 btVector3 axisA(0.0f, 1.0f, 0.0f);
		 btVector3 axisB(0.0f, 1.0f, 0.0f);
		 btVector3 pivotA(5.0f, 0.0f, 0.0f);
		 btVector3 pivotB(1.0f, 2.0f, 0.0f);
		 hinge = new btHingeConstraint(*rbF, *plank6, pivotA, pivotB, axisA, axisB);
		 hinge->setLimit(-SIMD_PI*0.5f, SIMD_PI*0.5f);
		 app_scene->get_world()->addConstraint(hinge, true);
*/
	
		 //spring
		btTransform frameInA, frameInB;
		frameInA = btTransform::getIdentity();
		frameInA.setOrigin(btVector3(btScalar(0.), btScalar(0.), btScalar(0.)));
		frameInB = btTransform::getIdentity();
		frameInB.setOrigin(btVector3(btScalar(0.), btScalar(0.), btScalar(0.)));

		btGeneric6DofSpringConstraint* pGen6DOFSpring;
		pGen6DOFSpring = new btGeneric6DofSpringConstraint(*plank3, *plank4, frameInA, frameInB, true);
		pGen6DOFSpring->setLinearUpperLimit(btVector3(3.0f, 0.01f, 0.01f));
		pGen6DOFSpring->setLinearLowerLimit(btVector3(3.0f, 0.01f, 0.01f));
		//disable rotations
		pGen6DOFSpring->setAngularLowerLimit(btVector3(0, 0, -0.1f));
		pGen6DOFSpring->setAngularUpperLimit(btVector3(0, 0, -0.1f));

		app_scene->get_world()->addConstraint(pGen6DOFSpring, true);
		pGen6DOFSpring->setDbgDrawSize(btScalar(5.f));

		pGen6DOFSpring->enableSpring(5, true);
		pGen6DOFSpring->setStiffness(0, 39.478f);
		pGen6DOFSpring->setDamping(0, 0.0f);
		pGen6DOFSpring->enableSpring(0, true);
		pGen6DOFSpring->setStiffness(0, 39.478f);
		pGen6DOFSpring->setDamping(0, 0.0f);
		pGen6DOFSpring->setEquilibriumPoint();
		
		//static banks
		mat.loadIdentity();
		mat.translate(14, 0, 1);
		mesh_instance* meshA = app_scene->add_shape(mat, new mesh_box(vec3(5, 2, 200)), mehblue, false);
		btRigidBody *rbA = meshA->get_node()->get_rigid_body();

		mat.loadIdentity();
		mat.translate(-14, 0, 1);
		mesh_instance* meshB = app_scene->add_shape(mat, new mesh_box(vec3(5, 2, 200)), mehblue, false);
		btRigidBody *rbB = meshB->get_node()->get_rigid_body();
		//ground
		mat.loadIdentity();
		mat.translate(0, -3, 0);
		mesh_instance* meshC = app_scene->add_shape(mat, new mesh_box(vec3(200, 1, 200)), green, false);
		btRigidBody *rbC = meshC->get_node()->get_rigid_body();

		 mat.loadIdentity();
		 mat.translate(-10, 6, 0);
		 mesh_instance* meshF = app_scene->add_shape(mat, new mesh_sphere(vec3(2, 2, 2), 2), blue, true);
		 btRigidBody *rbF = meshF->get_node()->get_rigid_body();
		 

	}


	

    void draw_world(int x, int y, int w, int h) {
      	
	  int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      app_scene->begin_render(vx, vy);
      app_scene->update(1.0f/30);
      app_scene->render((float)vx / vy);
    }

  };
   
}
