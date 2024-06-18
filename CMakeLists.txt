cmake_minimum_required(VERSION 3.0)
project(MyProject CXX)
#Setting the standard c++ we are using
set(CMAKE_CXX_STANDARD 17)


# Set the path to wxWidgets installation directory
set(wxWidgets_ROOT_DIR /usr/include/wx-3.2)
#set(wxWidgets_ROOT_DIR /usr/include/wx-3.0)

# Find wxWidgets
find_package(wxWidgets REQUIRED)

# Include wxWidgets macros
include(${wxWidgets_USE_FILE})
include_directories(${wxWidgets_INCLUDE_DIRS})

# Add source files to your project
set(SOURCES
        client/src/main.cpp
        client/src/controller/GameManager.cpp
        client/src/controller/GameManager.hpp
        client/src/controller/GamemodeController.cpp
        client/src/controller/GamemodeController.hpp
        client/src/controller/GameplayController.cpp
        client/src/controller/GameplayController.hpp
        client/src/controller/HomeController.cpp
        client/src/controller/HomeController.hpp
        client/src/controller/PanelSwitchManager.hpp
        client/src/controller/ProfileController.cpp
        client/src/controller/ProfileController.hpp
        client/src/controller/RedisManager.cpp
        client/src/controller/RedisManager.hpp
        client/src/controller/SettingsController.cpp
        client/src/controller/SettingsController.hpp
        client/src/controller/StockfishManager.cpp
        client/src/controller/StockfishManager.hpp

        client/src/model/GameOptions.cpp
        client/src/model/GameOptions.hpp
        client/src/model/User.cpp
        client/src/model/User.hpp

        client/src/model/chess_pachage/chess.hpp

        client/src/view/ChessboardView.cpp
        client/src/view/ChessboardView.hpp
        client/src/view/GameGraphicOptions.cpp
        client/src/view/GameGraphicOptions.hpp
        client/src/view/GameMode.cpp
        client/src/view/GameMode.hpp
        client/src/view/GameplayFrame.cpp
        client/src/view/GameplayFrame.hpp
        client/src/view/HomePage.cpp
        client/src/view/HomePage.hpp
        client/src/view/ImageFuncions.cpp
        client/src/view/ImageFuncions.hpp
        client/src/view/LoginPanel.cpp
        client/src/view/LoginPanel.hpp
        client/src/view/ProfilePanel.cpp
        client/src/view/ProfilePanel.hpp
        client/src/view/SettingsPanel.cpp
        client/src/view/SettingsPanel.hpp

)
#*******************************************
#[[# Add the Stockfish source files
file(GLOB STOCKFISH_SOURCES
        "src/stockfish/*.cpp"
        "include/src/stockfish/*.h"
)
# Include directories
include_directories(include/stockfish)
# Add the executable
add_executable(MyProject main.cpp ${STOCKFISH_SOURCES})]]
#*******************************************


# Add an executable target
add_executable(chess ${SOURCES})

# Link against wxWidgets libraries
target_link_libraries(chess ${wxWidgets_LIBRARIES})
