#pragma once

#ifndef __PRECOMPILEHEADERLOADED__
#include <string>
#endif
#include "../../NetWork/NetworkUDP.hpp"
#include "../../ExtendTemplateLibrary/StringGetter.hpp"
namespace Engine {
namespace Core {
	///<sumary>エンジンとの接続管理</sumary>
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
		///joinメッセージ受けることで接続
		static void ConnectMessage(std::size_t t, const char * h, Net::NNS::udp::endpoint& ep) {
			etd::StringGettor strg(h,t);	//受信パーサー
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
				std::cout << "Engine接続待機・・・(join win_x win_y)";
				netMgr.Recv();
				if (connectionstate) {
					std::cout << "接続しました:"<<std::endl;
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