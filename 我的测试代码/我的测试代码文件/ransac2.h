#ifndef RANSAC2_H
#define RANSAC2_H
#include "ImagePoint.h"
struct Point_Pair
{
	CPoint2D LP;
	CPoint2D RP;
	double march;
	double dtheta;
};

class CRansac
{
public:
	CRansac(double dErr = 5.0);
	~CRansac();

	bool AFFINERANSAC(Point_Pair* matchPair,int nMatchNum,
		int*& index,double*& dbAffine,int& nAffineNum);
private:

	bool ransac(Point_Pair* matchpair,int nMatchNum,int s,
		double*& dbaffinepara,int*& index,int& nAffineNum);

	void affinetransform(Point_Pair* matchpair,int* ind,int s,double* M);

	void EvaluateDist(Point_Pair* matchpair,int nMatchNum,double* M,int*& x,int& ninliers);
private:
	double m_dErr;

};
#endif