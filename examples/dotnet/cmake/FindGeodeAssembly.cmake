set(GEODE_ASSEMBLY_DEBUG_IN_BUILT_SOURCE_TREE "${CMAKE_CURRENT_SOURCE_DIR}/../../build/clicache/src/Debug/Apache.Geode.dll")
set(GEODE_ASSEMBLY_IN_BUILT_SOURCE_TREE "${CMAKE_CURRENT_SOURCE_DIR}/../../build/clicache/src/Release/Apache.Geode.dll")
set(GEODE_ASSEMBLY_IN_INSTALL_TREE "${CMAKE_CURRENT_SOURCE_DIR}/../../bin/Apache.Geode.dll")


if (EXISTS ${GEODE_ASSEMBLY_IN_INSTALL_TREE})
	set(GEODE_ASSEMBLY ${GEODE_ASSEMBLY_IN_INSTALL_TREE})
elseif (EXISTS ${GEODE_ASSEMBLY_DEBUG_IN_BUILT_SOURCE_TREE})
	set(GEODE_ASSEMBLY ${GEODE_ASSEMBLY_DEBUG_IN_BUILT_SOURCE_TREE})
elseif (EXISTS ${GEODE_ASSEMBLY_IN_BUILT_SOURCE_TREE})
	set(GEODE_ASSEMBLY ${GEODE_ASSEMBLY_IN_BUILT_SOURCE_TREE})
else()
	set(GEODE_ASSEMBLY "GEODE_ASSEMBLY-NOT_FOUND")
endif()

message(STATUS "Geode assembly located at ${GEODE_ASSEMBLY}")