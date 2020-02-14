#pragma once
#include "stdafx.h"
#include "Matrix.h"
#include "Object.h"

#define SINGLETON(class_name)\
private:\
class_name() {}\
public:\
static class_name* GetInstance(){\
static class_name instance;\
return &instance; }


template<typename Type>
inline bool CastCheck(Object* pointer)
{
	return (bool) dynamic_cast<Type*>(pointer);
}

template<typename Type>
inline Type* Cast(Object* pointer)
{
	return dynamic_cast<Type*>(pointer);
}