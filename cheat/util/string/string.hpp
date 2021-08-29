#pragma once
#include <Windows.h>
#include <string>

namespace util {
	namespace string {
		// wide -> multi-byte
		__forceinline std::string WideToMultiByte( const std::wstring& str ) {
			// Get WideCharToMultiByte function
			using WideCharToMultiByte_t = int( __stdcall* )( unsigned int , unsigned long , const wchar_t* , int , char* , int , const char* , int );
			static WideCharToMultiByte_t _pWideCharToMultiByte = NULL;
			if ( !_pWideCharToMultiByte ) {
				_pWideCharToMultiByte = ( WideCharToMultiByte_t ) GetProcAddress( GetModuleHandleA( "kernel32.dll" ) , "WideCharToMultiByte" );

				if ( !_pWideCharToMultiByte ) {
					return std::string( );
				}
			}

			std::string ret;
			int         str_len;

			// check if not empty str
			if ( str.empty( ) )
				return std::string( );

			// count size
			str_len = _pWideCharToMultiByte( CP_UTF8 , 0 , str.data( ) , ( int ) str.size( ) , 0 , 0 , 0 , 0 );

			// setup return value
			ret = std::string( str_len , 0 );

			// final conversion
			_pWideCharToMultiByte( CP_UTF8 , 0 , str.data( ) , ( int ) str.size( ) , &ret [ 0 ] , str_len , 0 , 0 );

			return ret;
		}

		// multi-byte -> wide
		__forceinline std::wstring MultiByteToWide( const std::string& str ) {
			// Get MultiByteToWideChar function
			using MultiByteToWideChar_t = int( __stdcall* )( unsigned int , unsigned long , const char* , int , wchar_t* , int );
			static MultiByteToWideChar_t _pMultiByteToWideChar = NULL;
			if ( !_pMultiByteToWideChar ) {
				_pMultiByteToWideChar = ( MultiByteToWideChar_t ) GetProcAddress( GetModuleHandleA( "kernel32.dll" ) , "MultiByteToWideChar" );

				if ( !_pMultiByteToWideChar ) {
					return std::wstring( );
				}
			}

			std::wstring    ret;
			int		        str_len;

			// check if not empty str
			if ( str.empty( ) )
				return std::wstring( );

			// count size
			str_len = _pMultiByteToWideChar( CP_UTF8 , 0 , str.data( ) , ( int ) str.size( ) , nullptr , 0 );

			// setup return value
			ret = std::wstring( str_len , 0 );

			// final conversion
			_pMultiByteToWideChar( CP_UTF8 , 0 , str.data( ) , ( int ) str.size( ) , &ret [ 0 ] , str_len );

			return ret;
		}
	}
}
