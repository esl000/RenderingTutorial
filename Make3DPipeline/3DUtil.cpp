#include "stdafx.h"
#include "3DUtil.h"

const Vector3 Vector3::zero = Vector3(0.f, 0.f, 0.f);
const Vector3 Vector3::one = Vector3(1.f, 1.f, 1.f);
const Matrix4x4 Matrix4x4::identity = Matrix4x4(1.f, 0.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
	0.f, 0.f, 1.f, 0.f, 
	0.f, 0.f, 0.f, 1.f);