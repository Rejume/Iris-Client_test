#pragma once

namespace etd {
	class StringGettor {
		const char* str;	//����������
		size_t max;		//�ő咷
		char delimiter;	//��؂蕶��
		unsigned seeker = 0;
	public:
		StringGettor(const char* str_,size_t max_,char delimiter_ = ' ') {
			str = str_; max = max_; delimiter = delimiter_;
		}
		bool SearchEnd() {
			return max <= seeker;
		}
		///<sumary></sumary>
		//<remarks>ret:������(null�܂܂Ȃ�)</remarks>
		int GetStr(char* dst_) {
			int idx = 0;
			dst_[0] = '\0';
			while (str[seeker] == delimiter)	//�����؂�̏ꍇ����
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
			while (str[seeker] == '\0' || str[seeker] == delimiter)	//�����؂�̏ꍇ����
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
				ELOG("Error:�����ȊO�̕ϊ����s���܂����B%s", e.what());
				return 0;
			}
		}
	};
}