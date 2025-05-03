set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Fetch FTXUI --------------------------------------------------------------
#include(FetchContent)
#
#set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
#set(FETCHCONTENT_QUIET OFF)
#
#FetchContent_Declare(ftxui
#        GIT_REPOSITORY https://github.com/arthursonzogni/ftxui.git
#        GIT_TAG        v5.0.0
#        GIT_PROGRESS   TRUE
#        GIT_SHALLOW    TRUE
#        EXCLUDE_FROM_ALL
#)
#FetchContent_MakeAvailable(ftxui)
# ------------------------------------------------------------------------------

add_executable(client src/main.cpp
        src/client/client.cpp
        src/client/client.h)

target_link_libraries(client
        PRIVATE common protocol
#        PRIVATE ftxui::screen
#        PRIVATE ftxui::dom
#        PRIVATE ftxui::component
)
target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

#if (EMSCRIPTEN)
#    string(APPEND CMAKE_CXX_FLAGS " -s USE_PTHREADS")
#    string(APPEND CMAKE_EXE_LINKER_FLAGS " -s ASYNCIFY")
#    string(APPEND CMAKE_EXE_LINKER_FLAGS " -s PROXY_TO_PTHREAD")
#
#    foreach(file "index.html" "run_webassembly.py")
#        configure_file("src/${file}" ${file})
#    endforeach(file)
#endif()