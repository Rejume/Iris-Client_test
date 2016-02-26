//�g���e���v���[�g���C�u����
//����F����
//�g�����Fetd::�����Ă��낢��ƁA
//���e�F�֗������ȋ@�\���v�������珑���Ă����w�b�_�[
//c++��stl�Ȃǂ̋@�\��p�����֗��@�\�������ADirectX�Ȃǃn�[�h�ɉe���������̂͏����Ȃ�
#pragma once
//ExtendTemplateLibrary
//STL�̋@�\�g���p�̃e���v���[�g�Q
//�v���v��(stdafx)�ɒǉ�
//�쐬�@����@2015_0531

namespace etd{
	//for_each�ȗ���
	//�������R���e�i(vector�Ȃ�)�������ɂ�mem_fun_ref(�֐���)�ŋ��߂��l���w��
	//�����ݒ肵�����Ƃ��Ƃ���bind2nd���g�p
	//�g�p��(CManiac) for_each(vecmaniac, bind2nd(mem_fun_ref(&CManiac::SetiHowManiac), 2));
	template < typename T_container, typename T_function>
	T_function for_each(T_container& rcontainer, T_function mem_fun_ref_func){
		return std::for_each(rcontainer.begin(), rcontainer.end(), mem_fun_ref_func);
	}
	//���������(nullptr��delete��c++�ł͗L���Ȃ̂Ŕ�r���Ȃ��j
	template < typename T_type>
	void Safe_Delete(T_type& ttype_){
		delete ttype_;
		ttype_ = nullptr;
	}
	template < typename T_type>
	void Safe_DeleteAlloc(T_type& ttype_){
		delete[] ttype_;
		ttype_ = nullptr;
	}

	template < typename T_type>
	class classobj{
		T_type* _ptr;
		classobj(){
			_ptr = nullptr;
		}
		void Make(T_type* newptr_){
			if (_ptr != nullptr){
				delete _ptr;
				_ptr = nullptr;
			}
			_ptr = newptr_;
		}
		T_type& Get(){
			return *_ptr;
		}
		~classobj(){
			delete _ptr;
			_ptr = nullptr;
		}
	};

	//�y�ʔ�abs(�����̔�r)
	template < typename T_type>
	T_type absif(T_type type){
		if (type < 0)
			type = -type;
	}

	//10��n��̒l���擾����(10��ȏ��int�̊i�[�O�Ȃ̂Ŏ���)
	//���x�d���ŃZ�[�t�e�B�|���ĂȂ��̂Ŏg�p���Őݒ肵�Ă�������
	static const int Pow10i[] = {
		1, 10, 100, 1000, 10000, 100000, 1000000,
		10000000, 100000000, 1000000000, //9��
	};
	//���l�𕶎��ω�(0~9)
	static const char IntegerToChar0to9[] = {
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
	};

	//���ƍ����o�C�i�����e�L�X�g�t�@�C�����[�_�[�N���X
	class BinaryTextLoader{
	protected:
		char* pfiledata;
		unsigned filesize;
		char* LoadFileBinary(const char* filename){
			//�t�@�C�����Ƃ肠�����o�b�t�@�ɓ����
			std::ifstream ifs(filename, std::ios::binary);
			if (ifs.fail()){//�t�@�C���I�[�v���Ɏ��s���������
				ifs.close();
				std::cout << filename << "�t�@�C���I�[�v���Ɏ��s���܂����B" << std::endl;
				MessageBoxA(nullptr, filename, "etl::BinaryTextLoader\n�t�@�C���I�[�v���Ɏ��s���܂���", MB_OK);
				return nullptr;
			}
			ifs.seekg(0, std::ios::end);
			filesize = (unsigned)ifs.tellg();
			pfiledata = new char[filesize];
			ifs.seekg(0, std::ios::beg);
			ifs.read(pfiledata, filesize);
			ifs.close();
			return pfiledata;
		}
		BinaryTextLoader(){}	//�g�p���Ȃ�
	public:
		BinaryTextLoader(const char* filename){
			filesize = 0;
			LoadFileBinary(filename);
		}
		const char* GetBinaryData(){ return pfiledata; }
		virtual ~BinaryTextLoader(){
			Safe_DeleteAlloc(pfiledata);
		}
		//�o�C�i���f�[�^����dest_�ɑ΂���copysize_�o�C�g�R�s�[
		//�J�n�ʒu��seeker_�Ŏw��(�t�@�C���̊J�n�n�_�ł���Ȃ��0�w��)
		//�߂�l��seeker_���i�񂾐��seek�n�_
		int CopyBinaryData(void* dest_, int copysize_, int seeker_){
			memcpy(dest_, pfiledata + seeker_, copysize_);
			return seeker_ + copysize_;
		}
		//�o�C�i���f�[�^���當����(null�������܂ށj���擾����
		//�J�n�ʒu��seeker_�Ŏw��(�t�@�C���̊J�n�n�_�ł���Ȃ��0�w��)
		//strmax �͕����������w��
		//�߂�l��seeker_���i�񂾐��seek�n�_
		int CopyString(char* dest_, int seeker_, const int strmax_){
			int i = 0;
			for (; i < strmax_; ++i){
				char c = *(pfiledata + seeker_ + i);
				dest_[i] = c;
				if (c == '\0'){
					//�T���I��
					return seeker_ + i + 1;
				}

			}
			//���s
			MessageBoxA(nullptr, "ExtendTemlateLibrary.hpp", "BinaryTextLoader->CopyString�ɂ�\n������ǂݍ��݂Ɏ��s���܂����B", MB_OK);
			return -1;

		}
		template <typename T>
		int Read(T* dest_, int seeker_){
			memcpy(dest_, pfiledata + seeker_, sizeof(T));
			return seeker_ + sizeof(T);
		}	
		template <typename T>
		int Read(T* dest_,unsigned elementsnum, int seeker_){
				memcpy(dest_, pfiledata + seeker_, sizeof(T)*elementsnum);
				return seeker_ + sizeof(T)*elementsnum;
			}
		//UTF-16�Ή���//���s��
		int CopyStringUTF16(char* dest_, int seeker_, const int strmax_){
			int i = 0;
			for (; i < strmax_; ++i){
				char c = *(pfiledata + seeker_ + i);
				char c2 = *(pfiledata + seeker_ + i + 1);
				char c3 = *(pfiledata + seeker_ + i + 2);
				dest_[i] = c;
				if (c == '\0' && c2 == '\0'&& c3 == '\0'){
					//�T���I��
					dest_[i + 1] = '\0';
					dest_[i + 2] = '\0';
					return seeker_ + i + 2;
				}

			}
			//���s
			MessageBoxA(nullptr, "ExtendTemlateLibrary.hpp", "BinaryTextLoader->CopyString�ɂ�\n������ǂݍ��݂Ɏ��s���܂����B", MB_OK);
			return -1;

		}
		unsigned GetFileSize(){ return filesize; }
		//�f�X�g���N�^�Ŏ�����������̂ŌĂ΂Ȃ��Ă��ǂ�
		void Release(){
			Safe_DeleteAlloc(pfiledata);
		}
	};
	//�����񌟍��N���X
	class StringSearcher{
		const char* pfiledata;
		unsigned filesize;
	public:
		//�f�t�H���g�R���X�g���N�^�Ȃ邾���Ă΂Ȃ�
		StringSearcher(){
			pfiledata = nullptr;
			filesize = 0;
		}
		void SetData(const char* filedata_){
			if (filedata_ == nullptr){
				pfiledata = nullptr;
				filesize = 0;
			}
			else{
				pfiledata = filedata_;
				filesize = strlen(filedata_) + 1;
			}
		}
		StringSearcher(const char* filedata_){
			pfiledata = filedata_;
			filesize = strlen(filedata_) + 1;
		}
		//�t�@�C���T�C�Y�w�肵�ăA�h���X���i�[(\0)���܂߂�
		StringSearcher(const char* filedata_, unsigned filesize_){
			pfiledata = filedata_;
			filesize = filesize_;
		}
		StringSearcher(BinaryTextLoader& loader){
			pfiledata = loader.GetBinaryData();
			filesize = loader.GetFileSize();
		}

		unsigned GetStringSize(){ return filesize; }
		const char* GetDataPointer(){ return pfiledata; }

		int SearchStringS(const char* str_, const unsigned startpos_){
			int strlength = strlen(str_);
			for (unsigned i = startpos_; i < filesize; ++i){
				if (pfiledata[i] == str_[0]){
					int hit = 1;
					for (int j = 1; j < strlength; j++){
						if (pfiledata[i + j] != str_[j]){
							hit = 0;
							break;
						}
						hit++;
					}
					if (hit){
						//���v���Ă���Η��E
						return i;
					}
				}
			}
			return -1;
		}
		//������̖������i�[����Ă���z��̃C���f�b�N�X��Ԃ�
		int SearchStringE(const char* str_, const unsigned startpos_){
			int strlength = strlen(str_);
			int index = -1;
			index = SearchStringS(str_, startpos_);
			if (index != -1){
				index += (strlength - 1);
			}
			return index;
		}

		//�����񂪓������ǂ�������(���S��v�̏ꍇ�̂�true�Ԃ�)
		bool operator ==(StringSearcher& ss2){
			if (&ss2.pfiledata == nullptr || pfiledata == nullptr)
				return false;
			if (filesize == ss2.GetStringSize()){
				const char* sp2 = ss2.GetDataPointer();
				for (unsigned i = 0; i < filesize; i++){
					if (pfiledata[i] != sp2[i])
						return false;
				}
				return true;
			}
			return false;
		}

		//�����񂩂琮���f�[�^�擾(1�̐��l��9�����ȉ���)
		//(�w�肵���o�b�t�@�ʒu���琔���f�[�^���������āA���l�ɕϊ����Ċi�[����j
		//���������l�̊i�[����Ă����z��̖����̃C���f�b�N�X��Ԃ��A
		//			opnum�ɒl���o��
		//���s�� -1��Ԃ�
		int SearchIntergerData(int* opnum, const unsigned startpos_){
			const int BUFMAX = 10;//���l����10�𒴂���ꍇint�^�Ɏ��܂肫��Ȃ��̂Ŏ��s����
			char buf[BUFMAX];
			int numlen = 0;
			int sign = 1;//����
			for (unsigned i = startpos_; i < filesize; i++){
				if (pfiledata[i] == '-')
					sign = -1;
				else if (pfiledata[i] >= '0' && pfiledata[i] <= '9'){
					numlen = 1;
					buf[0] = pfiledata[i];
					int j = 0;
					//���l���o�b�t�@�Ɋi�[
					for (j = 0; j < BUFMAX; j++){
						if (pfiledata[i + numlen] >= '0' && pfiledata[i + numlen] <= '9'){
							buf[numlen] = pfiledata[i + numlen];
							numlen++;
						}
						else{//���l�I��
							buf[numlen] = '\0';
							break;
						}
					}
					if (j == BUFMAX)	//������璷�����l�f�[�^�΂�������ł��ꍇ���s
						return -1;
					//������𐔒l�ɕϊ�
					int ans = 0;//���l�i�[��
					for (int x = 0; x < numlen; ++x){
						ans += ((int)(buf[x] - '0')*Pow10i[numlen - 1 - x]);
					}
					ans *= sign;
					if (opnum != nullptr)
						*opnum = ans;	//���l�i�[
					return i + numlen - 1;	//�����A�h���X�Ԃ�
				}
			}
			return -1;	//���s
		}
		//�����񂩂珬���f�[�^�擾(1�̐��l��256�����ȉ��ŁE�E�E)
		//(�w�肵���o�b�t�@�ʒu���琔���f�[�^���������āA���l�ɕϊ����Ċi�[����j
		//����: .3241�@�݂����Ȑ������̃f�[�^�������Ă�ꍇ����Ɏ擾�ł��܂���
		//���������l�̊i�[����Ă����z��̖����̃C���f�b�N�X��Ԃ��A
		//			opnum�ɒl���o��
		//���s�� -1��Ԃ�
		int SearchFDecimalData(float* opnum, const unsigned startpos_){
			const int BUFMAX = 256;//���l����10�𒴂���ꍇint�^�Ɏ��܂肫��Ȃ��̂Ŏ��s����
			char buf[2][BUFMAX];
			int numlen = 0;
			int numpart[2] = { 0, 0 };//[0]�������̐��l�� [1]�������̐��l��
			float sign = 1.f;
			for (unsigned i = startpos_; i < filesize; i++){
				if (pfiledata[i] == '-'){
					sign = -1.f;
				}
				else if (pfiledata[i] >= '0' && pfiledata[i] <= '9'){
					int dotfind = 0;
					buf[dotfind][0] = pfiledata[i];
					int j = 0;
					//���l���o�b�t�@�Ɋi�[
					for (j = 0; j < BUFMAX; j++){
						if (pfiledata[i + j] >= '0' && pfiledata[i + j] <= '9'){
							//���l����
							buf[dotfind][numpart[dotfind]] = pfiledata[i + j];
							numpart[dotfind]++;
						}
						else if (pfiledata[i + j] == '.'){
							//�ǂ���(.)����
							buf[dotfind][numpart[dotfind]] = '\0';
							//numlen++;
							dotfind = 1;
						}
						else{//���l�I��
							buf[dotfind][numpart[dotfind]] = '\0';
							break;
						}
					}
					if (j == BUFMAX)	//������璷�����l�f�[�^�΂�������ł��ꍇ���s
						return -1;
					//������𐔒l�ɕϊ�
					float ans = 0;//���l�i�[��
					//�������i�[
					for (int x = 0; x < numpart[0]; ++x){
						ans += (float)((float)(buf[0][x] - '0'))*powf(10.f, (numpart[0] - x - 1.f));
					}//�������i�[
					for (int x = 0; x < numpart[1]; ++x){
						ans += ((float)(buf[1][x] - '0'))*powf(10.f, -x - 1.f);
					}
					ans *= sign;
					if (opnum != nullptr)
						*opnum = ans;	//�|�C���^�[��nullpo����Ȃ���ΐ��l�i�[
					return i + j;	//�����A�h���X�Ԃ�
				}
			}
			return -1;	//���s
		}

	};

	//���s�R�[�h���ʃN���X
	enum LFTYPE{
		LFTYPE_CRLF,	//0d0a,\n\r
		LFTYPE_LF = 0x0a,//0a,\n
		LFTYPE_CR = 0x0d,//0d,\r
	};
	class LFCheckerObj{
		virtual class LFChecker{
		public:
			char GetFirstCode(){	}
			virtual int LFCheck(const char* filedata_, const int seeker_){ return 0; }
			virtual ~LFChecker(){}
		};
		class CR :public LFChecker{
		public:
			char GetFirstCode(){ return 0x0d; }
			int LFCheck(const char* filedata_, const int seeker_){
				if (filedata_[seeker_] == 0x0d)
					return 1;
				return 0;
			}
		};
		class LF :public LFChecker{
		public:
			char GetFirstCode(){ return 0x0a; }
			int LFCheck(const char* filedata_, const int seeker_){
				if (filedata_[seeker_] == 0x0a)
					return 1;
				return 0;
			}
		};
		class CRLF :public LFChecker{
		public:
			char GetFirstCode(){ return 0x0d; }
			int LFCheck(const char* filedata_, const int seeker_){
				if (filedata_[seeker_] == 0x0d && filedata_[seeker_ + 1] == 0x0a)
					return 1;
				return 0;
			}
		};
		LFChecker* checker;
		LFCheckerObj(){}
	public:
		int CheckLF(const char* filedata_, const int seeker_){
			return checker->LFCheck(filedata_, seeker_);
		}
		char GetFirstCode(){ return checker->GetFirstCode(); }
		LFCheckerObj(LFTYPE lftype_){
			switch (lftype_){
			case LFTYPE_LF:
				checker = new LF;
				break;
			case LFTYPE_CR:
				checker = new CR;
			case LFTYPE_CRLF:
				checker = new CRLF;
			}
		}
		~LFCheckerObj(){
			etd::Safe_DeleteAlloc(checker);
		}
	};
	//CSV�ɃA�N�Z�X���邽�߂̃N���X�ꎞ�I�ɑ�ʂ̃���������܂��B
	class CSVLoaderClass{
	protected:
		int cellsize;	//1�̃Z���̃T�C�Y(�����Ă���128�Ƃ�256�Ƃ��ŁE�E)
		int cellX;		//�Z���̗�
		int cellY;		//�Z���̍s��
		char*** pppdata;	//�f�[�^�v�f��[y][x][cellsize]
		int lftype;
		int datasize;

		//char�^�̃o�b�t�@����t�@�C���f�[�^���쐬����
	public:
		//�f�X�g���N�^�Ăяo��
		void Release(){
			if (pppdata){
				for (int i = 0; i < cellY; i++){
					for (int j = 0; j < cellX; j++){
						etd::Safe_DeleteAlloc(pppdata[i][j]);
					}
					etd::Safe_DeleteAlloc(pppdata[i]);
				}
				etd::Safe_DeleteAlloc(pppdata);
			}
		}

		//CSV�t�@�C���̉����c�������o
		void SearchCSVCellLine(int* polinemaxX, int* polinemaxY, const char* filedata_, const int filesize_, const LFTYPE lftype){
			LFCheckerObj lfcheck(lftype);	//���s�R�[�h����
			int lymax = 0, lxmax = 0;
			int ly = 0, lx = 0;
			for (int i = 0; i < filesize_; ++i){
				switch (filedata_[i]){
				case LFTYPE_CR:
				case LFTYPE_LF:
					//���s����
					if (lfcheck.CheckLF(filedata_, i)){
						++ly;
						++lx;
						if (lftype == LFTYPE_CRLF)
							i++;
						if (lxmax < lx)
							lxmax = lx;	//�ő啝�����o
						lx = 0;
					}
					break;
				case ',':
					++lx;
					break;
				}
			}
			if (filedata_[filesize_ - 1] == '\n' || filedata_[filesize_ - 1] == '\r')
				ly--;
			lymax = ly;
			lymax++;
			lxmax++;
			*polinemaxX = lxmax;
			*polinemaxY = lymax;
		}
		//CSV�t�@�C���𕶎���̃o�b�t�@�ɓ˂����� //�Z���̍s�͎����Ŏ���
		void LoadFromCSVBuffer(const char* filedata_, const int filesize_, const int cellsize_, const LFTYPE lftype_){
			cellsize = cellsize_; lftype = lftype_;

			//�s�񐔊m�F&�i�[
			//�΂�����
			SearchCSVCellLine(&cellX, &cellY, filedata_, filesize_, lftype_);
			int x = 0, y = 0;
			int strsize = 0;

			if (pppdata != nullptr){
				//���[�N�h�~
				Release();
			}
			//�����̈�m��
			pppdata = new char**[cellY];
			for (int i = 0; i < cellY; i++){
				pppdata[i] = new char*[cellX];
				for (int j = 0; j < cellX; j++){
					pppdata[i][j] = new char[cellsize];
				}
			}


			if (lftype == LFTYPE_CRLF){
				for (int i = 0; i < filesize_;){
					switch (filedata_[i]){
					case '\r':
						if (filedata_[i + 1] == '\n'){
							i += 2;//\r\n��2����������
						}
						else{
							pppdata[y][x][strsize] = filedata_[i];
							i++;
							strsize++;
							break;
						}
						pppdata[y][x][strsize] = '\0';//���s��������k�������Z�b�g
						strsize = 0;
						x++;
						for (int j = x; j < cellX; ++j){
							pppdata[y][j][0] = '\0';	//���s�R�[�h��������E�������ׂăk�������Ŗ��߂�
						}
						y++;
						x = 0;//���̍s���n�߂�
						break;
					case ',':
						pppdata[y][x][strsize] = '\0';//�R���}��������k�������Z�b�g
						strsize = 0;
						i++;
						x++;
						break;
					default:
						pppdata[y][x][strsize] = filedata_[i];
						i++;
						strsize++;
						break;
					}
				}
			}
			else{
				std::cout << "CRLF�ȊO���ݖ��Ή��ł�";
			}
		}
		CSVLoaderClass(){
			cellsize = 0;
			cellX = 0;
			cellY = 0;
			datasize = 0;
			pppdata = nullptr;
			int lftype = LFTYPE_CRLF;
		}
		virtual ~CSVLoaderClass(){
			Release();//���������
		}
		void ShowAllData(){
			for (int y = 0; y < cellY; ++y){
				for (int x = 0; x < cellX; ++x){
					std::cout << pppdata[y][x] << " ";
				}
				std::cout << "\n";
			}
		}
		char* GetCellData(int y_, int x_){
			return pppdata[y_][x_];
		}
		//�w�����Ɍ������������邩����
		//�߂�l:�i�[�ʒu�̍s�ԍ� ���� 0:�����̊i�[����Ă���|�C���^(�K�v�Ȃ����nullptr) 1:�������� 2:���������� 3:��������� 4:�J�n�ʒu(�s�̃X�L�b�v��)
		//����(opdata_�ɏo�͂����l��������̏ꍇ��""(�����R�[�h:\"\")���������1���Ƃ��ďo�͂����B�K�v�ɉ����Ďg�p���ŏ��O���Ă�������.)
		int SearchCellDataInColumn(char **opdata_, const char* searchstring_, const int stringlen_, const int searchColumn_, const int skipLine_){
			int x = searchColumn_;
			for (int y = skipLine_; y < cellY; y++){
				int i = 0;
				if (searchstring_[i] == pppdata[y][x][i]){
					for (i = 1; i < stringlen_; ++i){
						if (searchstring_[i] != pppdata[y][x][i])
							break;
					}
					if (i == stringlen_){
						//����
						if (opdata_ != nullptr)
							*opdata_ = pppdata[y][x];
						return y;
					}
				}
			}
			if (opdata_ != nullptr)
				*opdata_ = nullptr;
			return -1;
		}
		//�w��s���Ɍ������������邩����
		int SearchCellDataInLine(char **opdata_, const char* searchstring_, const int stringlen_, const int searchLine, const int skipColumn_){
			int y = searchLine;
			for (int x = skipColumn_; x < cellX; x++){
				int i = 0;
				if (searchstring_[i] == pppdata[y][x][i]){
					for (i = 1; i < stringlen_; ++i){
						if (searchstring_[i] != pppdata[y][x][i])
							break;
					}
					if (i == stringlen_){
						//����
						if (opdata_ != nullptr)
							*opdata_ = pppdata[y][x];
						return y;
					}
				}
			}
			if (opdata_ != nullptr)
				*opdata_ = nullptr;
			return -1;
		}
		//�w�����Ɍ������������s���邩����
		//�߂�l(���s��)
		//���� 0:�f�[�^�����������s�̃��X�g(�K�v�Ȃ����nullptr)
		int SearchDataNumInColumn(std::vector<int>* opindexlist_, const char* searchstring_, const int searchLine_){
			int strnum = 0;
			const int strlength = strlen(searchstring_);
			int i = 0;
			for (int i = 0; i < cellY; ++i){
				i = SearchCellDataInColumn(nullptr, searchstring_, strlength, searchLine_, i);
				if (i != -1){
					if (opindexlist_ != nullptr){
						opindexlist_->push_back(i);
					}
					strnum++;
				}
				else
					break;	//�����I��
			}
			return strnum;
		}
		char*** GetDataPointer(){ return pppdata; }
		int GetCellXMax(){ return cellX; }
		int GetCellYMax(){ return cellY; }
	};
	inline void CopyStr(char* dest, const char* src){
		unsigned i = 0;
		while (src[i] != '\0'){
			dest[i] = src[i];
			++i;
		}
		dest[i] = src[i];
	}

	//�̈���m�ۂ���src���̕�������i�[�����|�C���^��Ԃ�
	//�߂�l��'\0'�܂Ŋ܂߂�������	//new������̂ŉ����Y��Ȃ��E�E�E
	inline char* NewCopyStr(const char* src_){
		unsigned size = 1;
		while (src_[size] != '\0')
			++size;
		char* dest_ = new char[size];
		for (unsigned i = 0; i < size; ++i)
			dest_[i] = src_[i];
		return dest_;
	}

	//���\�[�X�Ǘ��p�̃e���v���[�g�N���X
	//���f�����ŃA�N�Z�X���A�Q�Ɛ��ɉ����ă��\�[�X�̎擾������s��
	//�����������������x�Ǝg���₷���d���ō���Ă��܂��B
	template<typename T_ManagedClass>
	class TemplateResourceManager{
	public:
		class ManagedData{
		public:
			char* mname;//���\�[�X��
			etd::StringSearcher searcher;
			int refnum;//���\�[�X�̎Q�ƒl(���f���̊i�[����Ă���Q�ƒl�ƎQ�Ƃ̉��Z(+1)���Z(-1)��n��)
			T_ManagedClass* pdata;				//�f�[�^�̊i�[��A�h���X
			//ManagedData(const char* filename_){
			//	SetData(filename_);
			//	modelcomparenum = 0;
			//}
			ManagedData(){
				mname = nullptr;
				searcher.SetData(mname);
				pdata = nullptr;
				refnum = 0;
			}
			virtual ~ManagedData(){
				Release();
			}
			//�����I�Ƀf�[�^���Z�b�g����
			//�f�[�^�������[�X����Ă��邱�Ƃ��O��
			void SetData(const char* filename_){
				mname = etd::NewCopyStr(filename_);
				pdata = new T_ManagedClass(filename_);
				searcher.SetData(mname);
			}
			void Release(){
				Safe_Delete(pdata);
				etd::Safe_DeleteAlloc(mname);
				searcher.SetData(mname);
				refnum = 0;
			}
			//�Q�Ɛ���ύX����ref_ = +1 or -1
			T_ManagedClass* AddReference(const int ref_){
				refnum += ref_;
				return pdata;
			}
			T_ManagedClass* AddReference(){
				++refnum;
				return pdata;
			}
			//�Q�ƒl���Z(0�Ȃ�΃f�[�^�����[�h)
			T_ManagedClass* AddReference(const char* filename_){
				if (!refnum){
					SetData(filename_);
				}
				++refnum;
				return pdata;
			}
			//�Q�ƒl����(0�ɂȂ�Ή��)
			T_ManagedClass* SubReference(){
				--refnum;
				if (!refnum)
					Release();
				return pdata;
			}
		};
		TemplateResourceManager(){}
		TemplateResourceManager(const unsigned capacity_){
			mvector.reserve(capacity_);
		}
		~TemplateResourceManager(){
			RemoveAllFile();
		}

		int modelallcomparenum;	//���f���S�̂̎Q�Ɛ�(0�ɂȂ�ƃ��X�g�����Z�b�g
		std::vector<ManagedData> mvector;
		std::stack<int> removeindex;	//�J�����ꂽ���f���̃C���f�b�N�X��Ԃ�
	private:
		//���f���̊J��
		void ReleaseResource(int index_){
			mvector[index_]->Release();
			removeindex.push(index_);	//�������\�[�X�C���f�b�N�X�ɉ��Z
		}
		void ReleaseResource(const char* filename_){
			int index_ = SearchResource(filename_);
			ReleaseResource(index_);
		}

	public:
		//���f���̎Q�ƒl���炷(�g�p���Ȃ��Ȃ�)
		void SubReference(int index_){
			if (mvector[index_].SubReference() == nullptr){
				//�Q�Ƃ��Ȃ��Ȃ�Ώ����ς݃��X�g�ɒǉ�
				removeindex.push(index_);
			}
		}

		//���\�[�X���m�ۂ���Ă��Ȃ���Ίm�ۂ���
		//���łɊm�ۂ���Ă���ꍇ�͎Q�ƒl�����Z���ăf�[�^���擾����
		//�߂�l�F���\�[�X�̃A�h���X ,out poindex:���f���̊i�[��̃C���f�b�N�X
		T_ManagedClass* GetResource(const char* filename_, int* poindex_){
			int index_ = SearchResource(filename_);
			if (index_ == -1)
				//������Ȃ������ꍇ�v�f�ǉ�
				return GetResourceDontUseSearch(filename_, poindex_);
			else{
				if (poindex_ != nullptr)
					*poindex_ = index_;
				return mvector[index_].AddReference(1);//�Q�Ɖ��Z
			}
		}
		//�����������\�[�X���m��(�����y������{�O������Ăяo���Ȃ��j
		T_ManagedClass* GetResourceDontUseSearch(const char* filename_, int* poindex_){
			if (removeindex.size()){
				//�r���������Ă���ꍇ
				int index_ = removeindex.pop();
				if (poindex_ != nullptr)
					*poindex_ = index_;
				return mvector[index_].AddReference(filename_);//�Q�Ɖ��Z
			}
			else{
				//�󂫂��Ȃ���Ζ�����
				mvector.push_back();
				int index_ = mvector.size() - 1;
				if (poindex_ != nullptr)
					*poindex_ = index_;
				return mvector[index_].AddReference(filename_);//�Q�Ɖ��Z
			}
		}
		//���łɑ��݂��郊�\�[�X���C���f�b�N�X�l����擾����
		T_ManagedClass* GetResource(const int index_){
			return mvector[iondex_].AddReference();
		}
		//���f���̊i�[�ꏊ�̃C���f�b�N�X��Ԃ�
		//�f�[�^���Ȃ����-1��Ԃ�
		unsigned SearchResource(const char* filename_){
			etd::StringSearcher fname(filename_);
			int i = 0;
			for (; i < mvector.size(); ++i){
				if (fname == mvector[i].mname){
					return i;
				}
			}
			return -1;
		}
		//�m�ۂ����t�@�C������ĊJ��
		void RemoveAllFile(){
			for (int i = 0; i < mvector.size(); ++i){
				etd::Safe_Delete(mvector[i]);
			}
			mvector.clear();
		}
	};

	template<typename T_ManagedClass>
	class TemplateManagedResource{
		//		TemplateMangedResource<T_ManagedClass>* manager;
		/*T_ManagedClass* pdata;
		int dataindex;
		TemplateMangedResource(TemplateMangedResource<T_ManagedClass>* manager_){
		manager = manager_;
		pdata = nullptr;
		dataindex = -1;
		}
		virtual ~TemplateManagedResource(){

		}*/
	};
	//�g���̂ėp�̃|�C���^�[�@�����ŉ������
	//�̈�̍Ċm��,�C�ӂ̉���͂ł��Ȃ�(�R���X�g���N�^�Ŏw�萔�̈�m��)
	//�K�x�Ɉ��S�ŁA�ȒP�Ɏg���邱�ƂƉ�������̎�ԏȂ����Ƃ��d�����Ă���
	//�Q�ƂŕԂ��ꂽ�l���|�C���^�ɕϊ����Ė������������悤�Ȏg�����͂��Ȃ��ł�������
	//���ƁA��ptr�g�����z��A�N�Z�X�ɑ΂��ăN���e�B�J���ł͂Ȃ��ł����֐��A�N�Z�X�̃I�[�o�[�w�b�h�������܂�(1000000�v�f�ɂ�����8ms disptr 50ms)
	template<typename T_type>
	class disptr{
		//����ƃf�t�H���g�R���X�g���N�^����
		void operator=(disptr& cant_){}
		disptr(){}
		T_type* p;
	public:
		T_type& operator[](unsigned index_){ return p[index_]; }
		disptr(const unsigned  datanum_){
			p = new T_type[datanum_];
		}
		T_type* GetAddr(){ return p; }
		~disptr(){
			etd::Safe_DeleteAlloc(p);
		}
	};

	//�g���̂ă|�C���^�[�Q
	//�N���X�i�C�ɑ��݂ł���悤�f�t�H���g�R���X�g���N�^���g�p��
	//���������[�N�ɂ��o�O�̌��m�����₷���̂ŁA�̈�m�ۂ̃`�F�b�N�͍s��Ȃ�
	template<typename T_type>
	class classptr{
		void operator=(classptr& cant_){}
		T_type* p;
		int datanum;
	public:
		classptr(){
			p = nullptr;
			datanum = 0;
		}
		classptr(const unsigned  datanum_){
			p = nullptr;
			getmemory(datanum_);
		}
#define _NEED_CHEKDEBUGFLAG
#ifdef _NEED_CHEKDEBUGFLAG
		std::vector<T_type*>p_ptr;	//�f�o�b�O�p
#endif
		T_type* getmemory(const unsigned  datanum_){
			if (p == nullptr){
				if (datanum_ <= 0)
					return nullptr;
				p = new T_type[datanum_];
				datanum = datanum_;

#ifdef _NEED_CHEKDEBUGFLAG
				//�f�o�b�O�p---
				for (unsigned i = 0; i < datanum_; ++i)
					p_ptr.push_back(&p[i]);
#endif
			}
			else
				MessageBoxA(nullptr, "etd::classptr::getmemory()", "�������͎擾�ς݂ł��B", MB_OK);
			return p;
		}
		void ZeroClear(){
			memset(p, 0, sizeof(T_type)*datanum);
		}
		T_type& operator[](unsigned index_){ return p[index_]; }
		void Release(){
			etd::Safe_DeleteAlloc(p);
		}

		T_type* &GetArray(){ return p; }
		~classptr(){
			etd::Safe_DeleteAlloc(p);
		}
		int GetDataNum(){ return datanum; }

	};
	///<sumary>TemplateClass �X�}�[�g�|�C���^</sumary>
	//<remarks>STL�ł���shared_ptr�ɋ߂�</remarks>
	template<typename T>
	class smptr{
	private:
		T* _ptr = nullptr;
		unsigned *_counter = nullptr;

		///<sumary>Func �Q�ƃJ�E���^�[����</sumary>
		//<remarks></remarks>
		void MinusCounter(){
			if (_counter != nullptr){
				(*_counter)--;
				//�Q�Ɛ���0�ɂȂ�����f���[�g
				if (*_counter == 0){
					Delete();
				}
				//�J�E���^�[��ؿ���̎Q�Ə���
				_counter = nullptr;
				_ptr = nullptr;
			}
		}
		///<sumary>�Q�ƃJ�E���^�[����</sumary>
		//<remarks></remarks>
		void AddCounter(smptr& smptr_){
			_counter = smptr_._counter;
			_ptr = smptr_._ptr;
			(*_counter)++;
		}
		//ؿ�����
		void Delete(){
			delete _counter;
			_counter = nullptr;
			delete _ptr;
			_ptr = nullptr;
		}
	public:
		///<sumary>operator �X�}�[�g�|�C���^�[�̃R�s�[</sumary>
		//<remarks>�Q�ƃJ�E���^+1</remarks>
		smptr& operator=(smptr& copy_){
			MinusCounter();	//���ɂȂ񂩓����Ă����p
			AddCounter(copy_);//�J�E���^�[����
			return *this;
		}
		smptr(){}
		///<sumary>�R���X�g���N�^</sumary>
		//<remarks>new �ō쐬�����A�h���X���g�p���Ă��������B���̂Ŏg�p���̃A�h���X���i�[�����ꍇ�A�f�X�g���N�^�Ŕj������܂��B</remarks>
		smptr(T* newptr_) : _counter(new unsigned(1)), _ptr(newptr_)
		{		}
		///<sumary>�R�s�[�R���X�g���N�^</sumary>
		//<remarks>�Q�ƃJ�E���g��+1���ăA�h���X�ێ�</remarks>
		smptr(smptr& copy_) :_counter(copy_._counter), _ptr(copy_._ptr)
		{
			++(*_counter);
		}
		///<sumary> ���[�u�R���X�g���N�^</sumary>
		//<remarks>�E��smptr�̒��g����ۂɂȂ�܂��B�Q�ƃJ�E���^�͕ϓ��Ȃ�</remarks>
		smptr(smptr&& move) : _ptr(move._ptr), _counter(move._counter) {
			move._ptr = nullptr;   // �ǂ����w���Ȃ�
			move._counter = nullptr;
		}
		///<sumary>operator *</sumary>
		//<remarks>�A�h���X������Ԏ擾</remarks>
		inline T& operator *(){
			return *_ptr;
		}
		inline T* operator ->() const{
			return _ptr;
		}
		~smptr(){ MinusCounter(); }

		///<sumary>�V�Ksmptr�쐬</sumary>
		//<remarks>new �ō쐬�����A�h���X���g�p���Ă��������B���̂Ŏg�p���̃A�h���X���i�[�����ꍇ�A�f�X�g���N�^�Ŕj������܂��B</remarks>
		smptr& New(T* newptr_){
			MinusCounter();
			_counter = new unsigned(1);
			_ptr = newptr_;
			return *this;
		}
	};

	void RemoveExtension(char* dst_, const char* src_){
		for (int i = 0; i < 256; ++i){
			if (src_[i] == '.'){
				dst_[i] = '\0';
				break;
			}
			dst_[i] = src_[i];
		}
	}


	//�t���p�X����t�H���_�p�X�ƃt�@�C�����𒊏o����֐�(1023�����܂�)
	//�o�͂���t�H���_�p�X�ɂ͖�����/�ƃk�����܂܂��
	unsigned GetFilePath(char* dest_folderpath_, char* dest_filename_, const char* src_fullpath_){
		unsigned latestslash = 0;	//�Ō��/���������ꏊ
		int find = 0;

		int i_1 = 0;
		for (; i_1 < 1024; ++i_1){
			if (src_fullpath_[i_1] == '\0')
				break;
			if (src_fullpath_[i_1] == '/'){
				latestslash = i_1;
				++find;
			}
		}
		if (!find){	//������Ȃ������ꍇ�A�t�H���_�p�X""�t�@�C�������̂܂܂ŏo�͂��ďI��
			dest_folderpath_[0] = '\0';	//�t�H���_�p�X��null�i�[
			//�t�@�C�����ɂ͓��͒l�����̂܂܏o��
			int i = 0;
			for (; src_fullpath_[i] != '\0'; ++i){
				dest_filename_[i] = src_fullpath_[i];
			}dest_filename_[i] = src_fullpath_[i];
			return 0;
		}
		//���������ꍇ
		unsigned j = 0;
		//�t�H���_��	/+\0�܂ŏo��
		for (; j < latestslash + 1; ++j)
			dest_folderpath_[j] = src_fullpath_[j];
		dest_folderpath_[j] = '\0';
		//�t�@�C����	�o��
		int i = 0;
		for (; src_fullpath_[j] != '\0'; ++i, ++j)
			dest_filename_[i] = src_fullpath_[j];
		dest_filename_[i] = '\0';
		return latestslash + 1;
	}

	//������A��//�Z�[�t��
	void StrCompare(char* dest_, const char* src1_, const char* src2_){
		int s = 0;
		for (int i = 0; src1_[i] != '\0'; ++i, ++s)
			dest_[s] = src1_[i];
		for (int i = 0; src2_[i] != '\0'; ++i, ++s)
			dest_[s] = src2_[i];
		dest_[s] = '\0';
	}

	


	//��������@�\�t���A�h���Xvector
	//push_back�Ŋi�[�̍�new �`���Ŋi�[���A//�C���X�^���X�̉�����Ɏ����I�ɉ������
	template<typename T_type>
	class ptr_vector{
	public:
		std::vector<T_type*>ptr;
		~ptr_vector(){
			deleteall();
		}
		ptr_vector(){
		}
		T_type& push_back(T_type* new_type_){
			ptr.push_back(new_type_);
			return *ptr.back();
		}
		T_type& back(){
			return *ptr.back();
		}
		T_type& operator[](int refnumber_){
			return *ptr[refnumber_];
		}
		unsigned size(){
			return ptr.size();
		}
		void deleteall(){
			for (auto idx : ptr){
				if (idx != nullptr)
					delete idx;
				idx = nullptr;
			}
			ptr.clear();
		}
		void pop_back(){
			delete ptr.back();
			ptr.back() = nullptr;
			ptr.pop_back();
		}

	};
	////����N���X�͈̔�for���Ή��p�C�e���[�^//�ԐڎQ�Ɖ��Z�q�ƑO�u�C���N�������g���Z�q�ƕs�������Z�q���`����
	//template<typename T_type>


	//�����̕�����ϊ�
	//�����@1:�o�͐�A�h���X�A���l�i���R���̂�
	void ComvertIntegerToString(char* pout_, int input_){
		if (input_ <= 0){	//���l0�ȉ��̏ꍇ��"0"�ɐݒ肵�ďI��
			pout_[0] = '0';
			pout_[1] = '\0';
			return;
		}
		char buf[256];
		int DigitNum = 0;
		while (input_ >= 1){
			int numnum = input_ % 10;
			buf[DigitNum] = IntegerToChar0to9[numnum];
			input_ /= 10;
			++DigitNum;
		}

		for (int i = 0; i < DigitNum; ++i)
			pout_[i] = buf[DigitNum - 1 - i];

		pout_[DigitNum] = '\0';
	}
}