project(ftpmirror)
INCLUDE_DIRECTORIES("/home/projects/FtpMirror")
add_executable(ftpmirror TcpClient.cpp FtpClient.cpp Exception.cpp main.cpp)
target_link_libraries(ftpmirror ${pthread} -lpthread )
