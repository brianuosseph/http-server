#include "web_server.h"
#include "http_request.h"
#include "http_response.h"

#include <sstream>

// TODO:
//  - CGI Script support (dynamic pages)
//    - Take URL query variables and use them as environment
//    - variables for a program
//    - Shell out request CGI headers to program. And send
//      stdout of program to client.
//  - Cookies
//  - Create simple question survey using CGI scripts to create
//    and print questions, and cookies from keeping track of
//    number of wrong answers and print results

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
  // No longer need server_info_
  freeaddrinfo(server_info_);
  // Set up signal handling
  set_reaper();
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

// Abstraction of socket() to include error checking. Also sets socket
// options to allow for quick port reuse after closing program.
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
  // Set socket options
  int option_value = 1;
  if (setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR,
                 &option_value, sizeof option_value) == -1) {
    perror("Couldn't set socket options");
    exit(1);
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

// Signal hanlder for zombies.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
void sigchld_handler(int signal) {
  while (waitpid(-1, NULL, WNOHANG) > 0);
}
#pragma clang diagnostic pop

// Sets up the signal handler used for handling zombies.
void WebServer::set_reaper() {
  memset(&signal_action_, 0, sizeof(struct sigaction));
  sigemptyset(&signal_action_.sa_mask);
  signal_action_.sa_handler = sigchld_handler;
  signal_action_.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &signal_action_, NULL) == -1) {
    perror("Error setting up signal handler");
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
  std::cout << "Waiting for connections..."
            << std::endl << std::endl;
  while (true) {
    // Accept connection, if there's an error skip (and do not fork).
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
    std::cout << "Connected to "
              << client_ip_ << std::endl;
    // Fork and handle request in child process
    int cpid = fork();
    if (cpid == -1) {
      perror("Error creating child process");
    }
    // Child process
    else if (cpid == 0) {
      // Child no longer needs listen
      close(socket_);
      std::string msg = get_message();
      // Determine if it's an HTTP request
      HttpRequest request = handler_.parse_message(msg);
      // Create HTTP response
      HttpResponse response
        = handler_.create_response(request, web_directory_path_);
      // Send response
      // TODO: CGI Support (Dynamic pages)
      // - Accpet query variables and shell to other program
      // - Stdout of program is response text
      respond_with_static_page(response);
      // TODO: Persistent connections
      // - Only close connection if requested by user, or
      // - If server responds with Connection header specifying the
      //   client to close the connection and stop sending requests
      close(client_socket_);
      exit(0);
    }
    // Parent process
    else {
      // Client socket handed to child, close in parent process.
      close(client_socket_);
    }
  }
}

// Gets message from `client_socket_`.
std::string WebServer::get_message() {
  std::cout << "Request from "
            << client_ip_ << std::endl;
  std::stringstream message;
  int bytes_recieved = 0;
  while (true) {
    bytes_recieved = recv(client_socket_, message_buffer_, MSG_BUF_LEN, 0);
    if (bytes_recieved == -1) {
      perror("Error getting message");
      exit(1);
    }
    if (bytes_recieved == 0) {
      std::cout << client_ip_ << " closed connection";
      break;
    }
    if (bytes_recieved < MSG_BUF_LEN) {
      for (int i = 0; i < bytes_recieved; ++i) {
        message << message_buffer_[i];
      }
      break;
    }
    else {
      message << message_buffer_;
    }
  }
  std::cout << message.str() << std::endl;
  std::cout << "----------------------------------------"
            << std::endl;
  return message.str();
}

// Sends message to `client_socket_`.
void WebServer::send_message(std::string message) {
  std::cout << "Response to "
            << client_ip_
            << std::endl;
  int message_len = strlen(message.c_str());
  int bytes_sent = 0;
  int total_bytes_sent = 0;
  while (total_bytes_sent != message_len) {
    bytes_sent = send(client_socket_, message.c_str(), message_len, 0);
    if (bytes_sent == -1) {
      perror("Error sending message");
      break;
      exit(1);
    }
    else {
      total_bytes_sent += bytes_sent;
    }
  }
  std::cout << message
            << std::endl
            << "----------------------------------------"
            << std::endl;
}

void WebServer::respond_with_static_page(HttpResponse response) {
  // Create HTTP message header
  std::string message_header = response.create_message_header();
  int message_len = strlen(message_header.c_str());
  int bytes_sent = 0;
  int total_bytes_sent = 0;
  while (total_bytes_sent != message_len) {
    bytes_sent = send(client_socket_, message_header.c_str(), message_len, 0);
    if (bytes_sent == -1) {
      perror("Error sending message header");
      break;
      exit(1);
    }
    else {
      total_bytes_sent += bytes_sent;
    }
  }
  std::cout << message_header
            << std::endl;
  // Send file data
  if (!response.resource_path.empty()) {
    char buffer[512];
    std::ifstream file;
    file.open(response.resource_path, std::ios::in | std::ios::binary);
    if (file.is_open()) {
      // Get file length
      file.seekg(0, file.end);
      int total_bytes = file.tellg();
      file.seekg(0, file.beg);
      // Read file and 
      int bytes_read = 0;
      int bytes_sent = 0;
      int total_bytes_sent = 0;
      while (total_bytes_sent != total_bytes) {
        file.read(buffer, 512);
        bytes_read = file.gcount();

        bytes_sent = send(client_socket_, buffer, bytes_read, 0);
        if (bytes_sent == -1 || bytes_sent != bytes_read) {
          perror("Error sending message");
          break;
          exit(1);
        }
        else {
          total_bytes_sent += bytes_sent;
        }
      }
    }
    else {
      std::cout << "Error opening requested file"
                << std::endl;
    }
    file.close();
  }
  std::cout << "----------------------------------------"
            << std::endl;
}


