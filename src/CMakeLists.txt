set(SOURCE_FILES
        pipeapp.h
        main.c)

set(LIB_SOURCE_FILES
        pipeapp.h
        lib.c)

add_library(gstpipeapp SHARED ${LIB_SOURCE_FILES})
target_link_libraries(gstpipeapp
        ${GLIB2_LIBRARIES} ${GSTRM_LIBRARIES} ${GSTRMAPP_LIBRARIES})

add_executable(gst-pipeapp ${SOURCE_FILES})
target_link_libraries(gst-pipeapp gstpipeapp
        ${GLIB2_LIBRARIES})