cmake_minimum_required(VERSION 3.16)

project(eosquickstart VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_directory_properties(PROPERTIES CLEAN_NO_CUSTOM 1)

find_package(Qt6 6.2 COMPONENTS Quick REQUIRED)

configure_file(config.h.in config.h @ONLY)

qt_add_executable(eos-quickstart
    main.cpp packagemodel.h packagemodel.cpp packagemanager.h packagemanager.cpp
)

qt_add_qml_module(eos-quickstart
    URI eosquickstart
    VERSION 1.0
    QML_FILES main.qml 
)

install(FILES eos-quickstart.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications/)
install(TARGETS eos-quickstart RUNTIME DESTINATION bin)

target_compile_definitions(eos-quickstart
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(eos-quickstart
    PRIVATE Qt6::Quick)
