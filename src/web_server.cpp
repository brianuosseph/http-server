#include "web_server.h"

WebServer::WebServer() {
  ip_ = IP;
  port_ = PORT;
  web_directory_path_ = WWW_DIR_PATH;

  // Load server_info_
  get_server_info();
  // Pass info to socket for creation
  create_socket(server_info_->ai_family,
                server_info_->ai_socktype,
                server_info_->ai_protocol);
  // Bind socket to port for listening
  bind_socket(socket_,
              server_info_->ai_addr,
              server_info_->ai_addrlen);
  std::cout << "Server socket created and bound to port "
            << port_ << std::endl;
  freeaddrinfo(server_info_);
}

WebServer::~WebServer() {
  close(socket_);
}

// Loads server address struct.
void WebServer::get_server_info() {
  struct addrinfo hints_;
  // Make sure hints_ struct is empty
  memset(&hints_, 0, sizeof hints_);
  // Accept IPv4 or IPv6
  hints_.ai_family = AF_UNSPEC;
  // Use TCP stream socket
  hints_.ai_socktype = SOCK_STREAM;
  // Let getaddrinfo assign local host address to socket
  hints_.ai_flags = AI_PASSIVE;
  int status_ = getaddrinfo(NULL, port_.c_str(), &hints_, &server_info_);
  if (status_ != 0) {
    std::cout << "Error getting address info: "
              << gai_strerror(status_) << std::endl;
    exit(1);
  }
  std::cout << "Obtained server address info" << std::endl;
}

// Abstraction of socket() to include error checking.
void WebServer::create_socket(int domain,
                              int type,
                              int protocol) {
  // Create socket if it doesn't exist.
  if (socket_ == -1) {
    socket_ = socket(domain, type, protocol);
    if (socket_ == -1) {
      perror("Couldn't create socket");
      exit(1);
    }
  }
}

// Abstraction of bind() to include error checking.
void WebServer::bind_socket(int socket,
                            struct sockaddr* socket_address,
                            int address_len) {
  // Remember all ports under 1024 are reserved for the super user.
  if (bind(socket, socket_address, address_len) == -1) {
    perror("Couldn't bind socket to port");
    exit(1);
  }
}

// Abstraction of listen() to include error checking.
void WebServer::listen_to(int socket,
                          int backlog) {
  if (listen(socket, backlog) == -1) {
    perror("Couldn't listen to socket");
    exit(1);
  }
}

// Returns IP address of socket.
void* WebServer::get_ip_address(struct sockaddr* socket_address) {
  // Return IPv4
  if (socket_address->sa_family == AF_INET) {
    return &(((struct sockaddr_in*) socket_address)->sin_addr);
  }
  // Return IPv6
  return &(((struct sockaddr_in6*) socket_address)->sin6_addr);
}

// Start listening for connections and handling requests.
void WebServer::run() {
  listen_to(socket_, BACKLOG);
  std::cout << "Waiting for connections..." << std::endl;
  while (true) {
    // Accept connection, if there's an error skip this
    // iteration (and do not fork).
    client_socket_size_ = sizeof client_addr_;
    client_socket_ = accept(socket_,
                            (struct sockaddr*)& client_addr_,
                            &client_socket_size_);
    if (client_socket_ == -1) {
      perror("Error accpeting client connection");
      continue;
    }
    // Get client IP address
    inet_ntop(client_addr_.ss_family,
              get_ip_address((struct sockaddr*)& client_addr_),
              client_ip_,
              sizeof client_ip_);
    std::cout << "Connection to "
              << client_ip_ << std::endl;
    // Fork and handle request in child process
    if (!fork()) {
      // Child no longer needs listen
      close(socket_);
      // TODO: Get HTTP request
      // Respond to connection
      if (send(client_socket_, "Hello, world!", 13, 0) == -1) {
        perror("Error responding");
      }
      // Close client socket and exit
      close(client_socket_);
      exit(0);
    }
    // Client socket handed to child, close in parent process.
    close(client_socket_);
  }
}