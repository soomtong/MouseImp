/**********************************************************************
thunk's (for func's call) - based on ATL thunk's (Intel only)
**********************************************************************/

#ifndef __SLTHUNK_H
#define __SLTHUNK_H

#include "SLUtil.h"

#include "SLDbg.h"

_SL_BEG



//////////////////////////////////////////////////////////////////////
//this code hardly based on ATL's Thunk's and AtdAux lib



#ifndef _M_IX86
  #pragma message("for X86 only!")
#endif


//////////////////////////////////////////////////////////////////////
//CSLThunkW32

#pragma pack(push,1)

template <class T>
class CSLThunkW32
{

  BYTE    m_mov;          // mov ecx, %pThis
  DWORD   m_this;         //
  BYTE    m_jmp;          // jmp func
  DWORD   m_relproc;      // relative jmp

public:
  typedef void (T::*TMFP)();
  void InitThunk(TMFP method, const T* pThis)
  {
    union {
      DWORD func;
      TMFP method;
    } addr;
    addr.method = method;
    m_mov = 0xB9;
    m_this = (DWORD)pThis;
    m_jmp = 0xE9;
    m_relproc = addr.func - (DWORD)(this+1);
    FlushInstructionCache(GetCurrentProcess(), this, sizeof(*this));
  }
  FARPROC GetThunk() const { 
    _ASSERT(m_mov == 0xB9);
    return (FARPROC)this; }
};

#pragma pack(pop)


_SL_END

#endif//__SLTHUNK_H