#include "../stdafx.h"
#include "Utils.h"




     

        /////////////////////////////////////////////////////////////////////////////////////
        //
        //

        CDuiString::CDuiString() : m_pstr(m_szBuffer)
        {
                m_szBuffer[0] = '\0';
        }

        CDuiString::CDuiString(const TCHAR ch) : m_pstr(m_szBuffer)
        {
                m_szBuffer[0] = ch;
                m_szBuffer[1] = '\0';
        }

        CDuiString::CDuiString(LPCTSTR lpsz, int nLen) : m_pstr(m_szBuffer)
        {      
                ASSERT(!::IsBadStringPtr(lpsz,-1) || lpsz==NULL);
                m_szBuffer[0] = '\0';
                Assign(lpsz, nLen);
        }

        CDuiString::CDuiString(const CDuiString& src) : m_pstr(m_szBuffer)
        {
                m_szBuffer[0] = '\0';
                Assign(src.m_pstr);
        }

        CDuiString::~CDuiString()
        {
                if( m_pstr != m_szBuffer ) free(m_pstr);
        }

        int CDuiString::GetLength() const
        {
                return (int) _tcslen(m_pstr);
        }

        CDuiString::operator LPCTSTR() const
        {
                return m_pstr;
        }

        void CDuiString::Append(LPCTSTR pstr)
        {
                int nNewLength = GetLength() + (int) _tcslen(pstr);
                if( nNewLength >= MAX_LOCAL_STRING_LEN ) {
                        if( m_pstr == m_szBuffer ) {
                                m_pstr = static_cast<LPTSTR>(malloc((nNewLength + 1) * sizeof(TCHAR)));
                                _tcscpy(m_pstr, m_szBuffer);
                                _tcscat(m_pstr, pstr);
                        }
                        else {
                                m_pstr = static_cast<LPTSTR>(realloc(m_pstr, (nNewLength + 1) * sizeof(TCHAR)));
                                _tcscat(m_pstr, pstr);
                        }
                }
                else {
                        if( m_pstr != m_szBuffer ) {
                                free(m_pstr);
                                m_pstr = m_szBuffer;
                        }
                        _tcscat(m_szBuffer, pstr);
                }
        }

        void CDuiString::Assign(LPCTSTR pstr, int cchMax)
        {
                if( pstr == NULL ) pstr = _T("");
                cchMax = (cchMax < 0 ? (int) _tcslen(pstr) : cchMax);
                if( cchMax < MAX_LOCAL_STRING_LEN ) {
                        if( m_pstr != m_szBuffer ) {
                                free(m_pstr);
                                m_pstr = m_szBuffer;
                        }
                }
                else if( cchMax > GetLength() || m_pstr == m_szBuffer ) {
                        if( m_pstr == m_szBuffer ) m_pstr = NULL;
                        m_pstr = static_cast<LPTSTR>(realloc(m_pstr, (cchMax + 1) * sizeof(TCHAR)));
                }
                _tcsncpy(m_pstr, pstr, cchMax);
                m_pstr[cchMax] = '\0';
        }

        bool CDuiString::IsEmpty() const
        {
                return m_pstr[0] == '\0';
        }

        void CDuiString::Empty()
        {
                if( m_pstr != m_szBuffer ) free(m_pstr);
                m_pstr = m_szBuffer;
                m_szBuffer[0] = '\0';
        }

        LPCTSTR CDuiString::GetData() const
        {
                return m_pstr;
        }

        TCHAR CDuiString::GetAt(int nIndex) const
        {
                return m_pstr[nIndex];
        }

        TCHAR CDuiString::operator[] (int nIndex) const
        {
                return m_pstr[nIndex];
        }  

        const CDuiString& CDuiString::operator=(const CDuiString& src)
        {      
                Assign(src);
                return *this;
        }

        const CDuiString& CDuiString::operator=(LPCTSTR lpStr)
        {      
                if ( lpStr )
                {
                        ASSERT(!::IsBadStringPtr(lpStr,-1));
                        Assign(lpStr);
                }
                else
                {
                        Empty();
                }
                return *this;
        }

#ifdef _UNICODE

        const CDuiString& CDuiString::operator=(LPCSTR lpStr)
        {
                if ( lpStr )
                {
                        ASSERT(!::IsBadStringPtrA(lpStr,-1));
                        int cchStr = (int) strlen(lpStr) + 1;
                        LPWSTR pwstr = (LPWSTR) _alloca(cchStr);
                        if( pwstr != NULL ) ::MultiByteToWideChar(::GetACP(), 0, lpStr, -1, pwstr, cchStr) ;
                        Assign(pwstr);
                }
                else
                {
                        Empty();
                }
                return *this;
        }

        const CDuiString& CDuiString::operator+=(LPCSTR lpStr)
        {
                if ( lpStr )
                {
                        ASSERT(!::IsBadStringPtrA(lpStr,-1));
                        int cchStr = (int) strlen(lpStr) + 1;
                        LPWSTR pwstr = (LPWSTR) _alloca(cchStr);
                        if( pwstr != NULL ) ::MultiByteToWideChar(::GetACP(), 0, lpStr, -1, pwstr, cchStr) ;
                        Append(pwstr);
                }
               
                return *this;
        }

#else

        const CDuiString& CDuiString::operator=(LPCWSTR lpwStr)
        {      
                if ( lpwStr )
                {
                        ASSERT(!::IsBadStringPtrW(lpwStr,-1));
                        int cchStr = ((int) wcslen(lpwStr) * 2) + 1;
                        LPSTR pstr = (LPSTR) _alloca(cchStr);
                        if( pstr != NULL ) ::WideCharToMultiByte(::GetACP(), 0, lpwStr, -1, pstr, cchStr, NULL, NULL);
                        Assign(pstr);
                }
                else
                {
                        Empty();
                }
               
                return *this;
        }

        const CDuiString& CDuiString::operator+=(LPCWSTR lpwStr)
        {
                if ( lpwStr )
                {
                        ASSERT(!::IsBadStringPtrW(lpwStr,-1));
                        int cchStr = ((int) wcslen(lpwStr) * 2) + 1;
                        LPSTR pstr = (LPSTR) _alloca(cchStr);
                        if( pstr != NULL ) ::WideCharToMultiByte(::GetACP(), 0, lpwStr, -1, pstr, cchStr, NULL, NULL);
                        Append(pstr);
                }
               
                return *this;
        }

#endif // _UNICODE

        const CDuiString& CDuiString::operator=(const TCHAR ch)
        {
                Empty();
                m_szBuffer[0] = ch;
                m_szBuffer[1] = '\0';
                return *this;
        }

        CDuiString CDuiString::operator+(const CDuiString& src) const
        {
                CDuiString sTemp = *this;
                sTemp.Append(src);
                return sTemp;
        }

        CDuiString CDuiString::operator+(LPCTSTR lpStr) const
        {
                if ( lpStr )
                {
                        ASSERT(!::IsBadStringPtr(lpStr,-1));
                        CDuiString sTemp = *this;
                        sTemp.Append(lpStr);
                        return sTemp;
                }

                return *this;
        }

        const CDuiString& CDuiString::operator+=(const CDuiString& src)
        {      
                Append(src);
                return *this;
        }

        const CDuiString& CDuiString::operator+=(LPCTSTR lpStr)
        {      
                if ( lpStr )
                {
                        ASSERT(!::IsBadStringPtr(lpStr,-1));
                        Append(lpStr);
                }
               
                return *this;
        }

        const CDuiString& CDuiString::operator+=(const TCHAR ch)
        {      
                TCHAR str[] = { ch, '\0' };
                Append(str);
                return *this;
        }

        bool CDuiString::operator == (LPCTSTR str) const { return (Compare(str) == 0); };
        bool CDuiString::operator != (LPCTSTR str) const { return (Compare(str) != 0); };
        bool CDuiString::operator <= (LPCTSTR str) const { return (Compare(str) <= 0); };
        bool CDuiString::operator <  (LPCTSTR str) const { return (Compare(str) <  0); };
        bool CDuiString::operator >= (LPCTSTR str) const { return (Compare(str) >= 0); };
        bool CDuiString::operator >  (LPCTSTR str) const { return (Compare(str) >  0); };

        void CDuiString::SetAt(int nIndex, TCHAR ch)
        {
                ASSERT(nIndex>=0 && nIndex<GetLength());
                m_pstr[nIndex] = ch;
        }

        int CDuiString::Compare(LPCTSTR lpsz) const
        {
                return _tcscmp(m_pstr, lpsz);
        }

        int CDuiString::CompareNoCase(LPCTSTR lpsz) const
        {
                return _tcsicmp(m_pstr, lpsz);
        }

        void CDuiString::MakeUpper()
        {
                _tcsupr(m_pstr);
        }

        void CDuiString::MakeLower()
        {
                _tcslwr(m_pstr);
        }

        CDuiString CDuiString::Left(int iLength) const
        {
                if( iLength < 0 ) iLength = 0;
                if( iLength > GetLength() ) iLength = GetLength();
                return CDuiString(m_pstr, iLength);
        }

        CDuiString CDuiString::Mid(int iPos, int iLength) const
        {
                if( iLength < 0 ) iLength = GetLength() - iPos;
                if( iPos + iLength > GetLength() ) iLength = GetLength() - iPos;
                if( iLength <= 0 ) return CDuiString();
                return CDuiString(m_pstr + iPos, iLength);
        }

        CDuiString CDuiString::Right(int iLength) const
        {
                int iPos = GetLength() - iLength;
                if( iPos < 0 ) {
                        iPos = 0;
                        iLength = GetLength();
                }
                return CDuiString(m_pstr + iPos, iLength);
        }

        int CDuiString::Find(TCHAR ch, int iPos /*= 0*/) const
        {
                ASSERT(iPos>=0 && iPos<=GetLength());
                if( iPos != 0 && (iPos < 0 || iPos >= GetLength()) ) return -1;
                LPCTSTR p = _tcschr(m_pstr + iPos, ch);
                if( p == NULL ) return -1;
                return (int)(p - m_pstr);
        }

        int CDuiString::Find(LPCTSTR pstrSub, int iPos /*= 0*/) const
        {
                ASSERT(!::IsBadStringPtr(pstrSub,-1));
                ASSERT(iPos>=0 && iPos<=GetLength());
                if( iPos != 0 && (iPos < 0 || iPos > GetLength()) ) return -1;
                LPCTSTR p = _tcsstr(m_pstr + iPos, pstrSub);
                if( p == NULL ) return -1;
                return (int)(p - m_pstr);
        }

        int CDuiString::ReverseFind(TCHAR ch) const
        {
                LPCTSTR p = _tcsrchr(m_pstr, ch);
                if( p == NULL ) return -1;
                return (int)(p - m_pstr);
        }

        int CDuiString::Replace(LPCTSTR pstrFrom, LPCTSTR pstrTo)
        {
                CDuiString sTemp;
                int nCount = 0;
                int iPos = Find(pstrFrom);
                if( iPos < 0 ) return 0;
                int cchFrom = (int) _tcslen(pstrFrom);
                int cchTo = (int) _tcslen(pstrTo);
                while( iPos >= 0 ) {
                        sTemp = Left(iPos);
                        sTemp += pstrTo;
                        sTemp += Mid(iPos + cchFrom);
                        Assign(sTemp);
                        iPos = Find(pstrFrom, iPos + cchTo);
                        nCount++;
                }
                return nCount;
        }
   
    int CDuiString::Format(LPCTSTR pstrFormat, va_list Args)
    {
#if _MSC_VER <= 1400

        TCHAR *szBuffer = NULL;
        int size = 512, nLen, counts;

        //
        //  allocate with init size
        //

        szBuffer = (TCHAR*)malloc(size);
        ZeroMemory(szBuffer, size);

        while (TRUE){
            counts = size / sizeof(TCHAR);
            nLen = _vsntprintf (szBuffer, counts, pstrFormat, Args);
            if (nLen != -1 && nLen < counts){
                break;
            }

            //
            //  expand the buffer.
            //

            if (nLen == -1){
                size *= 2;
            }else{
                size += 1 * sizeof(TCHAR);
            }


            //
            //  realloc the buffer.
            //

            if ((szBuffer = (TCHAR*)realloc(szBuffer, size)) != NULL){
                ZeroMemory(szBuffer, size);
            }else{
                break;
            }

        }

        Assign(szBuffer);
        free(szBuffer);
        return nLen;
#else
        int nLen, totalLen;
        TCHAR *szBuffer;

        nLen = _vsntprintf(NULL, 0, pstrFormat, Args);
        totalLen = (nLen + 1)*sizeof(TCHAR);
        szBuffer = (TCHAR*)malloc(totalLen);
        ZeroMemory(szBuffer, totalLen);
        nLen = _vsntprintf(szBuffer, nLen + 1, pstrFormat, Args);

        Assign(szBuffer);
        free(szBuffer);

        return nLen;

#endif
    }

    int CDuiString::Format(LPCTSTR pstrFormat, ...)
    {
        int nRet;
        va_list Args;

        va_start(Args, pstrFormat);
        nRet = Format(pstrFormat, Args);
        va_end(Args);

        return nRet;

    }

        int CDuiString::SmallFormat(LPCTSTR pstrFormat, ...)
        {
                CDuiString sFormat = pstrFormat;
                TCHAR szBuffer[64] = { 0 };
                va_list argList;
                va_start(argList, pstrFormat);
                int iRet = ::_vsntprintf(szBuffer, sizeof(szBuffer), sFormat, argList);
                va_end(argList);
                Assign(szBuffer);
                return iRet;
        }

        /////////////////////////////////////////////////////////////////////////////
        //
        //

        static UINT HashKey(LPCTSTR Key)
        {
                UINT i = 0;
                SIZE_T len = _tcslen(Key);
                while( len-- > 0 ) i = (i << 5) + i + Key[len];
                return i;
        }

        static UINT HashKey(const CDuiString& Key)
        {
                return HashKey((LPCTSTR)Key);
        };