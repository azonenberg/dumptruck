add_executable(main
	#DemoCLISessionContext.cpp
	#DemoTCPProtocol.cpp
	#DemoUDPProtocol.cpp
	globals.cpp
	Init.cpp
	vectors.cpp)

target_link_options(main
	PUBLIC "-ffunction-sections"
	PUBLIC "-Wl,--gc-sections"
	PUBLIC "-Wl,-T${CMAKE_CURRENT_SOURCE_DIR}/firmware.ld"
	PUBLIC "-L${PROJECT_ROOT}/stm32-cpp/devices/link/"
	PUBLIC "-L${CMAKE_CURRENT_SOURCE_DIR}/..")

target_link_libraries(main
	bsp
	common-embedded-platform-core
	common-embedded-platform-fpga
	common-embedded-platform-services
	embedded-cli
	embedded-utils
	microkvs
	staticnet
	stm32-cpp
	)

# Calculate flash usage and generate binary file
common_postbuild(main)
