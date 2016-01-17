#ifndef  CURVEPARAMESTIMATOR_H
#define  CURVEPARAMESTIMATOR_H

#include "ParameterEsitmator.h"
#include "ImagePoint.h"

struct MatchPoint
{
	MatchPoint(){}
	MatchPoint(CPoint2D ptSrc,CPoint2D ptRef):m_ptSrc(ptSrc),m_ptRef(ptRef){}
	CPoint2D m_ptSrc;
	CPoint2D m_ptRef;
};

class CurveParamEstimator : public ParameterEsitmator<MatchPoint,double> {
public:
	CurveParamEstimator(double delta);

	virtual void estimate(std::vector<MatchPoint *> &data, std::vector<double> &parameters);


	virtual void leastSquaresEstimate(std::vector<MatchPoint *> &data, std::vector<double> 
		&parameters);

	virtual bool agree(std::vector<double> &parameters, MatchPoint &data);

	static void debugTest(ostream &out);

private:
	double m_deltaSquared; //given line L and point P, if dist(L,P)^2 < m_delta^2 then the 
};

#endif