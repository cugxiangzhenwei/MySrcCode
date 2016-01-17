#pragma once 
#define ID_BAR_JIANMO_TREE 10001
#define ID_BAR_RES_LIB     10002
#define ID_BAR_JIANMO_COMPONENTLIB     10003
#define ID_JIANMO_COMPONENT_SPLITTER     10004
#define IDC_MAIN_TOOLBAR   41003

enum {
	IDM_TOOLBAR_BUTTON_FIRST = 45001,
	IDM_TOOLBAR_BUTTON_LAST = (IDM_TOOLBAR_BUTTON_FIRST + 256),

	ID_GRID_BUTTON_FIRST = (IDM_TOOLBAR_BUTTON_LAST + 1),
	ID_GRID_BUTTON_LAST = (ID_GRID_BUTTON_FIRST + 256),

	IDM_ROBOTBAR_BUTTON_FIRST = (ID_GRID_BUTTON_LAST + 1),
	IDM_ROBOTBAR_BUTTON_LAST = (IDM_ROBOTBAR_BUTTON_FIRST + 256),

	IDM_HISTORY_MODEL_START = (IDM_ROBOTBAR_BUTTON_LAST + 1),
	IDM_HISTORY_MODEL_LAST = (IDM_HISTORY_MODEL_START + 256),

	IDM_VIEW_START = (IDM_HISTORY_MODEL_LAST + 1),
	IDM_VIEW_LAST = (IDM_VIEW_START + 256)
};

extern  const char * g_strTitleJianMo;
extern  const char * g_strTitleWorkHistory;

#include <vector>
using namespace std;

template <class T>
void PtrVectorClear(vector<T*>& vTData)
{
	vector<T*>::iterator iter;
	for (iter = vTData.begin(); iter != vTData.end(); ++iter)
	{
		delete (*iter);
		*iter = NULL;
	}
	vTData.clear();
}