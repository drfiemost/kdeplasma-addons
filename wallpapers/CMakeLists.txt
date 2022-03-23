add_subdirectory(mandelbrot)
add_subdirectory(pattern)
add_subdirectory(potd)
add_subdirectory(virus)
add_subdirectory(weather)

if(KDeclarative_FOUND)
    add_subdirectory(qmlwallpapers)
endif(KDeclarative_FOUND)

if(MARBLE_FOUND)
    add_subdirectory(marble)
endif(MARBLE_FOUND)

