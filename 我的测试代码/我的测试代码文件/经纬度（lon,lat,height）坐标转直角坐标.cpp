/*
*@brief 经纬度坐标转直角坐标
*@param lon		输入大地经度
*@param lat		输入大纬度
*@param height  输入大地高程
*@param x		输出大地直角坐标x
*@param y		输出大地直角坐标y
*@param z		输出大地直角坐标z
*/
static void LatLonHeightToXYZ( const double lon, const double lat, const double height,double &x, double &y, double &z)
{
	double a = 6378249.2000 /*6378249.200*/;	//椭球体长半轴
	double e = 0.08248325679 /*0.08248325679*/;
	double N = a/sqrt(1-M_SQUARE(e*sin(lat)) );
	x = (N+ height)*cos(lat)*cos(lon);
	y = (N+ height)*cos(lat)*sin(lon);
	z = (N*(1-e*e) +height)*sin(lat);
	printf("经度：%f, 纬度：%f, 高程：%f, 椭球长半轴%f, 椭球扁率%f ,x = %.4f ,y =%.4f ,z = %.4f\n",lon,lat,height,a,e,x,y,z);
}