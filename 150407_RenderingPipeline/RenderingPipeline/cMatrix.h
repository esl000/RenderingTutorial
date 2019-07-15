#pragma once

class cVector3;

class cMatrix
{
private:
	class cRow
	{
	private:
		std::vector<float> m_vecData;

	public:
		cRow();
		cRow(int nDimension);
		~cRow();

		void Print();
		void Resize(int nDimension);
		float& operator[](int nIndex);
	};

private:
	std::vector<cRow> m_vecData;

public:
	cMatrix(void);
	cMatrix(int nDimension);
	~cMatrix(void);

	void			Print();
	cRow&			operator[](int nIndex);
	void			Resize(int nDimension);
	static cMatrix	Identity(int nDimension);
	int				Dimension();
	cMatrix			operator+(cMatrix& mat);
	cMatrix			operator-(cMatrix& mat);
	cMatrix			operator*(float f);
	friend cMatrix	operator*(float f, cMatrix mat);
	cMatrix			operator*(cMatrix& mat);
	bool			operator==(cMatrix& mat);
	bool			operator!=(cMatrix& mat);
	cMatrix			Transpose();
	cMatrix			Inverse(OUT float& fDeterminent);
	float			Determinent();
	cMatrix			Adjoint();
	float			Cofactor(int nRow, int nCol);
	float			Minor(int nRow, int nCol);

	static cMatrix	Translate(float x, float y, float z);
	static cMatrix	RotateX(float fAngle);
	static cMatrix	RotateY(float fAngle);
	static cMatrix	RotateZ(float fAngle);
	static cMatrix	Scale(float x, float y, float z);
	static cMatrix	View(cVector3* pEye, cVector3* pLookAt, cVector3* pUp);
	static cMatrix	Projection(float fFovY, float fAspect, float fNearZ, float fFarZ);
	static cMatrix	Viewport(float x, float y, float w, float h, float minz, float maxz);
};

