

#include "skeleton.h"


CG_GileSkeleton::CG_GileSkeleton()
{
	//order of makeJoint calls is very important
	makeJoint("shoulders", Vector3(0.53, 0.87, 0.5));                          //0
	makeJoint("back", Vector3(0.48, 0.53, 0.5), "shoulders");       //1
	makeJoint("hips", Vector3(0.5, 0.5, 0.45), "back");            //2
	makeJoint("head", Vector3(0.54, 1.38, 0.5), "shoulders");       //3

	makeJoint("lthigh", Vector3(0.37, 0.52, 0.5), "hips");            //4
	makeJoint("lknee", Vector3(0.37, 0.27, 0.53), "lthigh");          //5
	makeJoint("lankle", Vector3(0.38, 0.07, 0.53), "lknee");           //6
	makeJoint("lfoot", Vector3(0.4, 0.03, 0.5), "lankle");          //7

	makeJoint("rthigh", Vector3(0.63, 0.52, 0.5), "hips");            //8
	makeJoint("rknee", Vector3(0.63, 0.27, 0.53), "rthigh");          //9
	makeJoint("rankle", Vector3(0.62, 0.07, 0.53), "rknee");           //10
	makeJoint("rfoot", Vector3(0.6, 0.03, 0.5), "rankle");          //11

	makeJoint("lshoulder", Vector3(0.48, 0.87, 0.5), "shoulders");       //12
	makeJoint("lelbow", Vector3(0.21, 0.66, 0.5), "lshoulder");       //13
	makeJoint("lhand", Vector3(0.29, 0.5, 0.47), "lelbow");          //14

	makeJoint("rshoulder", Vector3(0.68, 0.87, 0.5), "shoulders");       //15
	makeJoint("relbow", Vector3(0.77, 0.65, 0.48), "rshoulder");       //16
	makeJoint("rhand", Vector3(0.78, 0.43, 0.52), "relbow");          //17

	//symmetry
	makeSymmetric("lthigh", "rthigh");
	makeSymmetric("lknee", "rknee");
	makeSymmetric("lankle", "rankle");
	makeSymmetric("lfoot", "rfoot");

	makeSymmetric("lshoulder", "rshoulder");
	makeSymmetric("lelbow", "relbow");
	makeSymmetric("lhand", "rhand");

	initCompressed();

	setFoot("lfoot");
	setFoot("rfoot");

	setFat("hips");
	setFat("shoulders");
	setFat("head");
}