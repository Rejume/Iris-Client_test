#pragma once

#ifndef __PRECOMPILEHEADERLOADED__
#include <iostream>
#endif

namespace etd{
	///<sumary>データ格納バッファクラス</sumary>
	//<remarks>'\0'が来ても格納を続ける</remarks>
	class DataBuf{
		int datasize = 0;
	public:
		int GetSize(){ return datasize; }
		char* data = nullptr;
		DataBuf() = default;
		///<sumary>生成 </sumary>
		//<remarks>コピー</remarks>
		DataBuf(const char* src_, int srcsize_){
			NonCheckInsert(src_, srcsize_);
		}
		///<sumary>高速生成 </sumary>
		//<remarks>※src_がnullptrに変化します　	※datasizeが規定されません
		//使用例 char* "hogehoge";	DataBuf hoge(hogehoge);</remarks>
		DataBuf(char*& src_){
			data = src_;
			src_ = nullptr;
		}
		///<sumary>高速生成</sumary>
		//<remarks>ムーブ</remarks>
		DataBuf(DataBuf&& r){
			NonCheckMove(r);
		}
		///<sumary>生成 </sumary>
		//<remarks>コピー</remarks>
		DataBuf(const DataBuf& r){
			NonCheckInsert(r.data, r.datasize);
		}
		///<sumary>領域のみ確保</sumary>
		//<remarks></remarks>
		DataBuf(int datasize_){
			datasize = datasize_;
			data = new char[datasize];
		}
		~DataBuf(){
			Release();
		}
		///コピー
		inline DataBuf& operator=(const DataBuf& r){
			Insert_withRelease(r.data, r.datasize);
			return *this;
		}
		///<sumary>ムーブ</sumary>
		//<remarks>自身から自身へのムーブは想定していません・・・</remarks>
		inline void operator=(DataBuf&& r){
			Release();
			NonCheckMove(r);
		}
		///<sumary>ムーブ</sumary>
		//<remarks>※src_がnullptrに変化します　	※datasizeが規定されません</remarks>
		inline void operator=(char*&& r){
			Release();
			data = r;
			r = nullptr;
		}
		inline char& operator[](const int idx){
			return data[idx];
		}
		///<sumary>高速処理用 内部チェック無領域確保</sumary>
		//<remarks>確実に中身が入っていないと確認できるとき以外使用しないでください。</remarks>
		void NonCheckGetMemory(int memsize_){
			datasize = memsize_;
			data = new char[datasize];
		}
		///<sumary>高速処理用 内部チェック無ムーブ</sumary>
		//<remarks>確実に中身が入っていないと確認できるとき以外使用しないでください。</remarks>
		void NonCheckMove(DataBuf& r){
			data = r.data;
			r.data = nullptr;
			datasize = r.datasize;
			r.datasize = 0;
		}
		///<sumary>メモリ解放</sumary>
		//<remarks>datasizeが0の場合削除しない</remarks>
		void Release(){
			if (datasize)	{
				SAFE_DELETE_ARRAY(data);
				datasize = 0;
			}
		}
		///<sumary>高速処理用 内部チェック無コピー</sumary>
		//<remarks>確実に中身が入っていないと確認できるとき以外使用しないでください。</remarks>
		void NonCheckInsert(const char* src_, int srcsize_){
			data = new char[srcsize_];
			memcpy_s(data, srcsize_, src_, srcsize_);
			datasize = srcsize_;
		}
		///<sumary>要素格納</sumary>
		//<remarks>自身から自身へのコピー時危険なので作成。</remarks>
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