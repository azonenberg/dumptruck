add_library(bsp STATIC
	#CrossbarSSHKeyManager.cpp
	#DeviceCryptoEngine.cpp
	#ELFFirmwareUpdater.cpp
	#EthernetInit.cpp
	#FPGAFirmwareUpdater.cpp
	#ManagementDHCPClient.cpp
	#ManagementPubkeyAuthenticator.cpp

	common-vectors.cpp
	hwinit.cpp
	)

target_include_directories(bsp
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
	"$<TARGET_PROPERTY:common-embedded-platform-core,INTERFACE_INCLUDE_DIRECTORIES>"
	)
