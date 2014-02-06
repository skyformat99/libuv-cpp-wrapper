CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (examples)

ADD_EXECUTABLE (echo_server
	echo_server.cpp)
TARGET_LINK_LIBRARIES (echo_server
	uv)

ADD_EXECUTABLE (client
	client.cpp)
TARGET_LINK_LIBRARIES (client
	uv)
