// OutlierDetection.cpp: implementation of the CRansac class.
//
//////////////////////////////////////////////////////////////////////


#include "ransac2.h"
#include <time.h>
#include "MartixEigenvalue.h"

#define expvalue 2.2204e-016
#define BrightValue 250
#define DarkValue 5
#define ROUND(x) ((int) ((x) + 0.5))


bool CalAffineTransformCor2(vector<CPoint2D> vSrcPoint,vector<CPoint2D> vRefPoint,vector<double> &dVCor)
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

void GetAffinePara(CPoint2D SrcPt[],CPoint2D RefPt[],int num,double *M)
{
	if (num<3 || !M)
		return;

	vector<CPoint2D> vSrcTmp,vRefTmp;
	for(int i=0;i<num;i++)
	{
		vSrcTmp.push_back(SrcPt[i]);
		vRefTmp.push_back(RefPt[i]);
	}
	vector<double> dVcor; //存储仿射变换参数b1,b2,b3,a1,a2,a3
	bool bOk = CalAffineTransformCor2(vSrcTmp,vRefTmp,dVcor);
	if (!bOk)
		return;
	for (size_t i=0;i<dVcor.size();i++)
		M[i] = dVcor[i];
}
CRansac::CRansac(double dErr)
{
	m_dErr = dErr;
}

CRansac::~CRansac()
{
}
/*
    输入参数：matchPair：初始匹配的特征点对
	          nMatchNum：初始匹配的特征点数目
			  index：最佳拟和时的拟和点在matchpair中的索引值
			  dbAffine：拟合出的仿射变换系数
			  nAffineNum：拟和出来的点数目

*/
bool CRansac::AFFINERANSAC(Point_Pair* matchPair,int nMatchNum,
								 int*& index,double*& dbAffine,int& nAffineNum)
{
	int s=3;//平面仿射变换
    index = NULL;
    if (ransac(matchPair,nMatchNum,s,dbAffine,index,nAffineNum)) 
        return true;
	else
		return false;
}
bool CRansac::ransac(Point_Pair* matchpair,int nMatchNum,int s,
			double*& dbaffinepara,int*& index,int& nAffineNum)
{
  	double p = 0.99;    
	int maxTrials = 1000;    
	int maxDataTrials = 100; 
   	
	double* M = new double[6];
	memset(M,0,sizeof(double)*6);
	int	trialcount = 0;
    int bestscore =  0;  
	int score; //支持某矩阵的点对数目
    int N = 1;
	int* ind = new int[s];
	int* perMIndex = NULL;
	int* bestMIndex = NULL;
	int i,k;
	int temp;
	while (N > trialcount)
	{
		/*产生随机序列*/
		srand((unsigned)time(NULL)); 
		for(i=0;i<s;i++) //s=3随机抽出三对儿点，且这三对点不能完全相同
		{
			temp = rand();
			ind[i] = ROUND(((double)(temp%RAND_MAX))/RAND_MAX*nMatchNum);
			if (ind[i]>=nMatchNum)
				ind[i]= nMatchNum-1;
		}
		while(1)
		{
			if ((ind[0] == ind[1]) && (ind[1]==ind[2])) //s等于3的情况，三个点不能完全相同
			{
				int temp = rand();
				ind[0] = ROUND(((double)(temp%RAND_MAX))/RAND_MAX*nMatchNum);
				if (ind[0]>=nMatchNum)
					ind[0]= nMatchNum-1;
			}
			else
				break;
		}

 		affinetransform(matchpair,ind,s,M);
		EvaluateDist(matchpair,nMatchNum,M,perMIndex,score);
		if(score > bestscore)   
		{
			bestscore = score;
			
			for(k=0;k<6;k++)
			{
  				if (M[k]<0.0000001) {
					M[k] = 0;
				}
			}
			dbaffinepara = M;
			/*删除前一拟和的序列*/
			if (bestMIndex!=NULL) {
				delete[] bestMIndex;
				bestMIndex = NULL;
			}
			bestMIndex = perMIndex;
			double fracinliers =  (double)score/nMatchNum;
			double pNoOutliers = 1 - pow(fracinliers,s);
			pNoOutliers = max(expvalue,pNoOutliers);
			pNoOutliers = min(1-expvalue, pNoOutliers);
			N = int(log(1-p)/log(pNoOutliers));
			trialcount = 0;
			continue;
		}
		else
		{
			if (perMIndex) {
				delete[] perMIndex;
				perMIndex = NULL;
			}
			trialcount = trialcount+1;
		}
		if (trialcount>maxTrials) {
			break;
		}
	}
	if (dbaffinepara==NULL) {
		return false;
	}
	else{
		index = bestMIndex;
		nAffineNum = bestscore;
		return true;
	}
}
void CRansac::affinetransform(Point_Pair* matchpair,int* ind,int s,double* M)
{
	int i,j;
	CPoint2D pPointBase[3];
	CPoint2D pPointSampl[3];
	for(i=0;i<s;i++)
	{
       j = ind[i];
	   pPointBase[i] = matchpair[j].LP; //左影像的点
	   pPointSampl[i] = matchpair[j].RP;//右影像的点
	}
	//*第一个系数 ＝ R*第二个系数矩阵
	GetAffinePara(pPointSampl,pPointBase,3,M);
	return;
}

void CRansac::EvaluateDist(Point_Pair* matchpair,int nMatchNum,double* M,int*& x,int& ninliers)
{
	CPoint2D LP,RP;
	ninliers = 0; 

	double b1 = M[0];
	double b2 = M[1];
	double b3 = M[2];
	double a1 = M[3];
	double a2 = M[4];
	double a3 = M[5];

	x = new int[nMatchNum];
	for(int i=0;i<nMatchNum;i++)
	{
		LP = matchpair[i].LP;
		RP = matchpair[i].RP;

		double dRrrX = b1 + b2 * LP.x + b3 * LP.y - RP.x;
		double dRrrY = a1 + a2 * LP.x + a3 * LP.y - RP.y;

		double dErr = sqrt(M_SQUARE(dRrrX)+M_SQUARE(dRrrY));  //求残差

		if (dErr< m_dErr)
		{
              x[ninliers] = i;
			 ninliers++;	
		}
	}
	return;
}	
	