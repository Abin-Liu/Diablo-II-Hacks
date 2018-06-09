#ifndef __STRSPLIT_H__
#define __STRSPLIT_H__

int StrSplit(const CString &str, CStringArray& arr, const CString& sDelimitior);
int StrSplit(const CString &str, CStringArray& arr, TCHAR chDelimitior);

#endif