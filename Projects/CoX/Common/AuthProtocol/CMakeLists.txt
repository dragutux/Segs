SET(target_CPP
${CMAKE_CURRENT_SOURCE_DIR}/AuthEventFactory.cpp
${CMAKE_CURRENT_SOURCE_DIR}/AuthPacketCodec.cpp
${CMAKE_CURRENT_SOURCE_DIR}/Events/ServerListResponse.cpp
)
SET(target_INCLUDE
${CMAKE_CURRENT_SOURCE_DIR}/AuthEvents.h
${CMAKE_CURRENT_SOURCE_DIR}/AuthEventFactory.h
${CMAKE_CURRENT_SOURCE_DIR}/AuthOpcodes.h
${CMAKE_CURRENT_SOURCE_DIR}/AuthPacketCodec.h
${CMAKE_CURRENT_SOURCE_DIR}/Events/AuthorizationProtocolVersion.h
${CMAKE_CURRENT_SOURCE_DIR}/Events/AuthorizationError.h
${CMAKE_CURRENT_SOURCE_DIR}/Events/DbError.h
${CMAKE_CURRENT_SOURCE_DIR}/Events/ServerSelectRequest.h
${CMAKE_CURRENT_SOURCE_DIR}/Events/ServerSelectResponse.h
${CMAKE_CURRENT_SOURCE_DIR}/Events/LoginResponse.h
${CMAKE_CURRENT_SOURCE_DIR}/Events/LoginRequest.h
${CMAKE_CURRENT_SOURCE_DIR}/Events/ServerListResponse.h
${CMAKE_CURRENT_SOURCE_DIR}/Events/ServerListRequest.h
)
#set(Boost_ADDITIONAL_VERSIONS "1.51" "1.51.0")
#FIND_PACKAGE(Boost REQUIRED)

SET(target_INCLUDE_DIR
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/Events"
#       ${Boost_INCLUDE_DIRS}
)
# add components here
SEGS_ADD_COMPONENT(EventSystem LIB)
# end of additional components

INCLUDE_DIRECTORIES(
${target_INCLUDE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}Events/
)

SET (target_SOURCES
${target_CPP}
${target_INCLUDE}
)

#ADD_LIBRARY(AuthProtocol STATIC ${target_SOURCES})

SEGS_REGISTER_COMPONENT(AuthProtocol "${target_INCLUDE_DIR}" "${target_CPP}" "${target_INCLUDE}" )
