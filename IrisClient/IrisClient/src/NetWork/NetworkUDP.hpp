#pragma once

#ifndef __PRECOMPILEHEADERLOADED__
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <memory>
#endif
#include "../ExtendTemplateLibrary/Random.hpp"

namespace Net{
	namespace NNS {
		namespace asio = boost::asio;
		namespace ip = asio::ip;
		using udp = ip::udp;
		using tcp = ip::tcp;
	}
	class NetworkUDP {
	private:
		NNS::asio::io_service _io_service;//中核となるクラス
		std::unique_ptr< NNS::udp::socket> _psock;
		UINT _portNo = 0;
			
		boost::array<char, 2048> _recv_buf;
		NNS::udp::endpoint remote_endpoint;//受信元の情報格納先
		std::function<void(std::size_t, const char*,NNS::udp::endpoint&)>callfunc;//呼び出される関数
		void receive() {
			using namespace NNS;
			try {
				boost::system::error_code error;
				auto& recv_ramda = [&](const boost::system::error_code&, std::size_t len) {
					callfunc(len, _recv_buf.data(), remote_endpoint);
				};
				_psock->async_receive_from(asio::buffer(_recv_buf), remote_endpoint, recv_ramda);
				_io_service.run();
			}
			catch (std::exception& e) {
				std::cerr << "error:" << e.what() << std::endl;
			}
		}
	public:
		///指定ポートでの生成
		//今のところv4で
		NetworkUDP(unsigned portNo_) {
			_portNo = portNo_;
			using namespace NNS;
			NNS::udp::endpoint ep(udp::v4(),portNo_);
			try {
				_psock.reset( new NNS::udp::socket(_io_service, ep));
			}
			catch(std::exception e){
				EMSG(e.what());
				EMSG("SOCKETの生成に失敗しました。Port番号が重複しているかもしれません。");
				assert(1);
			}
			std::cout << "ポート番号:" << _portNo << "　でソケットを生成しました." << std::endl;
		}
		///ランダムポートでの生成
		//乱数適当なので後で修正
		NetworkUDP() {
			auto rmd = etd::Random::GetIntegerDistribution<int>(10000, 65535);
			auto mt = etd::Random::GetMersenne();
			forto(60000) {
				auto port = rmd(mt);
				try {
					_psock.reset(new NNS::udp::socket(_io_service, NNS::udp::endpoint(NNS::udp::v4(),port)));
				}
				catch (std::exception e) {
					//ELOG("portOverlap:%d\n",port);
					continue;
				}
				_portNo = port;
				std::cout << "ポート番号:" << _portNo << "　でソケットを生成しました." << std::endl;
				break;
			}
		}
		///非同期受信
		//char _recv_buf[2048];
		//void Recv_async( std::function<void(std::size_t, const char*)> callfunc_) {
		//	using namespace NNS;
		//	try {
		//		boost::system::error_code error;
		//		udp::endpoint remote_endpoint;//受信元の情報格納先
		//		auto& recv_ramda = [&](const boost::system::error_code&, std::size_t len) {
		//			callfunc(len, _recv_buf.data()); 
		//		};
		//		_psock->async_receive_from(asio::buffer(_recv_buf), remote_endpoint, recv_ramda);
		//		_io_service.run();
		//	}
		//	catch (std::exception& e) {
		//		std::cerr << "error:" << e.what() << std::endl;
		//	}
		//	//実験
		//}
		void SetCallFunction(std::function<void(std::size_t, const char*, NNS::udp::endpoint&)> callfunc_) {
			callfunc = std::move(callfunc_);
		}
		bool receiving = false;
		static void Start_Async(NetworkUDP& nt) {
			nt.receiving = true;
			while(nt.receiving)
				nt.receive();
		}
		static void Start_AsyncOnce(NetworkUDP& nt) {
			nt.receive();
	
		}
		void Stop_async() {
			receiving = false;
			_io_service.stop();

		}
		void Recv() {
			using namespace NNS;
			//受信（受信できるまで待機される）
			udp::endpoint remote_endpoint;
			size_t len = _psock->receive_from(boost::asio::buffer(_recv_buf), remote_endpoint);
			callfunc(len, _recv_buf.data(), remote_endpoint);
		}
		void Send(const std::string& msg, const char* ipaddr, UINT portNo_) {
			using namespace NNS;
			try {
				udp::endpoint to_endpoint(ip::address::from_string(ipaddr), portNo_);//送信先IP,ポート指定
				_psock->send_to(asio::buffer(msg), to_endpoint);
			}
			catch (std::exception& e) {
				std::cerr << "error:" << e.what() << std::endl;
			}
		}
		void Send(const std::string& msg, const NNS::udp::endpoint &to_endpoint) {
			_psock->send_to(NNS::asio::buffer(msg), to_endpoint);
		}
	};
	
}