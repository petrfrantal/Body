#pragma once

/**
*	Auxiliary data structure containing information about used BVH file.
*/

struct BVHInfo {
	int jointCount;
	long frameCount;
	int framesPerSecond;
	long currentFrame;
};