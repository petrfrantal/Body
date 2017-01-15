#pragma once

#include <GL/glew.h>
#include <iostream>

class Slider
{
public:
	Slider();
	void drawSlider(int windowWidth, int windowHeight, int frame, int frameCount);
	int clickSlider(int x, int y);
	int dragSlider(int x);
	~Slider();
private:
	int backgroundLeft, backgroundRight, backgroundTop, backgroundBottom, numOfFrames;
	float pixelsPerFrame;
};

