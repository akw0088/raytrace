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

#ifndef __RAYTRACE_H
#define __RAYTRACE_H

#include "Def.h"

struct material
{
	color diffuse;
	float reflection;
	color specular;
	float power;
};

struct sphere
{
	point pos;
	float size;
	int materialId;
};

struct light
{
	point pos;
	color intensity;
};

struct ray
{
	point start;
	vec3 dir;
};


#define invsqrtf(x) (1.0f / sqrtf(x))

#endif // __RAYTRACE_H
