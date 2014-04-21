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


#ifndef __CONFIG_H
#define __CONFIG_H

#include "Def.h"
#include "SimpleString.h"
#pragma warning( push )
#pragma warning( disable : 4512 ) // assignment operator cannot be generated because of the const member. We don't need one.

// Some code
class Config
{
public:
	// When the variable called "sName" doesn't exit, you will get "default" 
	bool GetByNameAsBoolean(const SimpleString  & sName, bool bDefault) const;
	double GetByNameAsFloat(const SimpleString & sName, double fDefault) const;
	const SimpleString &GetByNameAsString(const SimpleString  &sName, const SimpleString  & sDefault) const;
	long GetByNameAsInteger(const SimpleString  &sName, long lDefault) const;
	vec3 GetByNameAsVector(const SimpleString &sName, const vec3& vDefault) const;
	point GetByNameAsPoint(const SimpleString &sName, const point& ptDefault) const;
    
	// SetSection will return -1 when the section wasn't found. 
	int SetSection(const SimpleString &sName);
	~Config();
	Config(const SimpleString &sFileName);
private:
	void * m_pVariables;
	void * m_pSections;
	const SimpleString m_sFileName;
	SimpleString m_sCurrentSection;
	bool m_bLoaded;
};

#pragma warning( pop ) 
#endif //__CONFIG_H
