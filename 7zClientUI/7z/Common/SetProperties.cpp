// SetProperties.cpp

#include "StdAfx.h"

#include "SetProperties.h"

#include "../Windows/PropVariant.h"
#include "MyString.h"
#include "StringToInt.h"
#include "MyCom.h"

#include "../Archive/IArchive.h"

using namespace NWindows;
using namespace NCOM;

UInt64 ConvertStringToUInt64(const wchar_t *s, const wchar_t **end)
{
	UInt64 result = 0;
	for (;;)
	{
		wchar_t c = *s;
		if (c < '0' || c > '9')
		{
			if (end != NULL)
				*end = s;
			return result;
		}
		result *= 10;
		result += (c - '0');
		s++;
	}
}

static void ParseNumberString(const UString &s, NCOM::CPropVariant &prop)
{
  const wchar_t *endPtr;
  UInt64 result = ConvertStringToUInt64(s, &endPtr);
  if (endPtr - (const wchar_t *)s != s.Length())
    prop = s;
  else if (result <= 0xFFFFFFFF)
    prop = (UInt32)result;
  else
    prop = result;
}

HRESULT SetProperties(IUnknown *unknown, const CObjectVector<CProperty> &properties)
{
  if (properties.IsEmpty())
    return S_OK;
  CMyComPtr<ISetProperties> setProperties;
  unknown->QueryInterface(IID_ISetProperties, (void **)&setProperties);
  if (!setProperties)
    return S_OK;

  UStringVector realNames;
  CPropVariant *values = new CPropVariant[properties.Size()];
  try
  {
    int i;
    for(i = 0; i < properties.Size(); i++)
    {
      const CProperty &property = properties[i];
      NCOM::CPropVariant propVariant;
      UString name = property.Name;
      if (property.Value.IsEmpty())
      {
        if (!name.IsEmpty())
        {
          wchar_t c = name.Back();
          if (c == L'-')
            propVariant = false;
          else if (c == L'+')
            propVariant = true;
          if (propVariant.vt != VT_EMPTY)
            name.DeleteBack();
        }
      }
      else
        ParseNumberString(property.Value, propVariant);
      realNames.Add(name);
      values[i] = propVariant;
    }
    CRecordVector<const wchar_t *> names;
    for(i = 0; i < realNames.Size(); i++)
      names.Add((const wchar_t *)realNames[i]);
    
    RINOK(setProperties->SetProperties(&names.Front(), values, names.Size()));
  }
  catch(...)
  {
    delete []values;
    throw;
  }
  delete []values;
  return S_OK;
}
