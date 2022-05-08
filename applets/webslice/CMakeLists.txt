project(webslice)

set(webslice_SRCS
    main.cpp
    kgraphicswebslice.cpp
    kwebslice.cpp
)

KDE4_ADD_EXECUTABLE(webslice ${webslice_SRCS})

target_link_libraries(webslice
    Qt4::QtCore
    Qt4::QtNetwork
    Qt4::QtGui
    Qt4::QtWebKit
    Qt4::QtScript
)

add_subdirectory(plasma)
