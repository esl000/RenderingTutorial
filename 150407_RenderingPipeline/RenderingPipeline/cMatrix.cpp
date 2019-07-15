#include "StdAfx.h"
#include "cMatrix.h"
#include "cVector3.h"

#define EPSILON 0.00001f

//***************************************************************
// cRow
//***************************************************************

cMatrix::cRow::cRow()
{

}

cMatrix::cRow::cRow( int nDimension )
{
	Resize(nDimension);
}

cMatrix::cRow::~cRow()
{

}

void cMatrix::cRow::Print()
{
	for (size_t i = 0; i < m_vecData.size(); ++i)
	{
		printf("%8.2f ", m_vecData[i]);
	}
	printf("\n");
}

void cMatrix::cRow::Resize( int nDimension )
{
	m_vecData.resize(nDimension);
}

float& cMatrix::cRow::operator[]( int nIndex )
{
	return m_vecData[nIndex];
}

//***************************************************************
// cMatrix
//***************************************************************

cMatrix::cMatrix(void)
{
}

cMatrix::cMatrix( int nDimension )
{
	Resize(nDimension);
}

cMatrix::~cMatrix(void)
{
}

void cMatrix::Print()
{
	for (size_t i = 0; i < m_vecData.size(); ++i)
	{
		m_vecData[i].Print();
	}
}

cMatrix::cRow& cMatrix::operator[]( int nIndex )
{
	return m_vecData[nIndex];
}

void cMatrix::Resize( int nDimension )
{
	m_vecData.resize(nDimension);
	for (int i = 0; i < nDimension; ++i)
	{
		m_vecData[i].Resize(nDimension);
	}
}

cMatrix cMatrix::Identity( int nDimension )
{
	cMatrix rmat;
	rmat.Resize(nDimension);
	for (int i = 0; i < nDimension; ++i)
	{
		for (int j = 0; j < nDimension; ++j)
		{
			rmat[i][j] = (i == j) ? 1.0f : 0.0f;
		}
	}
	return rmat;
}

int cMatrix::Dimension()
{
	return m_vecData.size();
}

cMatrix cMatrix::operator+( cMatrix& mat )
{
	cMatrix rmat(Dimension());

	for (int i = 0; i < Dimension(); ++i)
	{
		for (int j = 0; j < Dimension(); ++j)
		{
			rmat[i][j] = (*this)[i][j] + mat[i][j];
		}
	}

	return rmat;
}

cMatrix cMatrix::operator-( cMatrix& mat )
{
	cMatrix rmat(Dimension());

	for (int i = 0; i < Dimension(); ++i)
	{
		for (int j = 0; j < Dimension(); ++j)
		{
			rmat[i][j] = (*this)[i][j] - mat[i][j];
		}
	}

	return rmat;
}

cMatrix cMatrix::operator*( float f )
{
	cMatrix rmat(Dimension());

	for (int i = 0; i < Dimension(); ++i)
	{
		for (int j = 0; j < Dimension(); ++j)
		{
			rmat[i][j] = (*this)[i][j] * f;
		}
	}

	return rmat;
}

cMatrix cMatrix::operator*( cMatrix& mat )
{
	cMatrix rmat(Dimension());

	for (int i = 0; i < Dimension(); ++i)
	{
		for (int j = 0; j < Dimension(); ++j)
		{
			rmat[i][j] = 0.0f;

			for (int k = 0; k < Dimension(); ++k)
			{
				rmat[i][j] += (*this)[i][k] * mat[k][j];
			}
		}
	}

	return rmat;
}

cMatrix operator*( float f, cMatrix mat )
{
	return mat * f;
}

bool cMatrix::operator==( cMatrix& mat )
{
	for (int i = 0; i < Dimension(); ++i)
	{
		for (int j = 0; j < Dimension(); ++j)
		{
			if( fabs((*this)[i][j] - mat[i][j]) > EPSILON)
				return false;
		}
	}

	return true;
}

bool cMatrix::operator!=( cMatrix& mat )
{
	return !((*this) == mat);
}

cMatrix cMatrix::Transpose()
{
	cMatrix rmat(Dimension());

	for (int i = 0; i < Dimension(); ++i)
	{
		for (int j = 0; j < Dimension(); ++j)
		{
			rmat[i][j] = (*this)[j][i];
		}
	}

	return rmat;
}

cMatrix cMatrix::Inverse( OUT float& fDeterminent )
{
	// (1 / det(A)) * adj(A)
	fDeterminent = Determinent();
	return (1.0f / fDeterminent) * Adjoint();
}

float cMatrix::Determinent()
{
	if(Dimension() == 2)
	{
		return (*this)[0][0] * (*this)[1][1] - (*this)[0][1] * (*this)[1][0];
	}
	
	//sigma (0~n) a(i,j) * C(i, j)
	float fDeterminent = 0.0f;

	for (int i = 0; i < Dimension(); ++i)
	{
		fDeterminent += ((*this)[i][0] * Cofactor(i, 0));
	}

	return fDeterminent;
}

cMatrix cMatrix::Adjoint()
{
	cMatrix rmat(Dimension());

	for (int i = 0; i < Dimension(); ++i)
	{
		for (int j = 0; j < Dimension(); ++j)
		{
			rmat[i][j] = Cofactor(j, i);
		}
	}

	return rmat;
}

float cMatrix::Cofactor( int nRow, int nCol )
{
	// (-1)^(i + j) * M(i, j)
	int nConst = 1;
	if((nRow + nCol) % 2 == 1)
		nConst = -1;
	return nConst * Minor(nRow, nCol);
}

float cMatrix::Minor( int nRow, int nCol )
{
	cMatrix mat = cMatrix::Identity(Dimension() - 1);

	int nMinorRow = 0;
	int nMinorCol = 0;

	for (int i = 0; i < Dimension(); ++i)
	{
		if (i == nRow)
			continue;

		nMinorCol = 0;
		
		for (int j = 0; j < Dimension(); ++j)
		{
			if (j == nCol)
				continue;

			mat[nMinorRow][nMinorCol] = (*this)[i][j];
			++nMinorCol;
		}

		++nMinorRow;
	}

	return mat.Determinent();
}

cMatrix cMatrix::Translate( float x, float y, float z )
{
	cMatrix mat = cMatrix::Identity(4);

	mat[3][0] = x;
	mat[3][1] = y;
	mat[3][2] = z;

	return mat;
}

cMatrix cMatrix::RotateX( float fAngle )
{
	cMatrix mat = cMatrix::Identity(4);

	mat[1][1] = cosf(fAngle);
	mat[1][2] = sinf(fAngle);
	mat[2][1] = -sinf(fAngle);
	mat[2][2] = cosf(fAngle);

	return mat;
}

cMatrix cMatrix::RotateY( float fAngle )
{
	cMatrix mat = cMatrix::Identity(4);

	mat[0][0] = cosf(fAngle);
	mat[0][2] = -sinf(fAngle);
	mat[2][0] = sinf(fAngle);
	mat[2][2] = cosf(fAngle);

	return mat;
}

cMatrix cMatrix::RotateZ( float fAngle )
{
	cMatrix mat = cMatrix::Identity(4);

	mat[0][0] = cosf(fAngle);
	mat[0][1] = sinf(fAngle);
	mat[1][0] = -sinf(fAngle);
	mat[1][1] = cosf(fAngle);

	return mat;
}

cMatrix cMatrix::Scale( float x, float y, float z )
{
	cMatrix mat = cMatrix::Identity(4);

	mat[0][0] = x;
	mat[1][1] = y;
	mat[2][2] = z;

	return mat;
}

cMatrix cMatrix::View( cVector3* pEye, cVector3* pLookAt, cVector3* pUp )
{
	cVector3 e = (*pEye);

	cVector3 l = (*pLookAt - *pEye).Normalize();

	cVector3 r = cVector3::Cross(*pUp, l).Normalize();

	cVector3 u = cVector3::Cross(l, r).Normalize();

	cMatrix mat = cMatrix::Identity(4);
	
	mat[0][0] = r.x; mat[0][1] = u.x; mat[0][2] = l.x;
	mat[1][0] = r.y; mat[1][1] = u.y; mat[1][2] = l.y;
	mat[2][0] = r.z; mat[2][1] = u.z; mat[2][2] = l.z;
	mat[3][0] = -cVector3::Dot(r, e);
	mat[3][1] = -cVector3::Dot(u, e);
	mat[3][2] = -cVector3::Dot(l, e);

	return mat;
}

cMatrix cMatrix::Projection( float fFovY, float fAspect, float fNearZ, float fFarZ )
{
	float fScaleY = 1.0f / tanf(fFovY / 2.0f);
	float fScaleX = fScaleY / fAspect;

	cMatrix mat = cMatrix::Identity(4);

	mat[0][0] = fScaleX;
	mat[1][1] = fScaleY;
	mat[2][2] = fFarZ / (fFarZ - fNearZ);
	mat[2][3] = 1.0f;
	mat[3][2] = -(fNearZ * fFarZ) / (fFarZ - fNearZ);
	mat[3][3] = 0.0f;

	return mat;
}

cMatrix cMatrix::Viewport( float x, float y, float w, float h, float minz, float maxz )
{
	cMatrix mat = cMatrix::Identity(4);

	mat[0][0] = w / 2.0f;
	mat[1][1] = -h / 2.0f;
	mat[2][2] = maxz - minz;
	mat[3][0] = x + (w / 2.0f);
	mat[3][1] = y + (h / 2.0f);
	mat[3][2] = minz;

	return mat;
}
