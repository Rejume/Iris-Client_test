#pragma once
//ターゲットバージョン
//#define _WIN32_WINNT 0x0601 //Windows 7
//#define _WIN32_WINNT 0x0602 // Windows 8
#define _WIN32_WINNT 0x0A00  // Windows 10

//プリコンパイル済みヘッダーロード済みの場合ヘッダファイルincludeしない ↓のifndef endifをhppに追加
#define __PRECOMPILEHEADERLOADED__ 
//#ifndef __PRECOMPILEHEADERLOADED__
//#endif

#pragma region CPP DEFAULT HEADER
#include <boost/asio.hpp>//windows.h(winsock.h)より前に・・・
#include <boost/bind.hpp>
#include <Windows.h>

#include <MMSystem.h>					//タイマー処理
#include <time.h>
#pragma comment(lib,"winmm.lib")		//タイマー処理に必要

#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <memory>

#include <boost/thread.hpp>
#include <conio.h>
#include <random>

#include <vector>
#include <list>
#pragma endregion CPP DEFAULT HEADER


//メモリリーク検出用
#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
///<sumary>macro new</sumary>
//<remarks>Debugのみ：メモリリーク検出</remarks>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#pragma region USER DEFINED MACROS
///ループ省略化
#pragma region MACRO FOR LOOP
#ifndef forto
///<sumary>for省略用マクロ ++i</sumary>
//<remarks>カウントアップ　LoopIndex(i)固定　toまでLoop</remarks>
#define forto(to) for(int i = 0;i < (const int)to;++i)
///<sumary>for省略用マクロ ++i</sumary>
//<remarks>カウントアップ LoopIndex (idx)指定 toまでLoop</remarks>
#define forto_idx(to,idx) for(int idx= 0;i < (const int)to;++idx)
#define forfrom(to) for(int i = to-1;i >=0;--i)
#endif
#pragma endregion MACRO FOR LOOP
///セーフデリート
#pragma region MACRO SAFE DELETER
#ifndef SAFE_DELETE
#define SAFE_DELETE(x) { if (x) { delete (x); (x)=nullptr; } }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) { if (x) { delete[] (x); (x)=nullptr; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) { if (x) { (x)->Release(); (x)=nullptr; } }
#endif
#pragma endregion MACRO SAFE DELETER
///簡易タイマーセット //後程デバッグ用レポートを内蔵させる
#pragma region MACRO TIMER SET
#ifndef SETTIMERVARIABLE
#define MCSETTIMERVARIABLE \
	long clock_, time_;
#endif
#ifndef MCTIMERBEGIN
#define MCTIMERBEGIN \
	time_ = timeGetTime(); \
	clock_ = clock();
#endif
#ifndef MCTIMEREND
#define MCTIMEREND(str) \
	clock_ = clock() - clock_; \
	time_ = timeGetTime() - time_; \
	std::cout << str << " clock(ms) = " << clock_ << "time(ms) = " << time_ << std::endl;
#endif
#pragma endregion  MACRO TIMER SET
///エラーメッセージ表示
#pragma region MACRO ERRORMSG
///show Error Message to Console
#ifndef ELOG
#define ELOG( x, ... )      printf_s( "[File:%s, Line:%d] "x"\n", __FILE__, __LINE__, ##__VA_ARGS__ )
#endif
///show Error MessageBox
#ifndef EMSG
inline void MacroErrorMessage(const char* msg, const char* file, const int line_) {
	std::stringstream st;
	st << "file:" << file << " Line:" << line_ << std::endl
		<< msg;
	MessageBoxA(nullptr, st.str().c_str(), "error", MB_OK);
	assert(false);
}
#define EMSG( x, ... )  MacroErrorMessage(x,__FILE__,__LINE__)
#endif//ELOG
#pragma endregion MACRO ERRORMSG
///文字列変換
#pragma region MACRO NAMETOSTRING
#ifndef TO_STRING
///<sumary>macro 文字列変換マクロ[文字列化演算子]</sumary>
//<remarks>変数をその"変数名"の文字列に変換 
//例）int hoge = 3; TO_STRING(hoge) ⇒ "hoge"
//変数の中身が文字列化したものがほしい場合はstd::to_string()を使用してください。例）std::to_string(hoge)　⇒ string("3")</remarks>
#define TO_STRING(VariableName) # VariableName
#endif
#ifndef CONNECT_VALUENAME
///<sumary>macro 変数名連結[トークン連結演算子]</sumary>
//<remarks>複数の文字を連結して1つの文字列に変換します。多分ほとんど使い道ないです。 </remarks>
#define CONNECT_VALUENAME(l,r) l##r 
#endif
#pragma endregion MACRO NAMETOSTRING

#pragma endregion USER DEFINED MACROS

