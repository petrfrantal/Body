/**
*	Definitions of BVHLoader methods.
*/

#include "BVHLoader.h"

void BVHLoader::loadMotion(const std::istream & file) {

}

Joint * BVHLoader::loadJoint(const std::istream & file, Joint * parent = NULL) {
	Joint * joint = new Joint();
	joint->parent = parent;

}

void BVHLoader::loadHierarchy(const std::istream & file, Animation * animation) {
	std::string line;
	while (file.good()) {
		if (line.find("ROOT") != std::string::npos) {
			animation->skeleton->root = loadJoint(file);
			//animation->skeleton->root->name = "";
		} else if (line.find("MOTION") != std::string::npos) {
			loadMotion(file);
		}
	} 
}

Animation * BVHLoader::loadAnimation(const std::string & fileName) {
	std::ifstream file(fileName);
	if (file.is_open()) {
		std::string line;
		file >> line;
		if (line.find("HIERARCHY") != std::string::npos) {  // test whether the file starts with HIERARCHY keyword
			Animation * animation = new Animation();
			loadHierarchy(file, animation);
			file.close();
			return animation;
		} else {
			std::cout << "ERROR: Selected BVH file does not contain the keyword HIERARCHY in the beginning" << std::endl;
			file.close();
			return NULL;
		}
	}
}