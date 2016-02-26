#pragma once

//タイマークラス(ms)
namespace etd{
	class TimerClass{
	protected:
		unsigned long starttime;	//タイマーの開始時間
		unsigned long endtime;		//終了時間(StartTimerとともに初期化される)
		unsigned long passedtime;	//経過した時間(EndTimerが呼ばれるとともに更新される)
	public:
		TimerClass(){ starttime = endtime = GetNowTime(); passedtime = 0; }
		virtual ~TimerClass(){}
		//現在の時間を取得
		static unsigned long GetNowTime(){
			timeBeginPeriod(1);	//タイマーの精度を1msに
			return timeGetTime();
		}

		//SetAutoTimerとSetStart & Endタイマーは同一インスタンスでは併用できません.
		//1つセットするだけで自動的に更新するタイマー
		unsigned long SetAutoTimer(){ endtime = GetNowTime();	passedtime = endtime - starttime; starttime = endtime; return passedtime; }

		//好きなタイミングでセットして好きなタイミングで停止できるタイマー(2つで1セット)
		//タイマー開始
		void SetStartTimer(){ starttime = endtime = GetNowTime(); }
		//タイマー終了（戻り値：経過時間(ms)
		unsigned long SetEndTimer(){ endtime = GetNowTime(); passedtime = endtime - starttime; return passedtime; }

		//ゲッター類
		inline unsigned long GetPassedTime(){ return passedtime; }
		inline unsigned long GetStartTime(){ return starttime; }
		inline unsigned long GetEndTime(){ return endtime; }
	};
	class FPSTimerClass{
		
	protected:
		float fps;
		unsigned long framecounter;
		unsigned long lateststarttime;
	public:
		enum FPSTIMERTYPE_{//FPSタイマーの種類
			FPSTIMERTYPE_1FRAME,	//1フレームごとのFPS取得
			FPSTIMERTYPE_SETFRAMECOUNTER,//指定したフレーム経過時更新
			FPSTIMERTYPE_SETPASSEDTIMER,//指定時間(ms)経過時更新
		};
		FPSTimerClass(){ fps = 0; framecounter = lateststarttime = 0; }
		virtual ~FPSTimerClass(){}
		//現在の時間を取得
		static unsigned long GetNowTime(){ return TimerClass::GetNowTime(); }
		//FPSタイマーセット(type,使用するtypeごとにパラメータを指定)
		//TYPE_1FRAME			param:意味なし(なんでも可)	一番値が荒れます
		//TYPE_SETFRAMECOUNTER	param:フレーム数(何フレーム以上経過でfps更新するか)
		//TYPE_SETTIMER			param:指定時間(ms)(なんms以上経過でfps更新するか)
		float SetFPSTimer(const FPSTIMERTYPE_ fpstype, const unsigned long param){

			switch (fpstype){
			case FPSTIMERTYPE_1FRAME://1フレームごとにFPS更新
			{
										 unsigned long nowtime = timeGetTime();
										 unsigned long passedtime = nowtime - lateststarttime;
										 if (passedtime){
											 fps = 1.0f / (float)passedtime * 1000;
											 lateststarttime = nowtime;
										 }
										 else
											 fps = 0;
			}
				break;
			case FPSTIMERTYPE_SETFRAMECOUNTER:	//一定フレーム経過ごとにFPS更新
			{
													framecounter++;
													if (framecounter >= param){
														unsigned long nowtime = timeGetTime();
														unsigned long passedtime = nowtime - lateststarttime;
														if (passedtime){//念のため
															fps = (float)framecounter / (float)passedtime * 1000;
															lateststarttime = nowtime;
															framecounter = 0;
														}
														else
															fps = 0; framecounter = 0;
													}
			}
				break;
			case FPSTIMERTYPE_SETPASSEDTIMER:	//一定時間経過ごとにfps更新
			{
													unsigned long nowtime = timeGetTime();
													unsigned long passedtime = nowtime - lateststarttime;
													framecounter++;
													if (passedtime >= param){
														if (passedtime){//念のため
															lateststarttime = nowtime;
															framecounter = 0;
														}
														else
															fps = 0; framecounter = 0;
													}

			}
				break;
			}//switch
			return fps;
		}
		float GetFps(){ return fps; }
	};
}