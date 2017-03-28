//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
// Developed by Minigraph
//
// Author:  James Stanard 
//

#pragma once
#include "stdafx.h"

namespace Math 
{
	template <typename T> __forceinline T AlignUpWithMask(T value, size_t mask)
	{
		return (T)(((size_t)value + mask) & ~mask);
	}

	template <typename T> __forceinline T AlignDownWithMask(T value, size_t mask)
	{
		return (T)((size_t)value & ~mask);
	}

	template <typename T> __forceinline T AlignUp(T value, size_t alignment)
	{
		return AlignUpWithMask(value, alignment - 1);
	}

	template <typename T> __forceinline T AlignDown(T value, size_t alignment)
	{
		return AlignDownWithMask(value, alignment - 1);
	}

	template <typename T> __forceinline bool IsAligned(T value, size_t alignment)
	{
		return 0 == ((size_t)value & (alignment - 1));
	}

	template <typename T> __forceinline T DivideByMultiple(T value, size_t alignment)
	{
		return (T)((value + alignment - 1) / alignment);
	}

	template <typename T> __forceinline bool IsPowerOfTwo(T value)
	{
		return 0 == (value & (value - 1));
	}

	template <typename T> __forceinline bool IsDivisible(T value, T divisor)
	{
		return (value / divisor) * divisor == value;
	}
}

namespace Utility
{
	inline void Print( const char* msg ) { printf(msg); }
	inline void Print( const wchar_t* msg ) { wprintf(msg); }

	inline void Printf( const char* format, ... )
	{
		char buffer[256];
		va_list ap;
		va_start(ap, format);
		vsprintf_s(buffer, 256, format, ap);
		Print(buffer);
	}

	inline void Printf( const wchar_t* format, ... )
	{
		wchar_t buffer[256];
		va_list ap;
		va_start(ap, format);
		vswprintf(buffer, 256, format, ap);
		Print(buffer);
	}

	void ShowWindows(TCHAR Info[], TCHAR Title[])
	{
		MessageBox(NULL, Info, Title, 0);
	}

#ifndef RELEASE
	inline void PrintSubMessage( const char* format, ... )
	{
		Print("--> ");
		char buffer[256];
		va_list ap;
		va_start(ap, format);
		vsprintf_s(buffer, 256, format, ap);
		Print(buffer);
		Print("\n");
	}
	inline void PrintSubMessage( const wchar_t* format, ... )
	{
		Print("--> ");
		wchar_t buffer[256];
		va_list ap;
		va_start(ap, format);
		vswprintf(buffer, 256, format, ap);
		Print(buffer);
		Print("\n");
	}
	inline void PrintSubMessage( void )
	{
	}
#endif

} // namespace Utility

#ifdef ERROR
#undef ERROR
#endif
#ifdef ASSERT
#undef ASSERT
#endif
#ifdef HALT
#undef HALT
#endif

#define HALT( ... ) ERROR( __VA_ARGS__ ) __debugbreak();

#ifdef RELEASE

	#define ASSERT( isTrue, ... ) (void)(isTrue)
	#define WARN_ONCE_IF( isTrue, ... ) (void)(isTrue)
	#define WARN_ONCE_IF_NOT( isTrue, ... ) (void)(isTrue)
	#define ERROR( msg, ... )
	#define DEBUGPRINT( msg, ... ) do {} while(0)
	#define ASSERT_SUCCEEDED( hr, ... ) (void)(hr)

#else	// !RELEASE

	#define STRINGIFY(x) #x
	#define STRINGIFY_BUILTIN(x) STRINGIFY(x)
	#define ASSERT( isFalse, ... ) \
		if (!(bool)(isFalse)) { \
			Utility::Print("\nAssertion failed in " STRINGIFY_BUILTIN(__FILE__) " @ " STRINGIFY_BUILTIN(__LINE__) "\n"); \
			Utility::PrintSubMessage("\'" #isFalse "\' is false"); \
			Utility::PrintSubMessage(__VA_ARGS__); \
			Utility::Print("\n"); \
			__debugbreak(); \
		}

	#define ASSERT_SUCCEEDED( hr, ... ) \
		if (FAILED(hr)) { \
			Utility::Print("\nHRESULT failed in " STRINGIFY_BUILTIN(__FILE__) " @ " STRINGIFY_BUILTIN(__LINE__) "\n"); \
			Utility::PrintSubMessage("hr = 0x%08X", hr); \
			Utility::PrintSubMessage(__VA_ARGS__); \
			Utility::Print("\n"); \
			__debugbreak(); \
		}


	#define WARN_ONCE_IF( isTrue, ... ) \
	{ \
		static bool s_TriggeredWarning = false; \
		if ((bool)(isTrue) && !s_TriggeredWarning) { \
			s_TriggeredWarning = true; \
			Utility::Print("\nWarning issued in " STRINGIFY_BUILTIN(__FILE__) " @ " STRINGIFY_BUILTIN(__LINE__) "\n"); \
			Utility::PrintSubMessage("\'" #isTrue "\' is true"); \
			Utility::PrintSubMessage(__VA_ARGS__); \
			Utility::Print("\n"); \
		} \
	}

	#define WARN_ONCE_IF_NOT( isTrue, ... ) WARN_ONCE_IF(!(isTrue), __VA_ARGS__)

	#define ERROR( ... ) \
		Utility::Print("\nError reported in " STRINGIFY_BUILTIN(__FILE__) " @ " STRINGIFY_BUILTIN(__LINE__) "\n"); \
		Utility::PrintSubMessage(__VA_ARGS__); \
		Utility::Print("\n");

	#define DEBUGPRINT( msg, ... ) \
	Utility::Printf( msg "\n", ##__VA_ARGS__ );

#endif

#define BreakIfFailed( hr ) if (FAILED(hr)) __debugbreak()

void SIMDMemCopy( void* __restrict Dest, const void* __restrict Source, size_t NumQuadwords );
void SIMDMemFill( void* __restrict Dest, __m128 FillVector, size_t NumQuadwords );

std::wstring MakeWStr( const std::string& str );
std::string MakeStr(const std::wstring& str);