static void XYZToLatLonHeight(double x, double y, double z, double& lat, double& lon, double& height)
{
	double a = 6378137.0000000000 /*6378249.200*/;	//椭球体长半轴
	double e = 0.081819190928906375980579523352811 /*0.08248325679*/;
	double d = sqrt(x*x + y*y);

	double phi0  =  z / (  d*(1- a*e*e/sqrt(x*x + y*y +z*z) ) );
	double phi1  = phi0;
	int iterCount = 0;
	do 
	{
		phi0 = phi1;
		double dtmp = 1- a*e*e*cos(phi0)/(d* sqrt(1-M_SQUARE(e*sin(phi0)) )) ;
		dtmp = z/(d*dtmp);
		phi1 = atan(dtmp);
		iterCount++;
	}while(ABS(phi1-phi0) >= 1e-11);

	lon  = atan(y/x);
	lat = phi1;
	height = d/cos(lat) - a/sqrt(1- M_SQUARE(e*sin(lat)));
	printf("共迭代%d次,经度=%.12f,维度=%.12f,高程=%f.4\n",iterCount,lon,lat,height);
	lat = lat*180.0/M_PI; //角度制的维度
	lon = atan2(y, x)*180.0/M_PI; //角度制的经度
}