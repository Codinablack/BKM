#ifndef NETWORK_MANAGER_HPP
#define NETWORK_MANAGER_HPP

#include <asio.hpp>
#include <asio/io_context.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <memory>
#include <string>

/// <summary>
/// Here we are predefining our pointertypes to save on some repeated and extended typing
/// By doing this, we also ensure consistent pointer types and expectations.
/// </summary>
typedef std::shared_ptr<BasicNetController> BasicNetController_ptr;
typedef std::shared_ptr<NetServer> NetServer_ptr;
typedef std::shared_ptr<NetClient> NetClient_ptr;
typedef std::shared_ptr<GameServer> GameServer_ptr;
typedef std::shared_ptr<LoginServer> LoginServer_ptr;
typedef std::shared_ptr<LoginClient> LoginClient_ptr;
typedef std::shared_ptr<GameClient> GameClient_ptr;
typedef std::shared_ptr<DatabaseClient> DatabaseClient_ptr;

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
protected: // We Make the constructor private to enforce our 'Create' method

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

public:	/// We disable copy and assignment to reinforce creation control

	BasicNetController(const BasicNetController&) = delete;
	BasicNetController& operator=(const BasicNetController&) = delete;
	~BasicNetController() noexcept = default;
	// Our construction methods :: Factory Patterns
	static BasicNetController_ptr create( const std::string& address, int port)
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

	virtual void init() {}

private:
	std::string _net_address;
	int _main_port;
	int _aux_port;
};
// Servers
class NetServer : BasicNetController {
protected:	// We Make the constructor private to enforce our 'Create' method
	NetServer(const std::string& address, int port) : BasicNetController(address, port) {};
	NetServer(const std::string& address, int main_port, int aux_port) : BasicNetController(address, main_port, aux_port) {};

public: /// We disable copy and assignment to reinforce creation control

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

	virtual void init() {}
};

class GameServer : NetServer {
protected:	// We Make the constructor private to enforce our 'Create' method
	GameServer(const std::string& address, int port) : NetServer(address, port) {};
	GameServer(const std::string& address, int main_port, int aux_port) : NetServer(address, main_port, aux_port) {};

public:	/// We disable copy and assignment to reinforce creation control

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
		return std::dynamic_pointer_cast<GameServer>(getNetServer());
	}

	virtual void init() {}
};

class LoginServer : NetServer {
protected:	// We Make the constructor private to enforce our 'Create' method
	LoginServer(const std::string& address, int port) : NetServer(address, port) {};
	LoginServer(const std::string& address, int main_port, int aux_port) : NetServer(address, main_port, aux_port) {};

public:	/// We disable copy and assignment to reinforce creation control

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
		return std::dynamic_pointer_cast<LoginServer>(getNetServer());
	}

	virtual void init() {}
};
// Clients
class NetClient : BasicNetController {
protected:	// We Make the constructor private to enforce our 'Create' method
	NetClient(const std::string& address, int port) : BasicNetController(address, port) {};
	NetClient(const std::string& address, int main_port, int aux_port) : BasicNetController(address, main_port, aux_port) {};

public:	/// We disable copy and assignment to reinforce creation control

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
		return std::dynamic_pointer_cast<NetClient>(getBasicNetController());
	}

	virtual void init() {}
};

class LoginClient : NetClient {
protected:	// We Make the constructor private to enforce our 'Create' method
	LoginClient(const std::string& address, int port) : NetClient(address, port) {};
	LoginClient(const std::string& address, int main_port, int aux_port) : NetClient(address, main_port, aux_port) {};

public:	/// We disable copy and assignment to reinforce creation control

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
		return std::dynamic_pointer_cast<LoginClient>(getNetClient());
	}

	virtual void init() {}
};

class GameClient : NetClient {
protected:	// We Make the constructor private to enforce our 'Create' method
	GameClient(const std::string& address, int port) : NetClient(address, port) {};
	GameClient(const std::string& address, int main_port, int aux_port) : NetClient(address, main_port, aux_port) {};

public:	/// We disable copy and assignment to reinforce creation control

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
		return std::dynamic_pointer_cast<GameClient>(getNetClient());
	}

	virtual void init() {}
};

class DatabaseClient : NetClient {
protected:	// We Make the constructor private to enforce our 'Create' method
	DatabaseClient(const std::string& address, int port) : NetClient(address, port) {};
	DatabaseClient(const std::string& address, int main_port, int aux_port) : NetClient(address, main_port, aux_port) {};

public:	/// We disable copy and assignment to reinforce creation control

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
		return std::dynamic_pointer_cast<DatabaseClient>(getNetClient());
	}

	virtual void init() {}
};


typedef std::shared_ptr<asio::ip::udp::socket> UdpSocket_ptr;
typedef std::shared_ptr<asio::ip::tcp::socket> TcpSocket_ptr;

UdpSocket_ptr makeUdpSocket_ptr(asio::io_context& context) {
	return std::make_shared<asio::ip::udp::socket>(context);
}

TcpSocket_ptr makeTcpSocket_ptr(asio::io_context& context) {
	return std::make_shared<asio::ip::tcp::socket>(context);
}

class NetworkManager {
public:
  static NetworkManager& getInstance();

  void startServer(const std::string& address, int port);
  static void stopServer();

private:
  // Private constructor to enforce singleton pattern
  NetworkManager() = default;

  // Sockets
  UdpSocket_ptr nm_udp_socket;
  TcpSocket_ptr nm_tcp_socket;
  TcpSocket_ptr login_socket;
  TcpSocket_ptr database_socket;

  // Prevent copy construction and assignment
  NetworkManager(const NetworkManager&) = delete;
  NetworkManager& operator=(const NetworkManager&) = delete;
};

#endif
