#pragma once

/**
*	BVH file loader data structure.
*	It's purpose is to load motion capture data from a given file.
*/

#include <fstream>
#include <string>
#include <algorithm>	// std::remove

struct BVHLoader {
	void load(const std::string & fileName);
	void loadHierarchy(const std::istream & file);
	void loadMotion(const std::string & fileName);
};
