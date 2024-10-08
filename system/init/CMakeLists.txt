
sge_add_executable(init
	OUTPUT_NAME
		sge
	PRIVATE_SOURCES
		main.c
	PUBLIC_LINK_LIBRARIES
		vm
		base
	PUBLIC_LINK_PACKAGES
		glfw3 glfw
)
