#ifndef NETWORK_MANAGER_HPP
#define NETWORK_MANAGER_HPP

#include <asio.hpp>
#include <asio/io_context.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <memory>
#include <string>

#include "networkcontroller.hpp"

namespace BKM {

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
}
#endif
