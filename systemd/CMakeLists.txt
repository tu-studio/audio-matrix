configure_file(${CMAKE_CURRENT_SOURCE_DIR}/audio-matrix.service.in ${CMAKE_CURRENT_BINARY_DIR}/audio-matrix.service @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/audio-matrix.service DESTINATION /etc/systemd/user)