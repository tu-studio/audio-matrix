# for CMAKE_INSTALL_INCLUDEDIR definition
include(GNUInstallDirs)

message(STATUS "CMAKE_INSTALL_PREFIX is set to ${CMAKE_INSTALL_PREFIX}")

# install the target and create export-set
install(TARGETS ${PROJECT_NAME}
    # these get default values from GNUInstallDirs
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# create symlinks for the binary when versioned installation is enabled
if (VERSIONED_INSTALL)
    install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME}${INSTALL_POSTFIX} ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME})")
endif()