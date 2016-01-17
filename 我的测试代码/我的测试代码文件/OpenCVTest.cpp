/*This is a standalone program.Pass an image name as a first parameter
of the program.Switch between standard and probabilistic Hough transform
by changing"#if 1"to"#if 0"and back*/
#include<opencv\cv.h>

#include<opencv\highgui.h>
#include<math.h>

#pragma comment(lib, "opencv_imgproc243.lib")
#pragma comment(lib, "opencv_core243.lib")
#pragma comment(lib, "opencv_highgui243.lib")

int mainxxx(int argc,char**argv)
{
	IplImage*src;
	if(argc==2&&(src=cvLoadImage(argv[1],0))!=0)
	{
		IplImage*dst=cvCreateImage(cvGetSize(src),8,1);
		IplImage*color_dst=cvCreateImage(cvGetSize(src),8,3);
		CvMemStorage*storage=cvCreateMemStorage(0);
		CvSeq*lines=0;
		int i;
		cvCanny(src,dst,50,200,3);
		cvCvtColor(dst,color_dst,CV_GRAY2BGR);
#if 0
		lines=cvHoughLines2(dst,
			storage,
			CV_HOUGH_STANDARD,
			1,
			CV_PI/180,
			100,
			0,
			0);
		for(i=0;i<MIN(lines->total,100);i++)
		{
			float*line=(float*)cvGetSeqElem(lines,i);
			float rho=line[0];
			float theta=line[1];
			CvPoint pt1,pt2;
			double a=cos(theta),b=sin(theta);
			double x0=a*rho,y0=b*rho;
			pt1.x=cvRound(x0+1000*(-b));
			pt1.y=cvRound(y0+1000*(a));
			pt2.x=cvRound(x0-1000*(-b));
			pt2.y=cvRound(y0-1000*(a));
			cvLine(color_dst,pt1,pt2,CV_RGB(255,0,0),3,8);
		}
#else
		lines=cvHoughLines2(dst,
			storage,
			CV_HOUGH_PROBABILISTIC,
			1,
			CV_PI/180,
			80,
			30,
			10);
		for(i=0;i<lines->total;i++)
		{
			CvPoint*line=(CvPoint*)cvGetSeqElem(lines,i);
			cvLine(color_dst,line[0],line[1],CV_RGB(255,0,0),3,8);
		}
#endif
		cvNamedWindow("Source",1);
		cvShowImage("Source",src);
		cvNamedWindow("Hough",1);
		cvShowImage("Hough",color_dst);
		cvWaitKey(0);
	}

	return 1;
}
