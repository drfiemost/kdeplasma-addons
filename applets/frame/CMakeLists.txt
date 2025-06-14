project(plasma-frame)

find_package(Exiv2 0.21)
set_package_properties(Exiv2 PROPERTIES DESCRIPTION "Image Tag reader"
                       URL "http://www.exiv2.org" TYPE OPTIONAL
                       PURPOSE "RECOMMENDED: Enables automatic rotation for frame applet")

set(frame_SRCS
    frame.cpp
    slideshow.cpp
    picture.cpp
    imagescaler.cpp
    imageloader.cpp
    configdialog.cpp)

kde4_add_ui_files(frame_SRCS imageSettings.ui appearanceSettings.ui)

kde4_add_plugin(plasma_applet_frame ${frame_SRCS})


if(EXIV2_FOUND)
    include_directories( ${EXIV2_INCLUDE_DIR} )
    set_source_files_properties(imageloader.cpp PROPERTIES
                               COMPILE_FLAGS "-DHAVE_EXIV2 ${KDE4_ENABLE_EXCEPTIONS}")
    target_link_libraries(plasma_applet_frame ${KDE4_PLASMA_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KFILE_LIBS} ${EXIV2_LIBRARIES} )
else ()
    target_link_libraries(plasma_applet_frame ${KDE4_PLASMA_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KFILE_LIBS} )
endif()

install(TARGETS plasma_applet_frame DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-frame-default.desktop DESTINATION ${SERVICES_INSTALL_DIR})
#install(FILES picture-frame-default.jpg DESTINATION ${DATA_INSTALL_DIR}/desktoptheme/default/widgets/)

install(FILES picture-frame-default.jpg DESTINATION ${DATA_INSTALL_DIR}/plasma-applet-frame/)
