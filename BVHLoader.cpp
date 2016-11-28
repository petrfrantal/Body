/**
*	Definitions of BVHLoader methods.
*/

#include "BVHLoader.h"

void BVHLoader::loadHierarchy(const std::istream & file) {

}

void BVHLoader::load(const std::string & fileName) {
	std::ifstream file(fileName);
	if (file.is_open()) {
		std::string line;
		file >> line;
		std::remove(line.begin(), line.end(), ' ');
		if (line == "HIERARCHY") {
			loadHierarchy(file);
		}
		file.close();
	}
}