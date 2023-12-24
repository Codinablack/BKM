#include "NetworkManager.hpp"

namespace BKM {

    NetworkManager& NetworkManager::getInstance() {
        static NetworkManager instance;
        return instance;
    }

    void NetworkManager::startServer(const std::string& address, int port) {

        // TODO
        // Create Server struct/class
        // Make different servers for different needs, ie. database, login server, game server, etc
        // Need Validation on the address and port before getting here, or after.
        // Change this method(name) to match the type of server being started.
        asio::error_code udpError;
        asio::error_code tcpError;
        asio::io_context context;
        nm_udp_socket = makeUdpSocket_ptr(context);
        nm_tcp_socket = makeTcpSocket_ptr(context);

        asio::ip::udp::endpoint udp_endpoint(asio::ip::make_address(address, udpError), port + 1);
        asio::ip::tcp::endpoint tcp_endpoint(asio::ip::make_address(address, tcpError), port);
        nm_udp_socket->connect(udp_endpoint, udpError);
        nm_tcp_socket->connect(tcp_endpoint, tcpError);

    }

    void NetworkManager::stopServer() {

    }

}