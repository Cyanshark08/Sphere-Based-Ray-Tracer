#include <fstream>
#include "Renderer.h"

static uint32_t width = 1920u, height = 1080u;

int test()
{
	std::fstream imageStream;
	imageStream.open("image.ppm", std::ios::out);
	

	imageStream << "P3\n";
	imageStream <<  width << ' ' << height;
	imageStream << "\n255\n";

	XRay::Renderer renderer;
	renderer.Render(width, height, imageStream);

	imageStream.close();

	return 0;
}