cmake_minimum_required(VERSION 3.0)
project(Client CXX)
set(CMAKE_CXX_STANDARD 20)

# Set the path to wxWidgets installation directory
#set(wxWidgets_ROOT_DIR /usr/include/wx-3.2)
set(wxWidgets_ROOT_DIR /usr/local/include/wx-3.0)

# Find wxWidgets
find_package(wxWidgets REQUIRED)

# Include wxWidgets macros
include(${wxWidgets_USE_FILE})
include_directories(${wxWidgets_INCLUDE_DIRS})

# Add source files to your project
set(SOURCES
        src/main.cpp

        src/controller/GameManager.cpp
        src/controller/GameManager.hpp
        src/controller/GamemodeController.cpp
        src/controller/GamemodeController.hpp
        src/controller/GameplayController.cpp
        src/controller/GameplayController.hpp
        src/controller/HomeController.cpp
        src/controller/HomeController.hpp
        src/controller/ProfileController.cpp
        src/controller/ProfileController.hpp
        src/controller/RedisManager.cpp
        src/controller/RedisManager.hpp
        src/controller/SettingsController.cpp
        src/controller/SettingsController.hpp
        src/controller/StockfishManager.cpp
        src/controller/StockfishManager.hpp

        src/model/GameOptions.cpp
        src/model/GameOptions.hpp
        src/model/User.cpp
        src/model/User.hpp
        src/model/chess_pachage/chess.hpp

        src/view/ChessboardView.cpp
        src/view/ChessboardView.hpp
        src/view/GameGraphicOptions.cpp
        src/view/GameGraphicOptions.hpp
        src/view/GameMode.cpp
        src/view/GameMode.hpp
        src/view/GameplayFrame.cpp
        src/view/GameplayFrame.hpp
        src/view/HomePage.cpp
        src/view/HomePage.hpp
        src/view/ImageFuncions.cpp
        src/view/ImageFuncions.hpp
        src/view/LoginPanel.cpp
        src/view/LoginPanel.hpp
        src/view/ProfilePanel.cpp
        src/view/ProfilePanel.hpp
        src/view/SettingsPanel.cpp
        src/view/SettingsPanel.hpp

)

# Add an executable target
add_executable(chess ${SOURCES})

# set_target_properties(chess PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

# Link against wxWidgets libraries
target_link_libraries(chess PRIVATE ${wxWidgets_LIBRARIES})
target_link_libraries(chess PRIVATE -lhiredis -lpq -lm -ggdb -fpermissive -g )
