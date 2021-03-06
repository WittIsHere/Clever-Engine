#ifndef __JSON_PARSER_H__
#define __JSON_PARSER_H__

#include "Dependencies/parson/parson.h"
#include "Dependencies/MathGeoLib/include/Math/float2.h"
#include "Dependencies/MathGeoLib/include/Math/float3.h"
#include "Dependencies/MathGeoLib/include/Math/float4.h"
#include "Globals.h"

class ParsonArray;																		// To avoid having to forward-declare this, put Configuration_Array before Configuration. 

struct Color;

namespace math
{
	class float3;
	class float4;
	class Quat;
}

typedef unsigned int uint;

#define INVALID_PARSON_NUMBER -2147483647.0												// Not the actual max negative value of a double var., but an aproximation using long as reference.

class ParsonNode
{
public:
	ParsonNode();
	ParsonNode(const char* buffer);
	ParsonNode(JSON_Object* object);
	~ParsonNode();

	uint		SerializeToBuffer	(char** buffer);				
	uint		SerializeToFile		(const char* path, char** buffer);	
	bool		Release				();											

public:																
	int			GetInteger	(const char* name);
	double		GetNumber	(const char* name) const;	
	float2		GetFloat2	(const char* name);
	float3		GetFloat3	(const char* name);
	float4		GetFloat4	(const char* name);
	Quat		GetQuat		(const char* name);
	Color		GetColor	(const char* name);
	const char*	GetString	(const char* name) const;						
	bool		GetBool		(const char* name) const;						
	ParsonArray	GetArray	(const char* name, bool logErrors = true) const;						
	ParsonNode	GetNode		(const char* name, bool logErrors = true) const;						

	void		SetInteger	(const char* name, int number);
	void		SetNumber	(const char* name, double number);	
	void		SetFloat2	(const char* name, const float2& float2);
	void		SetFloat3	(const char* name, const float3& float3);
	void		SetFloat4	(const char* name, const float4& float4);
	void		SetColor	(const char* name, const Color& float4);
	void		SetQuat		(const char* name, const Quat& quat);
	void		SetString	(const char* name, const char* string);		
	void		SetBool		(const char* name, bool value);					
	ParsonArray	SetArray	(const char* name);								
	ParsonNode	SetNode		(const char* name);								

public:
	bool		NodeHasValueOfType	(const char* name, JSON_Value_Type valueType, bool logErrors = true) const;
	bool		NodeIsValid			();													
	JSON_Value*	FindValue			(const char* name, int index);						

private:
	JSON_Value*		rootValue;																
	JSON_Object*	rootNode;																
};

class ParsonArray
{
public:
	ParsonArray();
	ParsonArray(JSON_Array* jsonArray, const char* name = "Array");

public:																						
	double		GetNumber	(uint index) const;						
	const char*	GetString	(uint index) const;						
	bool		GetBool		(uint index) const;						
	void		GetColor	(uint index, Color& color) const;
	void		GetFloat3	(uint index, math::float3& vec3) const;
	void		GetFloat4	(uint index, math::float4& vec4) const;
	void		GetQuat		(uint index, math::Quat& quat) const;
	ParsonNode	GetNode		(uint index) const;						

	void		SetNumber	(double number);
	void		SetString	(const char* string);							
	void		SetBool		(bool value);
	void		SetColor	(const Color& color);
	void		SetFloat3	(const math::float3& vec3);
	void		SetFloat4	(const math::float4& vec4);
	ParsonNode	SetNode		(const char* name);

public:
	bool			ArrayIsValid			() const;

	uint			GetSize					() const;
	bool			IndexIsWithinRange		(uint index);

	JSON_Value_Type GetTypeAtIndex			(uint index) const;
	bool			HasValueOfTypeAtIndex	(uint index, JSON_Value_Type valueType) const;

public:
	JSON_Array*	jsonArray;
	uint		size;

	const char*	name;
};
#endif // !__JSON_PARSER_H__