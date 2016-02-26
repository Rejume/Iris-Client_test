#pragma once

#ifndef __PRECOMPILEHEADERLOADED__
#include <string>
#endif
#include "../../NetWork/NetworkUDP.hpp"
#include "../../ExtendTemplateLibrary/StringGetter.hpp"
namespace Engine {
namespace Core {
	///<sumary>�G���W���Ƃ̐ڑ��Ǘ�</sumary>
	class Connector {
		bool connectionstate = false;
		Net::NetworkUDP netMgr;	//network Manager
		Net::NNS::udp::endpoint serverEndpoint;
		int w_width = 0, w_height = 0;	//windowSize
 	public:
		static auto& Instance() {
			static Connector* inst=nullptr;
			return inst;
		}
		Connector() {
			Instance() = this;
			WaitConnect();
		}
	private:

		struct ConnectMsg{
			char msg[4];
			int width;
			int height;
		};
		///join���b�Z�[�W�󂯂邱�ƂŐڑ�
		static void ConnectMessage(std::size_t t, const char * h, Net::NNS::udp::endpoint& ep) {
			etd::StringGettor strg(h,t);	//��M�p�[�T�[
			const char defmsg[5] = "join";
			if (strg.IsEqual(defmsg)) {
				auto &inst = * Instance();
				//inst.w_width = strg.GetInteger();
				//inst.w_height = strg.GetInteger();
				inst.connectionstate = true;
				Instance()->serverEndpoint = ep;
			}
			else {
				std::cout << "GetMsg:";
				std::cout.write(h, t);
				std::cout << std::endl;
			}
			//std::strncmp(msg->msg, msg->defmsg, 4);
		}
		void WaitConnect() {
			netMgr.SetCallFunction(ConnectMessage);
			while (1) {
				std::cout << "Engine�ڑ��ҋ@�E�E�E(join win_x win_y)";
				netMgr.Recv();
				if (connectionstate) {
					std::cout << "�ڑ����܂���:"<<std::endl;
					SendEngine("FindClient");
					break;
				}
			}
		}
		void SendEngine(const char* msg) {
			netMgr.Send(msg, serverEndpoint);
		}
	};
}
}