#ifndef __UTILS_H__
#define __UTILS_H__

#pragma once

        /////////////////////////////////////////////////////////////////////////////////////
        //

       

        class  CDuiString
        {
        public:
                enum { MAX_LOCAL_STRING_LEN = 63 };

                CDuiString();
                CDuiString(const TCHAR ch);
                CDuiString(const CDuiString& src);
                CDuiString(LPCTSTR lpsz, int nLen = -1);
                ~CDuiString();

                void Empty();
                int GetLength() const;
                bool IsEmpty() const;
                TCHAR GetAt(int nIndex) const;
                void Append(LPCTSTR pstr);
                void Assign(LPCTSTR pstr, int nLength = -1);
                LPCTSTR GetData() const;

                void SetAt(int nIndex, TCHAR ch);
                operator LPCTSTR() const;

                TCHAR operator[] (int nIndex) const;
                const CDuiString& operator=(const CDuiString& src);
                const CDuiString& operator=(const TCHAR ch);
                const CDuiString& operator=(LPCTSTR pstr);
#ifdef _UNICODE
                const CDuiString& CDuiString::operator=(LPCSTR lpStr);
                const CDuiString& CDuiString::operator+=(LPCSTR lpStr);
#else
                const CDuiString& CDuiString::operator=(LPCWSTR lpwStr);
                const CDuiString& CDuiString::operator+=(LPCWSTR lpwStr);
#endif
                CDuiString operator+(const CDuiString& src) const;
                CDuiString operator+(LPCTSTR pstr) const;
                const CDuiString& operator+=(const CDuiString& src);
                const CDuiString& operator+=(LPCTSTR pstr);
                const CDuiString& operator+=(const TCHAR ch);

                bool operator == (LPCTSTR str) const;
                bool operator != (LPCTSTR str) const;
                bool operator <= (LPCTSTR str) const;
                bool operator <  (LPCTSTR str) const;
                bool operator >= (LPCTSTR str) const;
                bool operator >  (LPCTSTR str) const;

                int Compare(LPCTSTR pstr) const;
                int CompareNoCase(LPCTSTR pstr) const;

                void MakeUpper();
                void MakeLower();

                CDuiString Left(int nLength) const;
                CDuiString Mid(int iPos, int nLength = -1) const;
                CDuiString Right(int nLength) const;

                int Find(TCHAR ch, int iPos = 0) const;
                int Find(LPCTSTR pstr, int iPos = 0) const;
                int ReverseFind(TCHAR ch) const;
                int Replace(LPCTSTR pstrFrom, LPCTSTR pstrTo);

                int __cdecl Format(LPCTSTR pstrFormat, ...);
        int __cdecl Format(LPCTSTR pstrFormat, va_list Args);
                int __cdecl SmallFormat(LPCTSTR pstrFormat, ...);

        protected:
                LPTSTR m_pstr;
                TCHAR m_szBuffer[MAX_LOCAL_STRING_LEN + 1];
        };


        class CVariant : public VARIANT
        {
        public:
                CVariant()
                {
                        VariantInit(this);
                }
                CVariant(int i)
                {
                        VariantInit(this);
                        this->vt = VT_I4;
                        this->intVal = i;
                }
                CVariant(float f)
                {
                        VariantInit(this);
                        this->vt = VT_R4;
                        this->fltVal = f;
                }
                CVariant(LPOLESTR s)
                {
                        VariantInit(this);
                        this->vt = VT_BSTR;
                        this->bstrVal = s;
                }
                CVariant(IDispatch *disp)
                {
                        VariantInit(this);
                        this->vt = VT_DISPATCH;
                        this->pdispVal = disp;
                }

                ~CVariant()
                {
                        VariantClear(this);
                }
        };



#endif // __UTILS_H__
