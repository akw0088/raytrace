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


#ifndef __DEF_H
#define __DEF_H

const double PIOVER180 = 0.017453292519943295769236907684886;

struct point
{
	float x, y, z;
};

struct vec3
{
	float x, y, z;

    vec3& operator += (const vec3 &v2){
	    this->x += v2.x;
        this->y += v2.y;
        this->z += v2.z;
	    return *this;
    }
};

inline point operator + (const point&p, const vec3 &v){
	point p2={p.x + v.x, p.y + v.y, p.z + v.z };
	return p2;
}

inline point operator - (const point&p, const vec3 &v){
	point p2={p.x - v.x, p.y - v.y, p.z - v.z };
	return p2;
}

inline vec3 operator + (const vec3&v1, const vec3 &v2){
	vec3 v={v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
	return v;
}

inline vec3 operator - (const point&p1, const point &p2){
	vec3 v={p1.x - p2.x, p1.y - p2.y, p1.z - p2.z };
	return v;
}

inline vec3 operator * (float c, const vec3 &v)
{
	vec3 v2={v.x *c, v.y * c, v.z * c };
	return v2;
}

inline vec3 operator - (const vec3&v1, const vec3 &v2){
	vec3 v={v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
	return v;
}

inline float operator * (const vec3&v1, const vec3 &v2 ) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}



struct color
{
    enum OFFSET 
    {
        OFFSET_RED = 0,
        OFFSET_GREEN = 1,
        OFFSET_BLUE = 2,
        OFFSET_MAX  = 3
    };
    float red, green, blue;

    inline color & operator += (const color &c2 ) {
	    this->red +=  c2.red;
        this->green += c2.green;
        this->blue += c2.blue;
	    return *this;
    }

    inline float & getChannel(OFFSET offset )
    {
        return reinterpret_cast<float*>(this)[offset];
    }

    inline float getChannel(OFFSET offset ) const
    {
        return reinterpret_cast<const float*>(this)[offset];
    }
};

inline color operator * (const color&c1, const color &c2 ) {
	color c = {c1.red * c2.red, c1.green * c2.green, c1.blue * c2.blue};
	return c;
}

inline color operator + (const color&c1, const color &c2 ) {
	color c = {c1.red + c2.red, c1.green + c2.green, c1.blue + c2.blue};
	return c;
}

inline color operator * (float coef, const color &c ) {
	color c2 = {c.red * coef, c.green * coef, c.blue * coef};
	return c2;
}

#endif //__DEF_H
