#pragma once

/**
*	Auxiliary data structure containing information about used BVH file.
*/

struct BVHInfo {
	unsigned int jointCount;
	unsigned int frameCount;
	float frameDuration;
	float framesPerSecond;
	int currentFrame;
};