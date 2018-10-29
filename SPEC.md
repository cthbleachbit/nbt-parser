Minecraft NBT Specification
===========================

Reference: [Minecraft Gamepedia](https://minecraft.gamepedia.com/NBT_format)

Tag Types
---------

* Byte
* Short (signed 2 bytes __Big Endian__)
* Int (signed 4 bytes __Big Endian__)
* Long (signed 8 bytes __Big Endian__)
* Float (signed 4 bytes IEEE 754-2008 __Big Endian__)
* Double (signed 4 bytes IEEE 754-2008 __Big Endian__)
* ByteArray (variable size of bytes)
* String (string w/o 0-terminator)
* List (list of tags of same type)
* Compound (container for other tags)
* IntArray (variable size of 4 bytes)
* LongArray (variable size of 8 bytes)

Context Free Grammar
--------------------
* Name -> `short_lit` `string`
* Compound -> `0a` Name InnerCompound `00`
* InnerCompound -> Tag InnerCompound | `epsilon`
* IntPayloadSize -> `int_lit`

* TypeId -> `01` | `02` | `03` | `04` | `05` | `06` | `07` | `08` | `09` | `0a` | `0b` | `0c`
* Tag -> Byte | Short | Int | Long | Float | Double | ByteArray | String | List | Compound | IntArray | LongArray
* Byte -> `01` Name `byte_lit`
* Short -> `02` Name `short_lit`
* Int -> `03` Name `int_lit`
* Long -> `04` Name `long_lit`
* Float -> `05` Name `float_lit`
* Double -> `06` Name `double_lit`
* ByteArray -> `07` Name IntPayloadSize ByteArrayList
* String -> `08` Name ShortPayloadSize ByteArrayList
* List -> `09` Name TypeId IntPayloadSize PayloadList
* Compound -> `0a` Name InnerCompound `00`
* IntArray -> `0b` Name IntPayloadSize IntArrayList
* LongArray -> `0c` Name IntPayloadSize LongArrayList

* ${i}ArrayList -> ${i} ${i}ArrayList | `epsilon`
