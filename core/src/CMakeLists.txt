add_subdirectory(masl)
add_subdirectory(mar)
if($ENV{TARGET_PLATFORM_ANDROID})
	add_subdirectory(android)
endif()
add_subdirectory(spark)

