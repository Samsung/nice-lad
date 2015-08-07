CONFIGURE_FILE(nice_lad.conf.in nice_lad.conf @ONLY)

INSTALL(FILES
    ${CMAKE_BINARY_DIR}/conf/nice_lad.conf
    DESTINATION
    ${SYS_CONFIG_DIR}/audisp/plugins.d
)
