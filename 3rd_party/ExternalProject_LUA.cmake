set(lua_RELEASE 5.3.4)
libname(lua lua)
ExternalProject_Add(
    lua-${lua_RELEASE}
    URL http://www.lua.org/ftp/lua-${lua_RELEASE}.tar.gz
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/lua/CMakeLists.txt" <SOURCE_DIR>/CMakeLists.txt
    INSTALL_DIR ${ThirdParty_Install_Dir}
    CMAKE_ARGS -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} -DCMAKE_STAGING_PREFIX:PATH=${ThirdParty_Install_Dir} -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    CMAKE_GENERATOR "${CMAKE_GENERATOR}"
    CMAKE_GENERATOR_TOOLSET ${CMAKE_GENERATOR_TOOLSET}
    BUILD_BYPRODUCTS ${lua_LIBRARY_STATIC}
)

ADD_LIBRARY(lua_IMP STATIC IMPORTED GLOBAL)
add_dependencies(lua_IMP lua-${lua_RELEASE})
set_property(TARGET lua_IMP APPEND PROPERTY IMPORTED_LOCATION ${lua_LIBRARY_STATIC} )
target_include_directories(lua_IMP INTERFACE ${ThirdParty_Install_Dir}/include)
#set_shared_lib_properties(ace)
