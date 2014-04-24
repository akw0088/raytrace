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

#include "Scene.h"
#include "Config.h"
#include "Raytrace.h"
#include <iostream>
using namespace std;

#define SCENE_VERSION_MAJOR 1
#define SCENE_VERSION_MINOR 1

const vec3 NullVector = { 0.0f,0.0f,0.0f };
const point Origin = { 0.0f,0.0f,0.0f };
const SimpleString emptyString("");
const SimpleString diffuseString ("Diffuse");
const SimpleString specularString ("Specular");
const SimpleString intensityString ("Intensity");

void GetMaterial(const Config &sceneFile, material &currentMat)
{    
	// diffuse color
	{
		float fScalar;

		fScalar =  float(sceneFile.GetByNameAsFloat(diffuseString, 0.0f)); 
		vec3 vColor = {fScalar, fScalar, fScalar};
		vColor = sceneFile.GetByNameAsVector(diffuseString, vColor);
		currentMat.diffuse.red   = vColor.x;
		currentMat.diffuse.green = vColor.y;
		currentMat.diffuse.blue  = vColor.z;
	}

	// Reflection color
	{
		float fScalar;

		fScalar =  float(sceneFile.GetByNameAsFloat("Reflection", 0.0f)); 
		currentMat.reflection = fScalar;
	}

	// specular color
	{
		float fScalar;

		fScalar =  float(sceneFile.GetByNameAsFloat(specularString, 0.0f)); 
		vec3 vColor = {fScalar, fScalar, fScalar};
		vColor = sceneFile.GetByNameAsVector(specularString, vColor);
		currentMat.specular.red   = vColor.x;
		currentMat.specular.green = vColor.y;
		currentMat.specular.blue  = vColor.z;
	}

	// Specular power
	{
		float fScalar;
		fScalar =  float(sceneFile.GetByNameAsFloat("Power", 0.0f)); 
		currentMat.power = fScalar;
	}
}

void GetSphere(const Config &sceneFile, sphere &currentSph)
{
	currentSph.pos = sceneFile.GetByNameAsPoint("Center", Origin); 
	currentSph.size =  float(sceneFile.GetByNameAsFloat("Size", 0.0f)); 
	currentSph.materialId = sceneFile.GetByNameAsInteger("Material.Id", 0); 
}

void GetPlane(const Config &sceneFile, plane &currentPlane)
{
	currentPlane.p = sceneFile.GetByNameAsPoint("Center", Origin); 
	currentPlane.n = sceneFile.GetByNameAsPoint("Normal", Origin); 
	currentPlane.materialId = sceneFile.GetByNameAsInteger("Material.Id", 0); 
}


void GetLight(const Config &sceneFile, light &currentLight)
{
	currentLight.pos = sceneFile.GetByNameAsPoint("Position", Origin); 

	// light color
	{
		float fScalar;
		fScalar =  float(sceneFile.GetByNameAsFloat(intensityString, 0.0f)); 
		vec3 vColor = {fScalar, fScalar, fScalar};
		vColor = sceneFile.GetByNameAsVector(intensityString, vColor);

		currentLight.intensity.red   = vColor.x;
		currentLight.intensity.green = vColor.y;
		currentLight.intensity.blue  = vColor.z;
	}
}

bool init(char* inputName, scene &myScene)
{
	int nbMats, nbSpheres, nbLights, nbPlanes;
	int versionMajor, versionMinor;
	int i;
	Config sceneFile(inputName);

	if (sceneFile.SetSection("Scene") == -1)
	{
		cout << "Mal formed Scene file : No Scene section." << endl;
		return false;
	}

	versionMajor = sceneFile.GetByNameAsInteger("Version.Major", 0);
	versionMinor = sceneFile.GetByNameAsInteger("Version.Minor", 0);

	if (versionMajor != SCENE_VERSION_MAJOR || versionMinor != SCENE_VERSION_MINOR)
	{
        	cout << "Mal formed Scene file : Wrong scene file version." << endl;
		return false;
	}

	myScene.sizex = sceneFile.GetByNameAsInteger("Image.Width", 640);
	myScene.sizey = sceneFile.GetByNameAsInteger("Image.Height", 480);

	nbMats = sceneFile.GetByNameAsInteger("NumberOfMaterials", 0);
	nbSpheres = sceneFile.GetByNameAsInteger("NumberOfSpheres", 0);
	nbLights = sceneFile.GetByNameAsInteger("NumberOfLights", 0);
	nbPlanes = sceneFile.GetByNameAsInteger("NumberOfPlanes", 0);

	myScene.materialContainer.resize(nbMats);
	myScene.sphereContainer.resize(nbSpheres);
	myScene.lightContainer.resize(nbLights);
	myScene.planeContainer.resize(nbPlanes);

	for (i = 0; i < nbMats; ++i)
	{   
		material &currentMat = myScene.materialContainer[i];
		SimpleString sectionName("Material");
		sectionName.append((unsigned long) i);
		if (sceneFile.SetSection( sectionName ) == -1)
		{
			cout << "Mal formed Scene file : Missing Material section." << endl;
			return false;
		}
		GetMaterial(sceneFile, currentMat);
	}

	for (i = 0; i < nbSpheres; ++i)
	{   
		sphere &currentSphere = myScene.sphereContainer[i];
		SimpleString sectionName("Sphere");
		sectionName.append((unsigned long) i);
		if (sceneFile.SetSection( sectionName ) == -1)
		{
			cout << "Mal formed Scene file : Missing Sphere section." << endl;
			return false;
		}
		GetSphere(sceneFile, currentSphere);
		if (currentSphere.materialId >= nbMats)
		{
			cout << "Mal formed Scene file : Material Id not valid." << endl;
			return false;
		}

	}

	for (i = 0; i < nbLights; ++i)
	{   
		light &currentLight = myScene.lightContainer[i];
		SimpleString sectionName("Light");
		sectionName.append((unsigned long) i);
		if (sceneFile.SetSection( sectionName ) == -1)
		{
			cout << "Mal formed Scene file : Missing Light section." << endl;
			return false;
		}
		GetLight(sceneFile, currentLight);
	}

	for (i = 0; i < nbPlanes; i++)
	{   
		plane &currentPlane = myScene.planeContainer[i];
		SimpleString sectionName("Plane");
		sectionName.append((unsigned long) i);
		if (sceneFile.SetSection( sectionName ) == -1)
		{
			cout << "Mal formed Scene file : Missing Plane section." << endl;
			return false;
		}
		GetPlane(sceneFile, currentPlane);
		if (currentPlane.materialId >= nbMats)
		{
			cout << "Mal formed Scene file : Material Id not valid." << endl;
			return false;
		}
	}


	return true;
}

