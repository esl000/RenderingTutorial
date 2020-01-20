#include "stdafx.h"
#include "Matrix.h"

const Matrix4x4 Matrix4x4::identity = Matrix4x4(1.f, 0.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
	0.f, 0.f, 1.f, 0.f,
	0.f, 0.f, 0.f, 1.f);