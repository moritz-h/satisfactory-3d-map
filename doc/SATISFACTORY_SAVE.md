# Satisfactory Save Game Structure

## Info

Documentation of the Satisfactory save file structure.

Version: Satisfactory - Update 4

## Common Types

### String

The save game stores a lot of strings.
In the following document the type `length prefixed string` refers to the following binary structure:
```
+---------+---------------+
| int32_t | string length |
| char[]  | data          |
+---------+---------------+

length = 0: data is empty.
length > 0: data is a `char` array with the size `length`, representing a null terminated ASCII string.
length < 0: data is a `char16_t` array with the size `-length`, representing a null terminated UTF-16 string.
```
[Reference to Unreal Documentation](https://docs.unrealengine.com/en-US/ProgrammingAndScripting/ProgrammingWithCPP/UnrealArchitecture/StringHandling/CharacterEncoding/index.html)

### Object reference

The second common type used within the data is an `object reference`, defined in the following way:
```
+------------------------+------------+
| length prefixed string | level name |
| length prefixed string | path name  |
+------------------------+------------+
```

(Satisfactory internal class name: `FObjectReferenceDisc`, see `FGObjectReference.h`.)

## General file structure

The save file starts with a save header, followed by chunks of zlib compressed data.
Each chunk starts with a chunk header followed by the binary chunk data.
```
+--------------+
| Save Header  |
+--------------+
| Chunk Header |
+--------------+
| Chunk Data   |
+--------------+
| Chunk Header |
+--------------+
| Chunk Data   |
+--------------+
| ...          |
+--------------+
```

### Save header

The header has the following structure:
```
+------------------------+-----------------------+
| int32_t                | save header version   | Enum refering to the structure of this header (see FGSaveSystem.h)
| int32_t                | save version          |
| int32_t                | build version         | build number of the game
| length prefixed string | map name              |
| length prefixed string | map options           |
| length prefixed string | session name          |
| int32_t                | play duration seconds |
| int64_t                | save date time        |
| int8_t                 | session visibility    |
| int32_t                | editor object version |
| length prefixed string | mod metadata          |
| int32_t                | is modded save        |
+------------------------+-----------------------+
```

This is the save header as of Update 4.
In the past the header was shorter, but additional values were added with updates.
Each time this struct is extended the `save header version` value increases, current value is `8`.
The variable names are taken from the file `FGSaveSystem.h` distributed with the game files.

`save date time` is the serialisation of a [FDateTime object](https://docs.unrealengine.com/en-US/API/Runtime/Core/Misc/FDateTime/index.html).
Ticks since 0001-01-01 00:00, where 1 tick is 100 nano seconds. Satisfactory seems to use UTC time zone.

### Chunks

The whole save data is stored in a big binary structure (see below).
This binary structure is divided into chunks which then are individually compressed with zlib.
In the save data each chunk is prefixed by a header followed by the compressed binary data.

The chunk header has the following structure:
```
+----------+-----------------------+-----------------------------+
| uint64_t | package file tag      | always `2653586369`         |
| uint64_t | max chunk size        |`always `131072`             |
| uint64_t | compressed length 1   | size of compressed buffer   |
| uint64_t | decompressed length 1 | size of uncompressed buffer |
| uint64_t | compressed length 2   | same as length 1 (unused?)  |
| uint64_t | decompressed length 2 | same as length 1 (unused?)  |
+----------+-----------------------+-----------------------------+
```
The `package file tag` seems to be a constant magic number.
`max chunk size` is the maximum size used for (uncompressed) chunks.
The `compressed length` refers to the size of the binary data within the save file.
The `decompressed length` is the size of the chunk data after decompression with zlib.
Usually all chunks (except the last one) use `max chunk size` as decompressed length.
The lengths are stored twice with identical values.
Maybe there is something like a sub-chunks feature possible within Unreal, but it seems unused by Satisfactory.

After decompression all chunk buffers are merged together to a single large binary object, defined in the next section.

## Decompressed binary data

The save game basically stores three different types of data.
The first ones are objects, wither an actor or a basic object.
Each object has a class name in the form `/Game/FactoryGame/Buildable/Building/Foundation/Build_Foundation_8x4_01.Build_Foundation_8x4_01_C` and an addition instance name.
This allows to interpret a save file in a hierarchical way, similar to a file system.
But for now this should just provide a general idea about interpreting a save file.
Parsing objects from the save file is more straight forward as this is basically just a list of all objects.

The second part within the save data are object properties.
Properties are attached to each object and can be of any type and number.
Properties will probably be the most laborious part of parsing the save game data.

Finally the third type of data is just a list of object references.
This is basically a list of references of collected objects.
To save space within the save game, i.e. not all foliage objects are stored, but only the one which are collected by the player.
And because there is no need to store any properties, this is basically just a list of object names.

Now, looking at the binary data, these three types are stored within the following structure:
```
+---------+------------------------------------------------------+
| int32_t | total size of binary data (not including this value) |
| int32_t | world object count                                   |
| ...     | objects                                              |
| int32_t | world object data count                              |
| ...     | object properties                                    |
| int32_t | collected objects count                              |
| ...     | collected objects                                    |
+---------+------------------------------------------------------+
```

The size of each object/property/collected object varies, therefore, the data must be parsed sequentially.
Different objects are just written one by one after each other. The following sections will explain how to parse each object.
The numbers of world objects and the number of property sections must be the same.
There is a 1:1 mapping of a property section to the world object (first properties are for the first object, ...).

### Objects

Each object starts with a common header:
```
+------------------------+------------+
| int32_t                | type       |
| length prefixed string | class name |
| object reference       | reference  |
+------------------------+------------+
```

(Satisfactory internal class name: `FObjectBaseSaveHeader`)

There are two types:
```
0 = object
1 = actor
```

#### Object

An object has only one additional field:
```
+------------------------+-----------------+
| length prefixed string | outer path name |
+------------------------+-----------------+
```

(Satisfactory internal class name: `FObjectSaveHeader`)

#### Actor

An actor has these addition fields:
```
+----------+---------------------+-------------------------------------------+
| int32_t  | need_transform      |                                           |
| float[4] | rotation            | quaternion describing object rotation     |
| float[3] | position            | world position of object (in centi meter) |
| float[3] | scale               | object scale                              |
| int32_t  | was placed in level |                                           |
+----------+---------------------+-------------------------------------------+
```

(Satisfactory internal class name: `FActorSaveHeader`)

### Properties

Each property section has the following structure:
```
+---------+------------------------------+
| int32_t | length                       |
| char[]  | binary data of size `length` |
+---------+------------------------------+
```

The parsing of the binary properties structure is probably the most complex part of save parsing.
Therefore, the details are moved to the separate properties section below.

### Collected objects

The collected objects are simpy an array of `object referece` with `collected object count` number of entries.

```
+--------------------+-------------------+
| object reference[] | collected objects |
+--------------------+-------------------+
```

## Properties

TODO!
