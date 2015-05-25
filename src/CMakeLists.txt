cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(CPP_SOURCES
	filterPeptides.cpp
)


add_executable(filterPeptides ${CPP_SOURCES})

target_link_libraries(filterPeptides PugiXML)
install(TARGETS filterPeptides DESTINATION ${PROJECT_SOURCE_DIR}/bin)
