project( modeltest )

set(modeltest_SOURCES
    tst_modeltest.cpp modeltest.cpp dynamictreemodel.cpp
 )

set( modeltest_HEADERS
    modeltest.h dynamictreemodel.h
 )

qt4_wrap_cpp( modeltest_SOURCES ${modeltest_HEADERS} )
add_library( modeltest ${modeltest_SOURCES} )
target_link_libraries(modeltest ${QT_LIBRARIES} )

if( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
  set_target_properties(modeltest PROPERTIES COMPILE_FLAGS "-fPIC")
endif( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
