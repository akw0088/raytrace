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

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
using namespace std;

#include "Raytrace.h"
#include "Scene.h"
#include "Srgb.h"
#include <windows.h> // for RGB macro

bool hitSphere(const ray &r, const sphere& s, float &t)
{
	// Intersection of a ray and a sphere
	// Check the articles for the rationale
	// NB : this is probably a naive solution
	// that could cause precision problems
	// but that will do it for now. 

	vec3 dist = s.pos - r.start;
	float B = (r.dir.x * dist.x + r.dir.y * dist.y + r.dir.z * dist.z);
	float D = B*B - dist*dist + s.size * s.size;
	if (D < 0.0f) return false;
	float t0 = B - sqrtf(D);
	float t1 = B + sqrtf(D);
	bool retvalue = false;
	if ((t0 > 0.1f ) && (t0 < t))
	{
		t = t0;
		retvalue = true;
	}
	if ((t1 > 0.1f ) && (t1 < t))
	{
		t = t1;
		retvalue = true;
	}
	return retvalue;
}

color addRay(ray viewRay, scene &myScene)
{
	color output = {0.0f, 0.0f, 0.0f}; 
	float coef = 1.0f;
	int level = 0;
	do 
	{
	        point ptHitPoint;
		int currentSphere=-1;
	        {
			    float t = 2000.0f;
			    for (unsigned int i = 0; i < myScene.sphereContainer.size() ; ++i)
			    {
				    if (hitSphere(viewRay, myScene.sphereContainer[i], t))
				    {
					    currentSphere = i;
				    }
			    }
			    if (currentSphere == -1)
				    break;
	    		
			    ptHitPoint  = viewRay.start + t * viewRay.dir;
	        }
		vec3 vNormal = ptHitPoint - myScene.sphereContainer[currentSphere].pos;
		float temp = vNormal * vNormal;
		if (temp == 0.0f)
			break;
		temp = 1.0f / sqrtf(temp);
		vNormal = temp * vNormal;

		material currentMat = myScene.materialContainer[myScene.sphereContainer[currentSphere].materialId];

		ray lightRay;
		lightRay.start = ptHitPoint;

		for (unsigned int j = 0; j < myScene.lightContainer.size() ; ++j)
		{
			light currentLight = myScene.lightContainer[j];

			lightRay.dir = currentLight.pos - ptHitPoint;
        		float fLightProjection = lightRay.dir * vNormal;

			if ( fLightProjection <= 0.0f )
				continue;

			float lightDist = lightRay.dir * lightRay.dir;

			{
				float temp = lightDist;
				if ( temp == 0.0f )
					continue;
				temp = invsqrtf(temp);
				lightRay.dir = temp * lightRay.dir;
				fLightProjection = temp * fLightProjection;
			}

			bool inShadow = false;
			{
				float t = lightDist;
				for (unsigned int i = 0; i < myScene.sphereContainer.size() ; ++i)
				{
					if (hitSphere(lightRay, myScene.sphereContainer[i], t))
					{
						inShadow = true;
						break;
					}
				}
			}


			if (!inShadow)
			{
				float lambert = (lightRay.dir * vNormal) * coef;
				output.red += lambert * currentLight.intensity.red * currentMat.diffuse.red;
				output.green += lambert * currentLight.intensity.green * currentMat.diffuse.green;
				output.blue += lambert * currentLight.intensity.blue * currentMat.diffuse.blue;

				// Blinn 
				// The direction of Blinn is exactly at mid point of the light ray 
				// and the view ray. 
				// We compute the Blinn vector and then we normalize it
				// then we compute the coeficient of blinn
				// which is the specular contribution of the current light.

				float fViewProjection = viewRay.dir * vNormal;
				vec3 blinnDir = lightRay.dir - viewRay.dir;
				float temp = blinnDir * blinnDir;
				if (temp != 0.0f )
				{
					float blinn = invsqrtf(temp) * max(fLightProjection - fViewProjection , 0.0f);
					blinn = coef * powf(blinn, currentMat.power);
					output += blinn *currentMat.specular  * currentLight.intensity;
				}
			}
		}
		coef *= currentMat.reflection;
		float reflet = 2.0f * (viewRay.dir * vNormal);
		viewRay.start = ptHitPoint;
		viewRay.dir = viewRay.dir - reflet * vNormal;
		level++;
	} while ((coef > 0.0f) && (level < 10));  
	return output;
}

bool draw(scene &myScene, int *pixel)
{
	int x, y;

	for (y = 0; y < myScene.sizey; ++y)
	{
		for (x = 0 ; x < myScene.sizex; ++x)
		{
			color output = {0.0f, 0.0f, 0.0f};
			for (float fragmentx = float(x) ; fragmentx < x + 1.0f; fragmentx += 0.5f )
			for (float fragmenty = float(y) ; fragmenty < y + 1.0f; fragmenty += 0.5f )
			{
				float sampleRatio=0.25f;

				ray viewRay = { {fragmentx, fragmenty, -1000.0f},{ 0.0f, 0.0f, 1.0f}};
				color temp = addRay (viewRay, myScene);
				// pseudo photo exposure
				float exposure = -1.00f; // random exposure value. TODO : determine a good value automatically
				temp.blue = (1.0f - expf(temp.blue * exposure));
				temp.red =  (1.0f - expf(temp.red * exposure));
				temp.green = (1.0f - expf(temp.green * exposure));

				output += sampleRatio * temp;
			}

			// gamma correction
			output.blue = srgbEncode(output.blue);
			output.red = srgbEncode(output.red);
			output.green = srgbEncode(output.green);

			pixel[y * myScene.sizex + x] = RGB(min(output.blue*255.0f,255.0f), min(output.green*255.0f, 255.0f), min(output.red*255.0f, 255.0f));
		}
	}
	return true;
}

