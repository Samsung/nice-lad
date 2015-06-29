CONFIGURE_FILE(nice_lad.conf.in nice_lad.conf @ONLY)

INSTALL(FILES
    nice_lad.conf
    DESTINATION
    ${SYS_CONFIG_DIR}/audisp/plugins.d
)
