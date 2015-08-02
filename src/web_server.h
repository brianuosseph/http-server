#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <iostream>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "http_handler.h"

#define IP "127.0.0.1"
#define PORT "8080"
#define WWW_DIR_PATH "./www"
#define BACKLOG 5
#define MSG_BUF_LEN 512

class WebServer {
private:
  std::string ip_;
  std::string port_;
  std::string web_directory_path_;

  // Server listening socket
  int socket_ = -1;
  struct addrinfo* server_info_;

  // Client socket
  struct sockaddr_storage client_addr_;
  int client_socket_ = -1;
  socklen_t client_socket_size_;
  char client_ip_[INET6_ADDRSTRLEN];

  // Set up functions
  void get_server_info();
  void create_socket(int domain,
                     int type,
                     int protocol);
  void bind_socket(int socket,
                   struct sockaddr* socket_address,
                   int address_len);
  void listen_to(int socket,
                 int backlog);
  void* get_ip_address(struct sockaddr* socket_address);

  // Message handling
  HttpHandler handler_;
  char message_buffer_[MSG_BUF_LEN];
  void get_message();
  void send_message(std::string message);

  // File handling


public:
  WebServer();
  ~WebServer();
  void run();
};

#endif  // WEB_SERVER_H
