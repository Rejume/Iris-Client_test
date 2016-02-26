#pragma once

#ifndef __PRECOMPILEHEADERLOADED__
#include <iostream>
#endif

namespace etd{
	///<sumary>�f�[�^�i�[�o�b�t�@�N���X</sumary>
	//<remarks>'\0'�����Ă��i�[�𑱂���</remarks>
	class DataBuf{
		int datasize = 0;
	public:
		int GetSize(){ return datasize; }
		char* data = nullptr;
		DataBuf() = default;
		///<sumary>���� </sumary>
		//<remarks>�R�s�[</remarks>
		DataBuf(const char* src_, int srcsize_){
			NonCheckInsert(src_, srcsize_);
		}
		///<sumary>�������� </sumary>
		//<remarks>��src_��nullptr�ɕω����܂��@	��datasize���K�肳��܂���
		//�g�p�� char* "hogehoge";	DataBuf hoge(hogehoge);</remarks>
		DataBuf(char*& src_){
			data = src_;
			src_ = nullptr;
		}
		///<sumary>��������</sumary>
		//<remarks>���[�u</remarks>
		DataBuf(DataBuf&& r){
			NonCheckMove(r);
		}
		///<sumary>���� </sumary>
		//<remarks>�R�s�[</remarks>
		DataBuf(const DataBuf& r){
			NonCheckInsert(r.data, r.datasize);
		}
		///<sumary>�̈�̂݊m��</sumary>
		//<remarks></remarks>
		DataBuf(int datasize_){
			datasize = datasize_;
			data = new char[datasize];
		}
		~DataBuf(){
			Release();
		}
		///�R�s�[
		inline DataBuf& operator=(const DataBuf& r){
			Insert_withRelease(r.data, r.datasize);
			return *this;
		}
		///<sumary>���[�u</sumary>
		//<remarks>���g���玩�g�ւ̃��[�u�͑z�肵�Ă��܂���E�E�E</remarks>
		inline void operator=(DataBuf&& r){
			Release();
			NonCheckMove(r);
		}
		///<sumary>���[�u</sumary>
		//<remarks>��src_��nullptr�ɕω����܂��@	��datasize���K�肳��܂���</remarks>
		inline void operator=(char*&& r){
			Release();
			data = r;
			r = nullptr;
		}
		inline char& operator[](const int idx){
			return data[idx];
		}
		///<sumary>���������p �����`�F�b�N���̈�m��</sumary>
		//<remarks>�m���ɒ��g�������Ă��Ȃ��Ɗm�F�ł���Ƃ��ȊO�g�p���Ȃ��ł��������B</remarks>
		void NonCheckGetMemory(int memsize_){
			datasize = memsize_;
			data = new char[datasize];
		}
		///<sumary>���������p �����`�F�b�N�����[�u</sumary>
		//<remarks>�m���ɒ��g�������Ă��Ȃ��Ɗm�F�ł���Ƃ��ȊO�g�p���Ȃ��ł��������B</remarks>
		void NonCheckMove(DataBuf& r){
			data = r.data;
			r.data = nullptr;
			datasize = r.datasize;
			r.datasize = 0;
		}
		///<sumary>���������</sumary>
		//<remarks>datasize��0�̏ꍇ�폜���Ȃ�</remarks>
		void Release(){
			if (datasize)	{
				SAFE_DELETE_ARRAY(data);
				datasize = 0;
			}
		}
		///<sumary>���������p �����`�F�b�N���R�s�[</sumary>
		//<remarks>�m���ɒ��g�������Ă��Ȃ��Ɗm�F�ł���Ƃ��ȊO�g�p���Ȃ��ł��������B</remarks>
		void NonCheckInsert(const char* src_, int srcsize_){
			data = new char[srcsize_];
			memcpy_s(data, srcsize_, src_, srcsize_);
			datasize = srcsize_;
		}
		///<sumary>�v�f�i�[</sumary>
		//<remarks>���g���玩�g�ւ̃R�s�[���댯�Ȃ̂ō쐬�B</remarks>
		void Insert_withRelease(const char* src_, int srcsize_){
			int oldsize = datasize;
			char* olddata = data;
			data = new char[srcsize_];
			memcpy_s(data, srcsize_, src_, srcsize_);
			datasize = srcsize_;
			if (oldsize)
				SAFE_DELETE_ARRAY(olddata);
		}
		void InsertString_Rel(const std::string& src_){
			Release();
			datasize = src_.size() +1;
			data = new char[datasize];
			strcpy_s(data,datasize, src_.c_str());
		}
	};
}