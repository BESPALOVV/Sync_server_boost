
find_package(Boost REQUIRED)

	add_executable (server "server.cpp" "server.h" "Main.cpp")

	include_directories(${Boost_INCLUDE_DIRS})

	link_libraries(${Boost_LIBRARIES})
	
