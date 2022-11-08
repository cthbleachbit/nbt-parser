file(GLOB TEST_DATA *.nbt)

foreach(NBT ${TEST_DATA})
configure_file(${NBT} ${NBT} COPYONLY)
endforeach()