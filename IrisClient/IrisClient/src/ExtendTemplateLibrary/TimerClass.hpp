#pragma once

//�^�C�}�[�N���X(ms)
namespace etd{
	class TimerClass{
	protected:
		unsigned long starttime;	//�^�C�}�[�̊J�n����
		unsigned long endtime;		//�I������(StartTimer�ƂƂ��ɏ����������)
		unsigned long passedtime;	//�o�߂�������(EndTimer���Ă΂��ƂƂ��ɍX�V�����)
	public:
		TimerClass(){ starttime = endtime = GetNowTime(); passedtime = 0; }
		virtual ~TimerClass(){}
		//���݂̎��Ԃ��擾
		static unsigned long GetNowTime(){
			timeBeginPeriod(1);	//�^�C�}�[�̐��x��1ms��
			return timeGetTime();
		}

		//SetAutoTimer��SetStart & End�^�C�}�[�͓���C���X�^���X�ł͕��p�ł��܂���.
		//1�Z�b�g���邾���Ŏ����I�ɍX�V����^�C�}�[
		unsigned long SetAutoTimer(){ endtime = GetNowTime();	passedtime = endtime - starttime; starttime = endtime; return passedtime; }

		//�D���ȃ^�C�~���O�ŃZ�b�g���čD���ȃ^�C�~���O�Œ�~�ł���^�C�}�[(2��1�Z�b�g)
		//�^�C�}�[�J�n
		void SetStartTimer(){ starttime = endtime = GetNowTime(); }
		//�^�C�}�[�I���i�߂�l�F�o�ߎ���(ms)
		unsigned long SetEndTimer(){ endtime = GetNowTime(); passedtime = endtime - starttime; return passedtime; }

		//�Q�b�^�[��
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
		enum FPSTIMERTYPE_{//FPS�^�C�}�[�̎��
			FPSTIMERTYPE_1FRAME,	//1�t���[�����Ƃ�FPS�擾
			FPSTIMERTYPE_SETFRAMECOUNTER,//�w�肵���t���[���o�ߎ��X�V
			FPSTIMERTYPE_SETPASSEDTIMER,//�w�莞��(ms)�o�ߎ��X�V
		};
		FPSTimerClass(){ fps = 0; framecounter = lateststarttime = 0; }
		virtual ~FPSTimerClass(){}
		//���݂̎��Ԃ��擾
		static unsigned long GetNowTime(){ return TimerClass::GetNowTime(); }
		//FPS�^�C�}�[�Z�b�g(type,�g�p����type���ƂɃp�����[�^���w��)
		//TYPE_1FRAME			param:�Ӗ��Ȃ�(�Ȃ�ł���)	��Ԓl���r��܂�
		//TYPE_SETFRAMECOUNTER	param:�t���[����(���t���[���ȏ�o�߂�fps�X�V���邩)
		//TYPE_SETTIMER			param:�w�莞��(ms)(�Ȃ�ms�ȏ�o�߂�fps�X�V���邩)
		float SetFPSTimer(const FPSTIMERTYPE_ fpstype, const unsigned long param){

			switch (fpstype){
			case FPSTIMERTYPE_1FRAME://1�t���[�����Ƃ�FPS�X�V
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
			case FPSTIMERTYPE_SETFRAMECOUNTER:	//���t���[���o�߂��Ƃ�FPS�X�V
			{
													framecounter++;
													if (framecounter >= param){
														unsigned long nowtime = timeGetTime();
														unsigned long passedtime = nowtime - lateststarttime;
														if (passedtime){//�O�̂���
															fps = (float)framecounter / (float)passedtime * 1000;
															lateststarttime = nowtime;
															framecounter = 0;
														}
														else
															fps = 0; framecounter = 0;
													}
			}
				break;
			case FPSTIMERTYPE_SETPASSEDTIMER:	//��莞�Ԍo�߂��Ƃ�fps�X�V
			{
													unsigned long nowtime = timeGetTime();
													unsigned long passedtime = nowtime - lateststarttime;
													framecounter++;
													if (passedtime >= param){
														if (passedtime){//�O�̂���
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