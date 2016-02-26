#pragma once

#ifndef __PRECOMPILEHEADERLOADED__
#include <list>
#endif
#include "NetworkUDP.hpp"

namespace Net {
	class ChatUDP {
		NetworkUDP* pnetMgr;
		using itr = std::list<NNS::udp::endpoint>::iterator;
		std::list<NNS::udp::endpoint> othremember;
		std::function<void(std::size_t, const char*, NNS::udp::endpoint&)>callfunc;//ŒÄ‚Ño‚³‚ê‚éŠÖ”
		ChatUDP(NetworkUDP& netMgr_) {
			pnetMgr = &netMgr_;
		}
		itr& AddMember(NNS::udp::endpoint& ep) {
			othremember.push_front(ep);
			return othremember.begin();
		}
		int RemoveMember(itr& it) {
			othremember.erase(it);
			return othremember.size();
		}
		void SendAll(const std::string msg_) {
			for (auto& ep : othremember) {
				pnetMgr->Send(msg_, ep);
			}
		}
		void Recv() {
			
		}
	private:
		void ShowRecvMessage() {

		}
	};
}