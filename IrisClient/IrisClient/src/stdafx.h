#pragma once
//�^�[�Q�b�g�o�[�W����
//#define _WIN32_WINNT 0x0601 //Windows 7
//#define _WIN32_WINNT 0x0602 // Windows 8
#define _WIN32_WINNT 0x0A00  // Windows 10

//�v���R���p�C���ς݃w�b�_�[���[�h�ς݂̏ꍇ�w�b�_�t�@�C��include���Ȃ� ����ifndef endif��hpp�ɒǉ�
#define __PRECOMPILEHEADERLOADED__ 
//#ifndef __PRECOMPILEHEADERLOADED__
//#endif

#pragma region CPP DEFAULT HEADER
#include <boost/asio.hpp>//windows.h(winsock.h)���O�ɁE�E�E
#include <boost/bind.hpp>
#include <Windows.h>

#include <MMSystem.h>					//�^�C�}�[����
#include <time.h>
#pragma comment(lib,"winmm.lib")		//�^�C�}�[�����ɕK�v

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


//���������[�N���o�p
#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
///<sumary>macro new</sumary>
//<remarks>Debug�̂݁F���������[�N���o</remarks>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#pragma region USER DEFINED MACROS
///���[�v�ȗ���
#pragma region MACRO FOR LOOP
#ifndef forto
///<sumary>for�ȗ��p�}�N�� ++i</sumary>
//<remarks>�J�E���g�A�b�v�@LoopIndex(i)�Œ�@to�܂�Loop</remarks>
#define forto(to) for(int i = 0;i < (const int)to;++i)
///<sumary>for�ȗ��p�}�N�� ++i</sumary>
//<remarks>�J�E���g�A�b�v LoopIndex (idx)�w�� to�܂�Loop</remarks>
#define forto_idx(to,idx) for(int idx= 0;i < (const int)to;++idx)
#define forfrom(to) for(int i = to-1;i >=0;--i)
#endif
#pragma endregion MACRO FOR LOOP
///�Z�[�t�f���[�g
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
///�ȈՃ^�C�}�[�Z�b�g //����f�o�b�O�p���|�[�g�����������
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
///�G���[���b�Z�[�W�\��
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
///������ϊ�
#pragma region MACRO NAMETOSTRING
#ifndef TO_STRING
///<sumary>macro ������ϊ��}�N��[�����񉻉��Z�q]</sumary>
//<remarks>�ϐ�������"�ϐ���"�̕�����ɕϊ� 
//��jint hoge = 3; TO_STRING(hoge) �� "hoge"
//�ϐ��̒��g�������񉻂������̂��ق����ꍇ��std::to_string()���g�p���Ă��������B��jstd::to_string(hoge)�@�� string("3")</remarks>
#define TO_STRING(VariableName) # VariableName
#endif
#ifndef CONNECT_VALUENAME
///<sumary>macro �ϐ����A��[�g�[�N���A�����Z�q]</sumary>
//<remarks>�����̕�����A������1�̕�����ɕϊ����܂��B�����قƂ�ǎg�����Ȃ��ł��B </remarks>
#define CONNECT_VALUENAME(l,r) l##r 
#endif
#pragma endregion MACRO NAMETOSTRING

#pragma endregion USER DEFINED MACROS

