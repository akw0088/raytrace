/*
    This file belongs to the Ray tracing tutorial of http://www.codermind.com/
    It is free to use for educational purpose and cannot be redistributed
    outside of the tutorial pages.
    Any further inquiry :
    mailto:info@codermind.com

	codermind.com is now dead and gone, I modified it to work in the winapi and quazi realtime.
	The original pages are still available through the wayback machine: 
		http://web.archive.org/web/20131113102238/http://www.codermind.com/

		source code was named as follows:
			raytrace_en_page1.rar
			raytrace_en_page2.rar
			raytrace_en_page3.rar
			raytrace_en_page4.rar
*/


#ifndef __SCENE_H
#define __SCENE_H
#include <vector>
#include "raytrace.h"

struct scene
{
	std::vector<material> materialContainer;
	std::vector<sphere>   sphereContainer;
	std::vector<light>    lightContainer;
	std::vector<plane>    planeContainer;
	int sizex, sizey;
};

bool init(char* inputName, scene &myScene);

#endif // __SCENE_H
