#pragma once

/**
*	Defines the data structures related to playing the animation loaded from a BVH file.
*/

#include "BVHInfo.h"
#include "Skeleton.h"

/**
*	Animation object. Contains the skeleton object and a informational object about the animation.
*/
struct Animation {
	BVHInfo * animationInfo;		// informations about the animation (frame duration, etc.)
	Skeleton * skeleton;			// skeleton object with joints and transformation matrices
	Animation(void);
};