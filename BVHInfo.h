#pragma once

/**
*	Auxiliary data structure containing information about used BVH file.
*/

/**
*	The class containing the information.
*/
struct BVHInfo {
	unsigned int jointCount;
	unsigned int frameCount;
	float frameDurationMS;			// in milliseconds
	float framesPerSecond;
};