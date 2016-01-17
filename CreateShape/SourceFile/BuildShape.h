#include <gdal_priv.h>
#include <ogrsf_frmts.h>
#include <QMessageBox>
#pragma comment(lib,"gdal_i.lib")
bool build(const char* pszFileName ,const char *outdataName ,int line,int row);