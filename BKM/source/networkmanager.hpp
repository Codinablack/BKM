#ifndef NETWORK_MANAGER_HPP
#define NETWORK_MANAGER_HPP

#include <asio.hpp>
#include <asio/io_context.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <memory>
#include <string>

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
  static UdpSocket_ptr nm_udp_socket;
  static TcpSocket_ptr nm_tcp_socket;

  // Prevent copy construction and assignment
  NetworkManager(const NetworkManager&) = delete;
  NetworkManager& operator=(const NetworkManager&) = delete;
};

#endif
