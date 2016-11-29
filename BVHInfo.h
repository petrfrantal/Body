#pragma once

/**
*	Auxiliary data structure containing information about used BVH file.
*/

struct BVHInfo {
	unsigned int jointCount;
	int frameCount;
	float framesPerSecond;
	int currentFrame;
};