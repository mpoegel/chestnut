file(GLOB HTML_STATIC_FILES *.html)
file(GLOB CSS_STATIC_FILES *.css)

install(FILES ${HTML_STATIC_FILES} DESTINATION etc/chestnut/static)
install(FILES ${CSS_STATIC_FILES} DESTINATION etc/chestnut/static)
