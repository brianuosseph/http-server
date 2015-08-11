CXX 			= clang++
DEBUG 		= -g
CPP_FLAGS = -Wall -Wextra -std=c++11
EXPORT 		= -o server

LIBS 			= -L. \
#						-lcurl # Replace in-house url coding with libcurl
INCLUDES 	=	-I.

OBJS			= main.o web_server.o http_response.o http_handler.o

all: server

server: $(OBJS)
	$(CXX) $(CPP_FLAGS) $(OBJS) $(EXPORT)

# Objects
main.o: src/main.cpp
	$(CXX) $(CPP_FLAGS) -c $^ -o $@

web_server.o: src/web_server.cpp src/http_request.h src/http_response.h
	$(CXX) $(CPP_FLAGS) -c $< -o $@

http_handler.o: src/http_handler.cpp src/url/util.h
	$(CXX) $(CPP_FLAGS) -c $< -o $@

http_response.o: src/http_response.cpp src/http_util.h
	$(CXX) $(CPP_FLAGS) -c $< -o $@

clean:
	rm -f *.o