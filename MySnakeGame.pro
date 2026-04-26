# Qt Project Configuration File For Snake Game Application
# Specifies Modules, Compiler Options, Source Files, And Headers

# Core Qt Modules Required For The Application
QT       += core gui widgets

# C++ Language Standard Configuration
# Enables C++17 Features And Syntax
CONFIG += c++17

# Source Files To Be Compiled
# Contains Main Entry Point And Implementation Files
SOURCES += \
    main.cpp \
    snake.cpp \
    home.cpp

# Header Files Containing Class Declarations
# Defines The Interfaces For Snake And Home Classes
HEADERS += \
    snake.h \
    home.h

# UI Form Files (Currently Empty)
# Reserved For Future GUI Designer Integration
FORMS +=
