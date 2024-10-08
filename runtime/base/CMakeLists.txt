
sge_add_shared_library(base
	OUTPUT_NAME
		sge-base
	PUBLIC_INCLUDE_DIRECTORIES
		${CMAKE_CURRENT_SOURCE_DIR}/include
		${CMAKE_CURRENT_BINARY_DIR}/include
	PRIVATE_SOURCES
		include/HandmadeMath.h
		include/sge/config.h.in
		include/sge/common.h
		include/sge/list.h
		include/sge/log.h
		src/sge/list.c
		src/sge/log.c
	PUBLIC_HEADERS
		include/sge/common.h
		include/sge/list.h
		include/sge/log.h
		${CMAKE_CURRENT_BINARY_DIR}/include/sge/config.h
		DESTINATION include/sge
	PUBLIC_LINK_PACKAGES
		libuv uv
	INSTALL_FILES
		include/HandemadeMath.h
		DESTINATION include
)
