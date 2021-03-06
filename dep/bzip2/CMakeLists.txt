# MIT License

# Copyright (c) 2018-2019 NovusCore

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

project(bzip2 VERSION 1.0.0)

if (UNIX OR APPLE)
    find_package(BZip2 REQUIRED)

    add_library(${PROJECT_NAME} INTERFACE)
    add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

    target_include_directories(${PROJECT_NAME} SYSTEM INTERFACE "${BZIP2_INCLUDE_DIRS}")
    target_link_libraries(${PROJECT_NAME} INTERFACE "${BZIP2_LIBRARIES}")
else()
    file(GLOB sources *.c)
    add_library(${PROJECT_NAME} STATIC ${sources})
    add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

    target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "${ROOT_FOLDER}/dep")
endif()
