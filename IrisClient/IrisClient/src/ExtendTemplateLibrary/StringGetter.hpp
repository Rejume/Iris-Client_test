#pragma once

namespace etd {
	class StringGettor {
		const char* str;	//検索文字列
		size_t max;		//最大長
		char delimiter;	//区切り文字
		unsigned seeker = 0;
	public:
		StringGettor(const char* str_,size_t max_,char delimiter_ = ' ') {
			str = str_; max = max_; delimiter = delimiter_;
		}
		bool SearchEnd() {
			return max <= seeker;
		}
		///<sumary></sumary>
		//<remarks>ret:文字数(null含まない)</remarks>
		int GetStr(char* dst_) {
			int idx = 0;
			dst_[0] = '\0';
			while (str[seeker] == delimiter)	//初手区切りの場合無視
				seeker++;
			for (; seeker < max; seeker++) {
				if (str[seeker] == '\0' || str[seeker] == delimiter) {
					dst_[idx] = '\0';
					break;
				}
				else
					dst_[idx++] = str[seeker];
			}
			return idx;
		}		
		int GetStr(char* dst_,size_t maxb_) {
			UINT idx = 0;
			dst_[0] = '\0';
			while (str[seeker] == '\0' || str[seeker] == delimiter)	//初手区切りの場合無視
				seeker++;
			for (; seeker < max || idx > maxb_; seeker++) {
				if (str[seeker] == '\0' || str[seeker] == delimiter) {
					break;
				}
				else
					dst_[idx++] = str[seeker];
			}
			dst_[idx] = '\0';
			return idx;
		}
		int IsEqual(const char* str_) {
			char buf[512];
			GetStr(buf, 512);
			return std::strcmp(str_, buf) == 0;
		}
		//
		int GetInteger() {
			try {
				char buf[64];
				GetStr(buf);
				return std::stoi(buf);
			}
			catch(std::exception e){
				ELOG("Error:整数以外の変換が行われました。%s", e.what());
				return 0;
			}
		}
	};
}