FIND_PACKAGE(ACE REQUIRED)
#SET(OpenSceneGraph_DEBUG TRUE)
FIND_PACKAGE(OpenSceneGraph REQUIRED osgUtil osgText osgViewer osgGA)
FIND_PACKAGE(Qt COMPONENTS Gui Core REQUIRED)

################# Qt Ui related ###############################################
include(${QT_USE_FILE})
SET(gui_UI_FILES
    ui/mainwindow.ui
    ui/objecttreewidget.ui
)
SET(gui_MOC_HEADERS
    include/mainwindow.h
    include/objecttreewidget.h
    include/mapmodel.h
    include/CompositeViewerQOSG.h
    include/QOSGWidget.h
)
SET(gui_SOURCES
    src/mainwindow.cpp
    src/objecttreewidget.cpp
    src/mapmodel.cpp
    src/CompositeViewerQOSG.cpp
    src/QOSGWidget.cpp
    ${gui_MOC_HEADERS}
)
QT4_WRAP_UI(gui_UI_H ${gui_UI_FILES})
QT4_WRAP_CPP(gui_MOC_SRC ${gui_MOC_HEADERS})
qt4_automoc(${gui_SOURCES})
################################################################################

INCLUDE_DIRECTORIES(${OPENSCENEGRAPH_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}) # to access moc compiled headers

SET(target_CPP
${CMAKE_CURRENT_SOURCE_DIR}/src/LoadMap.cpp
)
SET(target_INCLUDE
)
SET(target_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
SET(target_DEPENDS "")

SEGS_ADD_COMPONENT(static_storage LIB)

INCLUDE_DIRECTORIES(${target_INCLUDE_DIR} ${ACE_path})

SET (target_SOURCES
${gui_SOURCES}
${target_CPP}
${target_INCLUDE}
${gui_MOC_SRC} ${gui_UI_H}
)




ADD_EXECUTABLE(mapviewer ${target_SOURCES})

ACE_ADD_LIBRARIES(mapviewer ACE)
TARGET_LINK_LIBRARIES(mapviewer   ${target_DEPENDS}
    ${CMAKE_THREAD_LIBS_INIT}
    ${OPENSCENEGRAPH_LIBRARIES}
    ${QT_LIBRARIES}
)

ADD_DEPENDENCIES(mapviewer ${target_DEPENDS})

