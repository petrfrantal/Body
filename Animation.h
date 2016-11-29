#pragma once

/**
*	Defines the data structures related to playing the animation loaded from a BVH file.
*/

#include "BVHInfo.h"
#include "Skeleton.h"

/**
*	Animation object. Introduces the API for playing the animation.
*	Also serves as a container object holding all the animation related informations.
*/
struct Animation {
	BVHInfo * animationInfo;
	Skeleton * skeleton;

	void playAnimation(long frame);
	void goToFrame(long frame);		// this method maybe will not be necessary
};