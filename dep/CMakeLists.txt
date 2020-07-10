# MIT License

# Copyright (c) 2020 NovusCore

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

if(NOT TARGET angelscript)
    add_subdirectory(angelscript)
endif()

if(NOT TARGET asio)
    add_subdirectory(asio)
endif()

if(NOT TARGET mysql)
    add_subdirectory(mysql)
endif()

if(NOT TARGET openssl)
    add_subdirectory(openssl)
endif()

if(NOT TARGET tracy)
    add_subdirectory(TracyProfiler)
endif()

if(NOT TARGET Entt)
    add_subdirectory(Entt)
endif()

if(NOT TARGET taskflow)
    add_subdirectory(taskflow)
endif()

if(NOT TARGET type_safe)
    add_subdirectory(type_safe)
endif()

if(NOT TARGET bzip2)
    add_subdirectory(bzip2)
endif()

if(NOT TARGET zlib)
    add_subdirectory(zlib)
endif()

if(NOT TARGET stormlib)
    add_subdirectory(stormlib)
endif()

if(NOT TARGET shadercooker)
    add_subdirectory(dxcompiler)
endif()