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

#ifndef __SRGB_H
#define __SRGB_H

float srgbEncode(float c)
{
	if (c <= 0.0031308f)
	{
		return 12.92f * c; 
	}
	else
	{
		return 1.055f * powf(c, 0.4166667f) - 0.055f; // Inverse gamma 2.4
	}
}

#endif //__SRGB_H
