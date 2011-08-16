add_subdirectory(masl)
add_subdirectory(mar)
if($ENV{IS_TARGET_PLATFORM_ANDROID})
	add_subdirectory(android)
endif()
add_subdirectory(animation)
add_subdirectory(spark)

