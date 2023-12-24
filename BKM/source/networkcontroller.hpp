#ifndef _NETWORKCONTROLLER_HPP
#define _NETWORKCONTROLLER_HPP


#include <asio.hpp>
#include <asio/io_context.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <memory>
#include <string>
#include <iostream>

#include "console.hpp"


/// <summary>
/// Here we are predefining our pointertypes to save on some repeated and extended typing
/// By doing this, we also ensure consistent pointer types and expectations.
/// </summary>

namespace BKM {

	class BasicNetController;
	class NetServer;
	class NetClient;
	class GameServer;
	class GameClient;
	class LoginServer;
	class LoginClient;
	class DatabaseClient;

	using BasicNetController_ptr = std::shared_ptr<BasicNetController>;
	using NetServer_ptr = std::shared_ptr<NetServer>;
	using NetClient_ptr = std::shared_ptr<NetClient>;
	using GameServer_ptr = std::shared_ptr<GameServer>;
	using GameClient_ptr = std::shared_ptr<GameClient>;
	using LoginServer_ptr = std::shared_ptr<LoginServer>;
	using LoginClient_ptr = std::shared_ptr<LoginClient>;
	using DatabaseClient_ptr = std::shared_ptr<DatabaseClient>;
	using UdpSocket_ptr = std::shared_ptr<asio::ip::udp::socket>;
	using TcpSocket_ptr = std::shared_ptr<asio::ip::tcp::socket>;

	UdpSocket_ptr makeUdpSocket_ptr(asio::io_context& context) {
		return std::make_shared<asio::ip::udp::socket>(context);
	}

	TcpSocket_ptr makeTcpSocket_ptr(asio::io_context& context) {
		return std::make_shared<asio::ip::tcp::socket>(context);
	}

	/// <summary>
	/// We create our most basic network class using enabled_shared_from_this to allow perfect use with shared pointers,
	/// not only for the base class but for all derived classes and their various pointers.
	/// </summary>
	/// <param name="address"> The ip address for the destination connection </param>
	/// <param name="port"> The port number for the desitination connection </param>
	/// <param name="main_port"> Same as port, but used when two ports are needed (gameserver) </param>
	/// <param name="aux_port"> Secondary port number for use when more than one port is needed (gameserver) </param>
	/// <returns></returns>
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

		std::string _net_address;
		int _main_port;
		int _aux_port;

		BasicNetController(const BasicNetController&) = delete;
		BasicNetController& operator=(const BasicNetController&) = delete;
		~BasicNetController() noexcept = default;

		// Our construction methods :: Factory Patterns
		static BasicNetController_ptr create(const std::string& address, int port)
		{
			return std::make_shared<BasicNetController>(address, port);
		}

		static BasicNetController_ptr create(const std::string& address, int main_port, int aux_port)
		{
			return std::make_shared<BasicNetController>(address, main_port, aux_port);
		}
		// This serves as a get.self function
		BasicNetController_ptr getBasicNetController() {
			return shared_from_this();
		}

		std::string getAddress() const { return _net_address; }
		int getMainPort() const { return _main_port; }
		int getAuxPort() const { return _aux_port; }
		virtual void init() {}

	private:

	};
	// Servers
	class NetServer : public BasicNetController {
	public:

		NetServer(const std::string& address, int port) : BasicNetController(address, port) {};
		NetServer(const std::string& address, int main_port, int aux_port) : BasicNetController(address, main_port, aux_port) {};
		NetServer(const NetServer&) = delete;
		NetServer& operator=(const NetServer&) = delete;
		~NetServer() noexcept = default;

		// Our construction methods :: Factory Patterns
		static NetServer_ptr create(const std::string& address, int port) {
			return std::make_shared<NetServer>(address, port);
		}

		static NetServer_ptr create(const std::string& address, int main_port, int aux_port) {
			return std::make_shared<NetServer>(address, main_port, aux_port);
		}
		// This serves as a get.self function
		virtual NetServer_ptr getNetServer() {
			return std::dynamic_pointer_cast<NetServer>(getBasicNetController());
		}

		virtual void init() override {}
	};

	class GameServer : public NetServer {
	public:

		GameServer(const std::string& address, int port) : NetServer(address, port) {};
		GameServer(const std::string& address, int main_port, int aux_port) : NetServer(address, main_port, aux_port) {};
		GameServer(const GameServer&) = delete;
		GameServer& operator=(const GameServer&) = delete;
		~GameServer() noexcept = default;

		// Our construction methods :: Factory Patterns
		static GameServer_ptr create(const std::string& address, int port) {
			return std::make_shared<GameServer>(address, port);
		}

		static GameServer_ptr create(const std::string& address, int main_port, int aux_port) {
			return std::make_shared<GameServer>(address, main_port, aux_port);
		}
		// This serves as a get.self function
		virtual GameServer_ptr getGameServer() {
			return std::dynamic_pointer_cast<GameServer>(shared_from_this());
		}

		virtual void init() override {
			asio::error_code udpError;
			asio::error_code tcpError;
			asio::io_context context;
			UdpSocket_ptr udpSocket = makeUdpSocket_ptr(context);
			TcpSocket_ptr tcpSocket = makeTcpSocket_ptr(context);
			asio::ip::udp::endpoint udp_endpoint(asio::ip::make_address(getAddress(), udpError), getAuxPort());
			asio::ip::tcp::endpoint tcp_endpoint(asio::ip::make_address(getAddress(), tcpError), getMainPort());

			udpSocket->connect(udp_endpoint, udpError);
			tcpSocket->connect(tcp_endpoint, tcpError);

			if (!udpError) {
				Console::Print << "Udp Connection Established with ip address : " << getAddress() << " on port : " << getAuxPort() << std::endl;
			}
			else {
				Console::Print << "Udp Connection Failed with error : " << udpError.message() << std::endl;
			}

			if (!tcpError) {
				Console::Print << "Tcp Connection Established with ip address : " << getAddress() << " on port : " << getMainPort() << std::endl;
			}
			else {
				Console::Print << "Tcp Connection Failed with error : " << tcpError.message() << std::endl;
			}

		}
	};

	class LoginServer : public NetServer {
	public:

		LoginServer(const std::string& address, int port) : NetServer(address, port) {};
		LoginServer(const std::string& address, int main_port, int aux_port) : NetServer(address, main_port, aux_port) {};
		LoginServer(const LoginServer&) = delete;
		LoginServer& operator=(const LoginServer&) = delete;
		~LoginServer() noexcept = default;

		// Our construction methods :: Factory Patterns
		static LoginServer_ptr create(const std::string& address, int port) {
			return std::make_shared<LoginServer>(address, port);
		}

		static LoginServer_ptr create(const std::string& address, int main_port, int aux_port) {
			return std::make_shared<LoginServer>(address, main_port, aux_port);
		}
		// This serves as a get.self function
		virtual LoginServer_ptr getLoginServer() {
			return std::dynamic_pointer_cast<LoginServer>(shared_from_this());
		}

		virtual void init() override {}
	};
	// Clients
	class NetClient : public BasicNetController {
	public:

		NetClient(const std::string& address, int port) : BasicNetController(address, port) {};
		NetClient(const std::string& address, int main_port, int aux_port) : BasicNetController(address, main_port, aux_port) {};
		NetClient(const NetClient&) = delete;
		NetClient& operator=(const NetClient&) = delete;
		~NetClient() noexcept = default;

		// Our construction methods :: Factory Patterns
		static NetClient_ptr create(const std::string& address, int port) {
			return std::make_shared<NetClient>(address, port);
		}

		static NetClient_ptr create(const std::string& address, int main_port, int aux_port) {
			return std::make_shared<NetClient>(address, main_port, aux_port);
		}
		// This serves as a get.self function
		virtual NetClient_ptr getNetClient() {
			return std::dynamic_pointer_cast<NetClient>(shared_from_this());
		}

		virtual void init() override {}
	};

	class LoginClient : public NetClient {
	public:

		LoginClient(const std::string& address, int port) : NetClient(address, port) {};
		LoginClient(const std::string& address, int main_port, int aux_port) : NetClient(address, main_port, aux_port) {};
		LoginClient(const LoginClient&) = delete;
		LoginClient& operator=(const LoginClient&) = delete;
		~LoginClient() noexcept = default;

		// Our construction methods :: Factory Patterns
		static LoginClient_ptr create(const std::string& address, int port) {
			return std::make_shared<LoginClient>(address, port);
		}

		static LoginClient_ptr create(const std::string& address, int main_port, int aux_port) {
			return std::make_shared<LoginClient>(address, main_port, aux_port);
		}
		// This serves as a get.self function
		virtual LoginClient_ptr getLoginClient() {
			return std::dynamic_pointer_cast<LoginClient>(shared_from_this());
		}

		virtual void init() override {}
	};

	class GameClient : public NetClient {
	public:

		GameClient(const std::string& address, int port) : NetClient(address, port) {};
		GameClient(const std::string& address, int main_port, int aux_port) : NetClient(address, main_port, aux_port) {};
		GameClient(const GameClient&) = delete;
		GameClient& operator=(const GameClient&) = delete;
		~GameClient() noexcept = default;

		// Our construction methods :: Factory Patterns
		static GameClient_ptr create(const std::string& address, int port) {
			return std::make_shared<GameClient>(address, port);
		}

		static GameClient_ptr create(const std::string& address, int main_port, int aux_port) {
			return std::make_shared<GameClient>(address, main_port, aux_port);
		}
		// This serves as a get.self function
		virtual GameClient_ptr getGameClient() {
			return std::dynamic_pointer_cast<GameClient>(shared_from_this());
		}

		virtual void init() override {}
	};

	class DatabaseClient : public NetClient {
	public:

		DatabaseClient(const std::string& address, int port) : NetClient(address, port) {};
		DatabaseClient(const std::string& address, int main_port, int aux_port) : NetClient(address, main_port, aux_port) {};
		DatabaseClient(const DatabaseClient&) = delete;
		DatabaseClient& operator=(const DatabaseClient&) = delete;
		~DatabaseClient() noexcept = default;

		// Our construction methods :: Factory Patterns
		static DatabaseClient_ptr create(const std::string& address, int port) {
			return std::make_shared<DatabaseClient>(address, port);
		}

		static DatabaseClient_ptr create(const std::string& address, int main_port, int aux_port) {
			return std::make_shared<DatabaseClient>(address, main_port, aux_port);
		}
		// This serves as a get.self function
		virtual DatabaseClient_ptr getDatabaseClient() {
			return std::dynamic_pointer_cast<DatabaseClient>(shared_from_this());
		}

		virtual void init() override {}
	};

} // namespace


#endif 
