
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES} ${QCA2_INCLUDE_DIR}
${QJSON_INCLUDE_DIR})

set(twitter_engine_SRCS
    imagesource.cpp
    usersource.cpp
    timelinesource.cpp
    timelineservice.cpp
    tweetjob.cpp
    twitterengine.cpp
    koauth.cpp
    koauthwebhelper.cpp
)

kde4_add_plugin(plasma_engine_microblog ${twitter_engine_SRCS})
target_link_libraries(plasma_engine_microblog
    ${QTOAUTH_LIBRARY}
    ${KDE4_KIO_LIBS}
    ${KDE4_PLASMA_LIBS}
    #${QT_QTXML_LIBS}
    Qt4::QtNetwork
    ${QCA2_LIBRARIES}
    ${QJSON_LIBRARIES}
    Qt4::QtWebKit
    #${KDE4_KDEUI_LIBS}
    ${KDE4_KDEWEBKIT_LIBRARY}
)

install(TARGETS plasma_engine_microblog DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-dataengine-microblog.desktop DESTINATION ${SERVICES_INSTALL_DIR} )
install(FILES tweet.operations DESTINATION ${DATA_INSTALL_DIR}/plasma/services)

