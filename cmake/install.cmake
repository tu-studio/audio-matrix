# for CMAKE_INSTALL_INCLUDEDIR definition
include(GNUInstallDirs)

message(STATUS "CMAKE_INSTALL_PREFIX is set to ${CMAKE_INSTALL_PREFIX}")

# install the target and create export-set
install(TARGETS ${PROJECT_NAME}
    # these get default values from GNUInstallDirs
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)