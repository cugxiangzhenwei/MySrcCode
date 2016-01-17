#pragma once
#include "StlHeader.h"

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

template <class T>
void PtrVectorCopy(vector<T*>& vDest, const vector<T*>& vSrc)
{
	PtrVectorClear(vDest);
	vector<T*>::const_iterator iter;
	T *p = NULL;
	for (iter = vSrc.begin(); iter != vSrc.end(); ++iter)
	{
		p = new T();
		*p = **iter;
		vDest.push_back(p);
	}
}

template <class T>
BOOL PtrVectorRemove(vector<T*>& vTData, T *&pData)
{
	if (pData == NULL) return FALSE;
	vector<T*>::iterator iter;
	for (iter = vTData.begin(); iter != vTData.end(); ++iter)
	{
		if (*iter == pData)
		{
			delete (*iter);
			*iter = NULL;
			vTData.erase(iter);
			pData = NULL;
			return TRUE;
		}
	}
	return FALSE;
}

template <class T>
void PtrVectorClear(vector<T*> *pvData)
{
	if (pvData == NULL) return;
	vector<T*>::iterator iter;
	for (iter = pvData->begin(); iter != pvData->end(); ++iter)
	{
		delete (*iter);
		*iter = NULL;
	}
	pvData->clear();
}

template <class T>
void PtrVectorCopy(vector<T*> *pvDest, vector<T*> *pvSrc)
{
	PtrVectorClear(pvDest);
	vector<T*>::iterator iter;
	T *p = NULL;
	for (iter = pvSrc->begin(); iter != pvSrc->end(); ++iter)
	{
		p = new T();
		*p = **iter;
		pvDest->push_back(p);
	}
}

template <class T>
BOOL PtrVectorCompare(vector<T*> *pvSrc1, vector<T*> *pvSrc2)
{
	if (pvSrc1 == pvSrc2) return TRUE;
	if (pvSrc1 == NULL || pvSrc2 == NULL) return FALSE;
	if (pvSrc1->size() != pvSrc2->size()) return FALSE;
	UINT nSize = (UINT)pvSrc1->size();
	for (UINT i=0; i<nSize; i++)
	{
		if (*pvSrc1->at(i) == *pvSrc2->at(i)) continue;
		return FALSE;
	}
	return TRUE;
}


template <class TKey, class TValue>
void PtrMapClear(map<TKey, TValue*>& vTData)
{
	map<TKey, TValue*>::iterator iter;
	for (iter = vTData.begin(); iter != vTData.end(); ++iter)
	{
		delete iter->second;
		iter->second = NULL;
	}
	vTData.clear();
}

template <class TKey, class TValue>
void PtrMapCopy(map<TKey, TValue*>& mapDest, const map<TKey, TValue*>& mapSrc)
{
	map<TKey, TValue*>::const_iterator iter;
	TValue * pValue = NULL;
	PtrMapClear(mapDest);
	for (iter = mapSrc.begin(); iter != mapSrc.end(); ++iter)
	{
		pValue = new TValue();
		*pValue = *iter->second;
		mapDest[(LPCTSTR)iter->first] = pValue;
	}
}


template <class TKey, class TValue>
BOOL PtrMapRemove(map<TKey, TValue*>& mapTData, TKey DataKey)
{
	map<TKey, TValue* >::iterator itrData = mapTData.find( DataKey );
	if (itrData == mapTData.end()) return FALSE;

	delete itrData->second;
	itrData->second = NULL;
	mapTData.erase(itrData);
	return TRUE;
}

