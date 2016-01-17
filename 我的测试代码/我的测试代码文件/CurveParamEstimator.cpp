#include <math.h>
#include "MartixEigenvalue.h"
#include "CurveParamEstimator.h"

bool CalAffineTransformCor(vector<CPoint2D> vSrcPoint,vector<CPoint2D> vRefPoint,vector<double> &dVCor)
{
	int nCount=vSrcPoint.size();

	if (nCount<3) //点对个数小于2将不能计算
		return false;

	double *pDataB=new double[2*nCount]; //矩阵B的数据

	double *pDataA=new double[12*nCount];

	for(int i=1;i<=nCount;i++) //给矩阵数据域赋值
	{
		pDataB[2*(i-1)]	  = vSrcPoint[i-1].x;
		pDataB[2*(i-1)+1] = vSrcPoint[i-1].y;

		pDataA[12*(i-1)]	=1;
		pDataA[12*(i-1)+1]	=vRefPoint[i-1].x;
		pDataA[12*(i-1)+2]	=vRefPoint[i-1].y;
		pDataA[12*(i-1)+3]  =0;
		pDataA[12*(i-1)+4]  =0;
		pDataA[12*(i-1)+5]  =0;
		pDataA[12*(i-1)+6]  =0;
		pDataA[12*(i-1)+7]  =0;
		pDataA[12*(i-1)+8]  =0;
		pDataA[12*(i-1)+9]	=1;
		pDataA[12*(i-1)+10]	=vRefPoint[i-1].x;
		pDataA[12*(i-1)+11]	=vRefPoint[i-1].y;
	}

	ExtMatrix eB(pDataB, 2*nCount,1 );
	ExtMatrix eA(pDataA, 2*nCount,6);

	Matrix mB(2*nCount,1);
	Matrix mA(2*nCount,6);

	mtl::twod_copy_default(eB, mB);
	mtl::twod_copy_default(eA, mA);

	Vector X(6); //系数b1,b2,b3,a1,a2,a3

	bool f = SloveMartix(mA, mB, X);

	RELEASE(pDataB);
	RELEASE(pDataA);

	if (!f)
		return false;

	for (size_t i=0;i<6;i++)
		dVCor.push_back(X[i]);

	return true;
}

CurveParamEstimator::CurveParamEstimator(double delta)
{
	m_deltaSquared = delta;
}

void CurveParamEstimator::estimate(std::vector<MatchPoint *> &data, std::vector<double> &parameters)
{
	parameters.clear();
	int nCount = data.size();

	if (nCount<3)
		return;

	vector<CPoint2D> vSrcTmp,vRefTmp;
	for(int i=0;i<nCount;i++)
	{
		vSrcTmp.push_back(data[i]->m_ptSrc);
		vRefTmp.push_back(data[i]->m_ptRef);
	}
	vector<double> dVcor; //存储仿射变换参数b1,b2,b3,a1,a2,a3
	bool bOk = CalAffineTransformCor(vSrcTmp,vRefTmp,dVcor);
	if (!bOk)
		return;

	for (size_t i=0;i<dVcor.size();i++)
		parameters.push_back(dVcor[i]);
}


void CurveParamEstimator::leastSquaresEstimate(std::vector<MatchPoint *> &data, std::vector<double> &parameters)
{
	parameters.clear();
	if(data.size()<3)
		return;

	vector<CPoint2D> vSrcTmp,vRefTmp;
	for (size_t i=0;i<data.size();i++) //用所有点，估算最小二乘的系数
	{
		vSrcTmp.push_back(data[i]->m_ptSrc);
		vRefTmp.push_back(data[i]->m_ptRef);
	}

	vector<double> dVcor; //存储仿射变换参数b1,b2,b3,a1,a2,a3
	bool bOk = CalAffineTransformCor(vSrcTmp,vRefTmp,dVcor);
	if (!bOk)
		return;

	for (size_t i=0;i<dVcor.size();i++)
		parameters.push_back(dVcor[i]);
}

bool CurveParamEstimator::agree(std::vector<double> &parameters, MatchPoint &data)
{
	CPoint2D ptSrc,ptRef;
	ptSrc = data.m_ptSrc;
	ptRef = data.m_ptRef;


	double b1 = parameters[0];
	double b2 = parameters[1];
	double b3 = parameters[2];
	double a1 = parameters[3];
	double a2 = parameters[4];
	double a3 = parameters[5];

	double dRrrX = b1 + b2 * ptSrc.x + b3 * ptSrc.y - ptRef.x;
	double dRrrY = a1 + a2 * ptSrc.x + a3 * ptSrc.y - ptRef.y;

	double dErr = sqrt(M_SQUARE(dRrrX)+M_SQUARE(dRrrY));  //求残差

	if (dErr<m_deltaSquared)
		return true;
	else
		return false;
}

void CurveParamEstimator::debugTest(ostream &out)
{

}
