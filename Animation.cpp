#include "Animation.h"

/**
*	Constructor handling the creation of member variables.
*/
Animation::Animation(void) {
	animationInfo = new BVHInfo();
	skeleton = new Skeleton();
}

Animation::~Animation(void) {
	delete animationInfo;
	delete skeleton;
}