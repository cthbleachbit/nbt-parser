set(TEST_DATA
		malformed.nbt
		map_101.nbt
		playerdata.nbt
		simple.nbt
		simplelist.nbt
		world.dat.nbt
		wrongsize.nbt
		)

foreach(NBT ${TEST_DATA})
configure_file(${NBT} ${NBT} COPYONLY)
endforeach()