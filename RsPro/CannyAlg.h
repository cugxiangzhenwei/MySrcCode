#ifndef _CANNYALG_H
#define _CANNYALG_H

/*
*@ brief 采用高斯滤波对图象进行滤波，滤波先对x方向进行，然后对y方向滤波
*@ param pImgData		指向图象数据的指针
*@ param nWidth			图象数据宽度
*@ param nHeight		图象数据高度
*@ param sigma			高斯函数的标准差
*@ param pImgOutData	指向经过平滑之后的图象数据
*/
void GaussianSmooth(unsigned char *pImgData, int nWidth, int nHeight, 
					double sigma, unsigned char * pImgOutData);
/*
*@ brief 这个函数计算方向倒数，采用的微分算子是(-1 0 1) 和 (-1 0 1)T(转置) 计算的时候对边界象素采用了特殊处理
*@ param pGaussSmthedData		指向图象数据的指针
*@ param nWidth			图象数据宽度
*@ param nHeight		图象数据高度
*@ param pnGradX			高斯函数的标准差
*@ param pnGradY	指向经过平滑之后的图象数据
*/
void DirGrad(unsigned char *pGaussSmthedData, int nWidth, int nHeight,
			 int *pnGradX , int *pnGradY);

/*
*@ brief				利用方向倒数计算梯度幅度，方向倒数是DirGrad函数计算的结果
*@ param nWidth			图象数据宽度
*@ param nHeight		图象数据高度
*@ param pnGradX		高斯函数的标准差
*@ param pnGradY	    指向经过平滑之后的图象数据
*/
void GradMagnitude(int *pnGradX, int *pnGradY, int nWidth, int nHeight, int *pnMag);

/*
*@ brief				抑止梯度图中非局部极值点的象素
*@ param pnMag			梯度图
*@ param pnGradX		x方向的方向导数
*@ param pnGradY		y方向的方向导数
*@ param nWidth	        图象数据宽度
*@ param nHeight		图象数据高度
*@ param pUnchRst	    存储处理后的结果数据
*/
void NonmaxSuppress(int *pnMag, int *pnGradX, int *pnGradY, int nWidth, 
					int nHeight,	unsigned char *pUnchRst);

/*
*@ brief		本函数实现类似“磁滞现象”的一个功能，也就是，先调用EstimateThreshold
*函数对经过non-maximum处理后的数据pUnchSpr估计一个高阈值，然后判断
*pUnchSpr中可能的边界象素(=128)的梯度是不是大于高阈值nThdHigh，如果比
*该阈值大，该点将作为一个边界的起点，调用TraceEdge函数，把对应该边界
*的所有象素找出来。最后，当整个搜索完毕时，如果还有象素没有被标志成
*边界点，那么就一定不是边界点。
*@ param pnMag			梯度幅度图
*@ param nWidth	        图象数据宽度
*@ param nHeight		图象数据高度
*@ param dRatioLow	    存储处理后的结果数据
*@ param dRatioHigh	    图象数据宽度
*@ param pUnchEdge		图象数据高度
*/
void Hysteresis(int *pnMag, int nWidth, int nHeight, double dRatioLow, 
				double dRatioHigh, unsigned char *pUnchEdge);
/*
*@ brief		经过non-maximum处理后的数据pUnchEdge，统计pnMag的直方图，确定阈值。
*   本函数中只是统计pUnchEdge中可能为边界点的那些象素。然后利用直方图，
*   根据dRatioHigh设置高阈值，存储到pnThdHigh。利用dRationLow和高阈值，
*   设置低阈值，存储到*pnThdLow。dRatioHigh是一种比例：表明梯度小于
*   *pnThdHigh的象素数目占象素总数目的比例。dRationLow表明*pnThdHigh
*   和*pnThdLow的比例，这个比例在canny算法的原文里，作者给出了一个区间。
*@ param pnMag			梯度幅度图
*@ param nWidth	        图象数据宽度
*@ param nHeight		图象数据高度
*@ param pnThdHigh	    高阈值
*@ param pnThdLow	    低阈值
*@ param dRatioLow		低阈值和高阈值之间的比例
*@ param dRatioHigh	    高阈值占图象象素总数的比例
*@ param pUnchEdge		经过non-maximum处理后的数据
*/
void EstimateThreshold(int *pnMag, int nWidth, int nHeight, int *pnThdHigh,int *pnThdLow, 
					   unsigned char * pUnchEdge, double dRatioHigh, double dRationLow);
/*
*@ brief  从(x,y)坐标出发，进行边界点的跟踪，跟踪只考虑pUnchEdge中没有处理并且
*可能是边界点的那些象素(=128),象素值为0表明该点不可能是边界点，象素值为255表明该点已经被设置为边界点，不必再考虑
*@ param y			跟踪起点的y坐标 
*@ param x	        跟踪起点的x坐标
*@ param nLowThd	判断一个点是否为边界点的低阈值
*@ param pUnchEdge	记录边界点的缓冲区
*@ param pnMag	    梯度幅度图
*@ param nWidth		图象数据宽度
*/
void TraceEdge (int y, int x, int nLowThd, unsigned char *pUnchEdge, int *pnMag, int nWidth);

/*
*@ brief				canny分割算子，计算的结果保存在pUnchEdge中，逻辑1(255)表示该点为
* 边界点，逻辑0(0)表示该点为非边界点。该函数的参数sigma，dRatioLow
* dRatioHigh，是需要指定的。这些参数会影响分割后边界点数目的多少
*@ param pUnchImage		图象数据
*@ param nWidth			图象数据宽度
*@ param nHeight		图象数据高度
*@ param sigma			高斯滤波的标准方差
*@ param dRatioLow		低阈值和高阈值之间的比例
*@ param dRatioHigh	    高阈值占图象象素总数的比例
*@ param pUnchEdge		canny算子计算后的分割图
*/ 
void Canny(unsigned char *pUnchImage, int nWidth, int nHeight, double sigma,
		   double dRatioLow, double dRatioHigh, unsigned char *pUnchEdge);
#endif