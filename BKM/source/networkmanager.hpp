#ifndef NETWORK_MANAGER_HPP
#define NETWORK_MANAGER_HPP

#include <asio.hpp>
#include <asio/io_context.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <memory>
#include <string>

typedef std::shared_ptr<BasicNetController> BasicNetController_ptr;
typedef std::shared_ptr<NetServer> NetServer_ptr;
typedef std::shared_ptr<NetClient> NetClient_ptr;

class BasicNetController : public std::enable_shared_from_this<BasicNetController> {
public:
	BasicNetController(const std::string& address, int port) {
		_net_address = address;
		_main_port = port;
		_aux_port = 0;
	};

	BasicNetController(const std::string& address, int main_port, int aux_port) {
		_net_address = address;
		_main_port = main_port;
		_aux_port = aux_port;
	};

	static BasicNetController_ptr create( const std::string& address, int port)
	{	
		return std::make_shared<BasicNetController>(address, port);
	}

	static BasicNetController_ptr create(const std::string& address, int main_port, int aux_port)
	{
		return std::make_shared<BasicNetController>(address, main_port, aux_port);
	}

	BasicNetController_ptr getBasicNetController() {
		return shared_from_this();
	}
	

private:
	static std::string _net_address;
	static int _main_port;
	static int _aux_port;
};



class NetServer : BasicNetController {
public:
	NetServer(const std::string& address, int port) : BasicNetController(address, port) {};
	NetServer(const std::string& address, int main_port, int aux_port) : BasicNetController(address, main_port, aux_port) {};

	NetServer_ptr create(const std::string& address, int port) {
		return std::make_shared<NetServer>(address, port);
	}

	NetServer_ptr create(const std::string& address, int main_port, int aux_port) {
		return std::make_shared<NetServer>(address, main_port, aux_port);
	}

	virtual NetServer_ptr getNetServer() {
		return std::dynamic_pointer_cast<NetServer>(shared_from_this());
	}
};


class NetClient : BasicNetController {
public:
	NetClient(const std::string& address, int port) : BasicNetController(address, port) {};
	NetClient(const std::string& address, int main_port, int aux_port) : BasicNetController(address, main_port, aux_port) {};

	NetClient_ptr create(const std::string& address, int port) {
		return std::make_shared<NetClient>(address, port);
	}

	NetClient_ptr create(const std::string& address, int main_port, int aux_port) {
		return std::make_shared<NetClient>(address, main_port, aux_port);
	}

	virtual NetClient_ptr getNetClient() {
		return std::dynamic_pointer_cast<NetClient>(shared_from_this());
	}
};

class GameServer : NetServer {};
class LoginServer : NetServer {};

class LoginClient : NetClient {};
class GameClient : NetClient {};
class DataBaseClient : NetClient {};


typedef std::shared_ptr<asio::ip::udp::socket> UdpSocket_ptr;
typedef std::shared_ptr<asio::ip::tcp::socket> TcpSocket_ptr;

UdpSocket_ptr makeUdpSocket_ptr(asio::io_context& context) {
	return std::make_shared<asio::ip::udp::socket>(context);
}

TcpSocket_ptr makeTcpSocket_ptr(asio::io_context& context) {
	return std::make_shared<asio::ip::tcp::socket>(context);
}

struct Server {};

class NetworkManager {
public:
  static NetworkManager& getInstance();

  void startServer(const std::string& address, int port);
  static void stopServer();

private:
  // Private constructor to enforce singleton pattern
  NetworkManager() = default;

  // Sockets
  static UdpSocket_ptr nm_udp_socket;
  static TcpSocket_ptr nm_tcp_socket;
  static TcpSocket_ptr login_socket;
  static TcpSocket_ptr database_socket;

  // Prevent copy construction and assignment
  NetworkManager(const NetworkManager&) = delete;
  NetworkManager& operator=(const NetworkManager&) = delete;
};

#endif
