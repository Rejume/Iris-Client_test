//拡張テンプレートライブラリ
//制作：根占
//使い方：etd::をつけていろいろと、
//内容：便利そうな機能を思いついたら書いていくヘッダー
//c++やstlなどの機能を用いた便利機能を書き、DirectXなどハードに影響されるものは書かない
#pragma once
//ExtendTemplateLibrary
//STLの機能拡張用のテンプレート群
//プリプロ(stdafx)に追加
//作成　根占　2015_0531

namespace etd{
	//for_each簡略化
	//第一引数コンテナ(vectorなど)第二引数にはmem_fun_ref(関数名)で求めた値を指定
	//引数設定したいときとかはbind2ndを使用
	//使用例(CManiac) for_each(vecmaniac, bind2nd(mem_fun_ref(&CManiac::SetiHowManiac), 2));
	template < typename T_container, typename T_function>
	T_function for_each(T_container& rcontainer, T_function mem_fun_ref_func){
		return std::for_each(rcontainer.begin(), rcontainer.end(), mem_fun_ref_func);
	}
	//メモリ解放(nullptrのdeleteはc++では有効なので比較しない）
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

	//軽量版abs(ただの比較)
	template < typename T_type>
	T_type absif(T_type type){
		if (type < 0)
			type = -type;
	}

	//10のn乗の値を取得する(10乗以上はintの格納外なので死ぬ)
	//速度重視でセーフティ掛けてないので使用側で設定してください
	static const int Pow10i[] = {
		1, 10, 100, 1000, 10000, 100000, 1000000,
		10000000, 100000000, 1000000000, //9乗
	};
	//数値を文字変化(0~9)
	static const char IntegerToChar0to9[] = {
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
	};

	//割と高速バイナリ化テキストファイルローダークラス
	class BinaryTextLoader{
	protected:
		char* pfiledata;
		unsigned filesize;
		char* LoadFileBinary(const char* filename){
			//ファイルをとりあえずバッファに入れる
			std::ifstream ifs(filename, std::ios::binary);
			if (ifs.fail()){//ファイルオープンに失敗したら閉じる
				ifs.close();
				std::cout << filename << "ファイルオープンに失敗しました。" << std::endl;
				MessageBoxA(nullptr, filename, "etl::BinaryTextLoader\nファイルオープンに失敗しました", MB_OK);
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
		BinaryTextLoader(){}	//使用しない
	public:
		BinaryTextLoader(const char* filename){
			filesize = 0;
			LoadFileBinary(filename);
		}
		const char* GetBinaryData(){ return pfiledata; }
		virtual ~BinaryTextLoader(){
			Safe_DeleteAlloc(pfiledata);
		}
		//バイナリデータからdest_に対してcopysize_バイトコピー
		//開始位置はseeker_で指示(ファイルの開始地点であるならば0指定)
		//戻り値はseeker_が進んだ先のseek地点
		int CopyBinaryData(void* dest_, int copysize_, int seeker_){
			memcpy(dest_, pfiledata + seeker_, copysize_);
			return seeker_ + copysize_;
		}
		//バイナリデータから文字列(null文字を含む）を取得する
		//開始位置はseeker_で指示(ファイルの開始地点であるならば0指定)
		//strmax は文字列上限を指定
		//戻り値はseeker_が進んだ先のseek地点
		int CopyString(char* dest_, int seeker_, const int strmax_){
			int i = 0;
			for (; i < strmax_; ++i){
				char c = *(pfiledata + seeker_ + i);
				dest_[i] = c;
				if (c == '\0'){
					//探査終了
					return seeker_ + i + 1;
				}

			}
			//失敗
			MessageBoxA(nullptr, "ExtendTemlateLibrary.hpp", "BinaryTextLoader->CopyStringにて\n文字列読み込みに失敗しました。", MB_OK);
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
		//UTF-16対応版//失敗作
		int CopyStringUTF16(char* dest_, int seeker_, const int strmax_){
			int i = 0;
			for (; i < strmax_; ++i){
				char c = *(pfiledata + seeker_ + i);
				char c2 = *(pfiledata + seeker_ + i + 1);
				char c3 = *(pfiledata + seeker_ + i + 2);
				dest_[i] = c;
				if (c == '\0' && c2 == '\0'&& c3 == '\0'){
					//探査終了
					dest_[i + 1] = '\0';
					dest_[i + 2] = '\0';
					return seeker_ + i + 2;
				}

			}
			//失敗
			MessageBoxA(nullptr, "ExtendTemlateLibrary.hpp", "BinaryTextLoader->CopyStringにて\n文字列読み込みに失敗しました。", MB_OK);
			return -1;

		}
		unsigned GetFileSize(){ return filesize; }
		//デストラクタで自動解放されるので呼ばなくても良い
		void Release(){
			Safe_DeleteAlloc(pfiledata);
		}
	};
	//文字列検索クラス
	class StringSearcher{
		const char* pfiledata;
		unsigned filesize;
	public:
		//デフォルトコンストラクタなるだけ呼ばない
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
		//ファイルサイズ指定してアドレスを格納(\0)を含める
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
						//合致していれば離脱
						return i;
					}
				}
			}
			return -1;
		}
		//文字列の末尾が格納されている配列のインデックスを返す
		int SearchStringE(const char* str_, const unsigned startpos_){
			int strlength = strlen(str_);
			int index = -1;
			index = SearchStringS(str_, startpos_);
			if (index != -1){
				index += (strlength - 1);
			}
			return index;
		}

		//文字列が同じかどうか判定(完全一致の場合のみtrue返す)
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

		//文字列から整数データ取得(1つの数値は9文字以下で)
		//(指定したバッファ位置から数字データを検索して、数値に変換して格納する）
		//成功時数値の格納されていた配列の末尾のインデックスを返し、
		//			opnumに値を出力
		//失敗時 -1を返す
		int SearchIntergerData(int* opnum, const unsigned startpos_){
			const int BUFMAX = 10;//数値数が10を超える場合int型に収まりきらないので失敗する
			char buf[BUFMAX];
			int numlen = 0;
			int sign = 1;//符号
			for (unsigned i = startpos_; i < filesize; i++){
				if (pfiledata[i] == '-')
					sign = -1;
				else if (pfiledata[i] >= '0' && pfiledata[i] <= '9'){
					numlen = 1;
					buf[0] = pfiledata[i];
					int j = 0;
					//数値をバッファに格納
					for (j = 0; j < BUFMAX; j++){
						if (pfiledata[i + numlen] >= '0' && pfiledata[i + numlen] <= '9'){
							buf[numlen] = pfiledata[i + numlen];
							numlen++;
						}
						else{//数値終了
							buf[numlen] = '\0';
							break;
						}
					}
					if (j == BUFMAX)	//やったら長い数値データばっか並んでた場合失敗
						return -1;
					//文字列を数値に変換
					int ans = 0;//数値格納先
					for (int x = 0; x < numlen; ++x){
						ans += ((int)(buf[x] - '0')*Pow10i[numlen - 1 - x]);
					}
					ans *= sign;
					if (opnum != nullptr)
						*opnum = ans;	//数値格納
					return i + numlen - 1;	//末尾アドレス返す
				}
			}
			return -1;	//失敗
		}
		//文字列から小数データ取得(1つの数値は256文字以下で・・・)
		//(指定したバッファ位置から数字データを検索して、数値に変換して格納する）
		//注意: .3241　みたいな整数部のデータが抜けてる場合正常に取得できません
		//成功時数値の格納されていた配列の末尾のインデックスを返し、
		//			opnumに値を出力
		//失敗時 -1を返す
		int SearchFDecimalData(float* opnum, const unsigned startpos_){
			const int BUFMAX = 256;//数値数が10を超える場合int型に収まりきらないので失敗する
			char buf[2][BUFMAX];
			int numlen = 0;
			int numpart[2] = { 0, 0 };//[0]整数部の数値長 [1]小数部の数値長
			float sign = 1.f;
			for (unsigned i = startpos_; i < filesize; i++){
				if (pfiledata[i] == '-'){
					sign = -1.f;
				}
				else if (pfiledata[i] >= '0' && pfiledata[i] <= '9'){
					int dotfind = 0;
					buf[dotfind][0] = pfiledata[i];
					int j = 0;
					//数値をバッファに格納
					for (j = 0; j < BUFMAX; j++){
						if (pfiledata[i + j] >= '0' && pfiledata[i + j] <= '9'){
							//数値発見
							buf[dotfind][numpart[dotfind]] = pfiledata[i + j];
							numpart[dotfind]++;
						}
						else if (pfiledata[i + j] == '.'){
							//どっと(.)発見
							buf[dotfind][numpart[dotfind]] = '\0';
							//numlen++;
							dotfind = 1;
						}
						else{//数値終了
							buf[dotfind][numpart[dotfind]] = '\0';
							break;
						}
					}
					if (j == BUFMAX)	//やったら長い数値データばっか並んでた場合失敗
						return -1;
					//文字列を数値に変換
					float ans = 0;//数値格納先
					//整数部格納
					for (int x = 0; x < numpart[0]; ++x){
						ans += (float)((float)(buf[0][x] - '0'))*powf(10.f, (numpart[0] - x - 1.f));
					}//少数部格納
					for (int x = 0; x < numpart[1]; ++x){
						ans += ((float)(buf[1][x] - '0'))*powf(10.f, -x - 1.f);
					}
					ans *= sign;
					if (opnum != nullptr)
						*opnum = ans;	//ポインターがnullpoじゃなければ数値格納
					return i + j;	//末尾アドレス返す
				}
			}
			return -1;	//失敗
		}

	};

	//改行コード識別クラス
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
	//CSVにアクセスするためのクラス一時的に大量のメモリ消費します。
	class CSVLoaderClass{
	protected:
		int cellsize;	//1つのセルのサイズ(たいていは128とか256とかで・・)
		int cellX;		//セルの列数
		int cellY;		//セルの行数
		char*** pppdata;	//データ要素は[y][x][cellsize]
		int lftype;
		int datasize;

		//char型のバッファからファイルデータを作成する
	public:
		//デストラクタ呼び出し
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

		//CSVファイルの横幅縦幅を検出
		void SearchCSVCellLine(int* polinemaxX, int* polinemaxY, const char* filedata_, const int filesize_, const LFTYPE lftype){
			LFCheckerObj lfcheck(lftype);	//改行コード識別
			int lymax = 0, lxmax = 0;
			int ly = 0, lx = 0;
			for (int i = 0; i < filesize_; ++i){
				switch (filedata_[i]){
				case LFTYPE_CR:
				case LFTYPE_LF:
					//改行発生
					if (lfcheck.CheckLF(filedata_, i)){
						++ly;
						++lx;
						if (lftype == LFTYPE_CRLF)
							i++;
						if (lxmax < lx)
							lxmax = lx;	//最大幅を検出
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
		//CSVファイルを文字列のバッファに突っ込む //セルの行は自動で識別
		void LoadFromCSVBuffer(const char* filedata_, const int filesize_, const int cellsize_, const LFTYPE lftype_){
			cellsize = cellsize_; lftype = lftype_;

			//行列数確認&格納
			//ばぐあり
			SearchCSVCellLine(&cellX, &cellY, filedata_, filesize_, lftype_);
			int x = 0, y = 0;
			int strsize = 0;

			if (pppdata != nullptr){
				//リーク防止
				Release();
			}
			//初期領域確保
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
							i += 2;//\r\nの2文字分送る
						}
						else{
							pppdata[y][x][strsize] = filedata_[i];
							i++;
							strsize++;
							break;
						}
						pppdata[y][x][strsize] = '\0';//改行が来たらヌル文字セット
						strsize = 0;
						x++;
						for (int j = x; j < cellX; ++j){
							pppdata[y][j][0] = '\0';	//改行コードが来たら右側をすべてヌル文字で埋める
						}
						y++;
						x = 0;//次の行を始める
						break;
					case ',':
						pppdata[y][x][strsize] = '\0';//コンマが来たらヌル文字セット
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
				std::cout << "CRLF以外現在未対応です";
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
			Release();//メモリ解放
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
		//指定列内に検索文字があるか検索
		//戻り値:格納位置の行番号 引数 0:文字の格納されているポインタ(必要なければnullptr) 1:検索文字 2:検索文字長 3:検索する列 4:開始位置(行のスキップ数)
		//注意(opdata_に出力される値が文字列の場合は""(文字コード:\"\")も文字列の1部として出力される。必要に応じて使用側で除外してください.)
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
						//発見
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
		//指定行内に検索文字があるか検索
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
						//発見
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
		//指定列内に検索文字が何行あるか検索
		//戻り値(何行か)
		//引数 0:データが見つかった行のリスト(必要なければnullptr)
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
					break;	//検索終了
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

	//領域を確保してsrc内の文字列を格納したポインタを返す
	//戻り値は'\0'まで含めた文字列長	//newをするので解放を忘れない・・・
	inline char* NewCopyStr(const char* src_){
		unsigned size = 1;
		while (src_[size] != '\0')
			++size;
		char* dest_ = new char[size];
		for (unsigned i = 0; i < size; ++i)
			dest_[i] = src_[i];
		return dest_;
	}

	//リソース管理用のテンプレートクラス
	//モデル名でアクセスし、参照数に応じてリソースの取得解放を行う
	//メモリ効率よりも速度と使いやすさ重視で作っています。
	template<typename T_ManagedClass>
	class TemplateResourceManager{
	public:
		class ManagedData{
		public:
			char* mname;//リソース名
			etd::StringSearcher searcher;
			int refnum;//リソースの参照値(モデルの格納されている参照値と参照の加算(+1)減算(-1)を渡す)
			T_ManagedClass* pdata;				//データの格納先アドレス
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
			//強制的にデータをセットする
			//データがリリースされていることが前提
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
			//参照数を変更するref_ = +1 or -1
			T_ManagedClass* AddReference(const int ref_){
				refnum += ref_;
				return pdata;
			}
			T_ManagedClass* AddReference(){
				++refnum;
				return pdata;
			}
			//参照値加算(0ならばデータをロード)
			T_ManagedClass* AddReference(const char* filename_){
				if (!refnum){
					SetData(filename_);
				}
				++refnum;
				return pdata;
			}
			//参照値減少(0になれば解放)
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

		int modelallcomparenum;	//モデル全体の参照数(0になるとリストもリセット
		std::vector<ManagedData> mvector;
		std::stack<int> removeindex;	//開放されたモデルのインデックスを返す
	private:
		//モデルの開放
		void ReleaseResource(int index_){
			mvector[index_]->Release();
			removeindex.push(index_);	//除去リソースインデックスに加算
		}
		void ReleaseResource(const char* filename_){
			int index_ = SearchResource(filename_);
			ReleaseResource(index_);
		}

	public:
		//モデルの参照値減らす(使用しなくなる)
		void SubReference(int index_){
			if (mvector[index_].SubReference() == nullptr){
				//参照がなくなれば除去済みリストに追加
				removeindex.push(index_);
			}
		}

		//リソースが確保されていなければ確保して
		//すでに確保されている場合は参照値を加算してデータを取得する
		//戻り値：リソースのアドレス ,out poindex:モデルの格納先のインデックス
		T_ManagedClass* GetResource(const char* filename_, int* poindex_){
			int index_ = SearchResource(filename_);
			if (index_ == -1)
				//見つからなかった場合要素追加
				return GetResourceDontUseSearch(filename_, poindex_);
			else{
				if (poindex_ != nullptr)
					*poindex_ = index_;
				return mvector[index_].AddReference(1);//参照加算
			}
		}
		//検索せずリソースを確保(少し軽いが基本外部から呼び出さない）
		T_ManagedClass* GetResourceDontUseSearch(const char* filename_, int* poindex_){
			if (removeindex.size()){
				//途中があいている場合
				int index_ = removeindex.pop();
				if (poindex_ != nullptr)
					*poindex_ = index_;
				return mvector[index_].AddReference(filename_);//参照加算
			}
			else{
				//空きがなければ末尾に
				mvector.push_back();
				int index_ = mvector.size() - 1;
				if (poindex_ != nullptr)
					*poindex_ = index_;
				return mvector[index_].AddReference(filename_);//参照加算
			}
		}
		//すでに存在するリソースをインデックス値から取得する
		T_ManagedClass* GetResource(const int index_){
			return mvector[iondex_].AddReference();
		}
		//モデルの格納場所のインデックスを返す
		//データがなければ-1を返す
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
		//確保したファイルを一斉開放
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
	//使い捨て用のポインター　自動で解放する
	//領域の再確保,任意の解放はできない(コンストラクタで指定数領域確保)
	//適度に安全で、簡単に使えることと解放処理の手間省くことを重視している
	//参照で返された値をポインタに変換して無理やり解放するような使い方はしないでください
	//あと、生ptr使うより配列アクセスに対してクリティカルではないですが関数アクセスのオーバーヘッドが生じます(1000000要素につき生ぽ8ms disptr 50ms)
	template<typename T_type>
	class disptr{
		//代入とデフォルトコンストラクタ拒否
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

	//使い捨てポインター２
	//クラスナイに存在できるようデフォルトコンストラクタが使用可に
	//メモリリークによるバグの検知がしやすいので、領域確保のチェックは行わない
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
		std::vector<T_type*>p_ptr;	//デバッグ用
#endif
		T_type* getmemory(const unsigned  datanum_){
			if (p == nullptr){
				if (datanum_ <= 0)
					return nullptr;
				p = new T_type[datanum_];
				datanum = datanum_;

#ifdef _NEED_CHEKDEBUGFLAG
				//デバッグ用---
				for (unsigned i = 0; i < datanum_; ++i)
					p_ptr.push_back(&p[i]);
#endif
			}
			else
				MessageBoxA(nullptr, "etd::classptr::getmemory()", "メモリは取得済みです。", MB_OK);
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
	///<sumary>TemplateClass スマートポインタ</sumary>
	//<remarks>STLでいうshared_ptrに近い</remarks>
	template<typename T>
	class smptr{
	private:
		T* _ptr = nullptr;
		unsigned *_counter = nullptr;

		///<sumary>Func 参照カウンター減少</sumary>
		//<remarks></remarks>
		void MinusCounter(){
			if (_counter != nullptr){
				(*_counter)--;
				//参照数が0になったらデリート
				if (*_counter == 0){
					Delete();
				}
				//カウンターとﾘｿｰｽの参照消す
				_counter = nullptr;
				_ptr = nullptr;
			}
		}
		///<sumary>参照カウンター増加</sumary>
		//<remarks></remarks>
		void AddCounter(smptr& smptr_){
			_counter = smptr_._counter;
			_ptr = smptr_._ptr;
			(*_counter)++;
		}
		//ﾘｿｰｽ解放
		void Delete(){
			delete _counter;
			_counter = nullptr;
			delete _ptr;
			_ptr = nullptr;
		}
	public:
		///<sumary>operator スマートポインターのコピー</sumary>
		//<remarks>参照カウンタ+1</remarks>
		smptr& operator=(smptr& copy_){
			MinusCounter();	//中になんか入ってた時用
			AddCounter(copy_);//カウンター増加
			return *this;
		}
		smptr(){}
		///<sumary>コンストラクタ</sumary>
		//<remarks>new で作成したアドレスを使用してください。他ので使用中のアドレスを格納した場合、デストラクタで破棄されます。</remarks>
		smptr(T* newptr_) : _counter(new unsigned(1)), _ptr(newptr_)
		{		}
		///<sumary>コピーコンストラクタ</sumary>
		//<remarks>参照カウントを+1してアドレス保持</remarks>
		smptr(smptr& copy_) :_counter(copy_._counter), _ptr(copy_._ptr)
		{
			++(*_counter);
		}
		///<sumary> ムーブコンストラクタ</sumary>
		//<remarks>右側smptrの中身空っぽになります。参照カウンタは変動なし</remarks>
		smptr(smptr&& move) : _ptr(move._ptr), _counter(move._counter) {
			move._ptr = nullptr;   // どこも指さない
			move._counter = nullptr;
		}
		///<sumary>operator *</sumary>
		//<remarks>アドレスから実態取得</remarks>
		inline T& operator *(){
			return *_ptr;
		}
		inline T* operator ->() const{
			return _ptr;
		}
		~smptr(){ MinusCounter(); }

		///<sumary>新規smptr作成</sumary>
		//<remarks>new で作成したアドレスを使用してください。他ので使用中のアドレスを格納した場合、デストラクタで破棄されます。</remarks>
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


	//フルパスからフォルダパスとファイル名を抽出する関数(1023文字まで)
	//出力するフォルダパスには末尾の/とヌルが含まれる
	unsigned GetFilePath(char* dest_folderpath_, char* dest_filename_, const char* src_fullpath_){
		unsigned latestslash = 0;	//最後に/があった場所
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
		if (!find){	//見つからなかった場合、フォルダパス""ファイル名そのままで出力して終了
			dest_folderpath_[0] = '\0';	//フォルダパスにnull格納
			//ファイル名には入力値をそのまま出力
			int i = 0;
			for (; src_fullpath_[i] != '\0'; ++i){
				dest_filename_[i] = src_fullpath_[i];
			}dest_filename_[i] = src_fullpath_[i];
			return 0;
		}
		//見つかった場合
		unsigned j = 0;
		//フォルダ名	/+\0まで出力
		for (; j < latestslash + 1; ++j)
			dest_folderpath_[j] = src_fullpath_[j];
		dest_folderpath_[j] = '\0';
		//ファイル名	出力
		int i = 0;
		for (; src_fullpath_[j] != '\0'; ++i, ++j)
			dest_filename_[i] = src_fullpath_[j];
		dest_filename_[i] = '\0';
		return latestslash + 1;
	}

	//文字列連結//セーフ無
	void StrCompare(char* dest_, const char* src1_, const char* src2_){
		int s = 0;
		for (int i = 0; src1_[i] != '\0'; ++i, ++s)
			dest_[s] = src1_[i];
		for (int i = 0; src2_[i] != '\0'; ++i, ++s)
			dest_[s] = src2_[i];
		dest_[s] = '\0';
	}

	


	//自動解放機能付きアドレスvector
	//push_backで格納の際new 形名で格納し、//インスタンスの解放時に自動的に解放する
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
	////自作クラスの範囲for文対応用イテレータ//間接参照演算子と前置インクリメント演算子と不等価演算子を定義する
	//template<typename T_type>


	//整数の文字列変換
	//引数　1:出力先アドレス、数値（自然数のみ
	void ComvertIntegerToString(char* pout_, int input_){
		if (input_ <= 0){	//数値0以下の場合は"0"に設定して終了
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