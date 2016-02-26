#include "NetWork\Nettest.hpp"
#include "Engine\Core\Connector.hpp"
#include "ExtendTemplateLibrary\StringGetter.hpp"
Net::NNS::udp::endpoint ep;
void hoge(std::size_t t,const char * h, Net::NNS::udp::endpoint& ep) {
	std::cout << "GetMsg:";
	std::cout << h;
	std::cout <<"	len:"<<t<<std::endl;
}
int main() {
	Engine::Core::Connector connector;
	
	rewind(stdin);
	getchar();
	return 0;
}