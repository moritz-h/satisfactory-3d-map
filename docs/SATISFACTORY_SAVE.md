# Satisfactory Save Game - File Structure

## Info

Documentation of the Satisfactory save game file structure.

[Satisfactory](https://www.satisfactorygame.com/) is a game from [Coffee Stain Studios](https://www.coffeestainstudios.com/) and based on the [Unreal Engine](https://www.unrealengine.com/).
Many structures in the save game are based on the Unreal serialization system.
Therefore, at many points references to the Unreal source code are made for further reading, but this should not be required to understand the basic save file structure.
In addition, Satisfactory uses individual structures only specific for this game.
Some of these structures can be found in the C++ headers distributed with the game, and are referenced here if possible.

All binary data is encoded little-endian in the save.

Document Version: Satisfactory - Update 5

## Common Types

### FString

Strings are very commonly used within the save game.
Strings are based on the Unreal class `FString`, which will be used as a type name in this document.
It has the following binary structure:
```
+---------+---------------+
| int32_t | string length |
| char[]  | data          |
+---------+---------------+

length = 0: data is empty.
length > 0: data is a `char` array with the size `length`, representing a null-terminated ASCII string.
length < 0: data is a `char16_t` array with the size `-length`, representing a null-terminated UTF-16 string.
```

- [Reference to serialization source code](https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Core/Private/Containers/String.cpp#L1367-L1495)
- [Reference to Unreal Documentation](https://docs.unrealengine.com/en-US/ProgrammingAndScripting/ProgrammingWithCPP/UnrealArchitecture/StringHandling/CharacterEncoding/index.html)

### Object reference

Another common type used within the save data is an `ObjectReference`, defined in the following way:
```
+---------+------------+
| FString | level name |
| FString | path name  |
+---------+------------+
```

- Satisfactory internal class name: `FObjectReferenceDisc`
- Header file: `FGObjectReference.h`

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

The save header has the following structure:
```
+---------+---------------------+
| int32_t | SaveHeaderVersion   |
| int32_t | SaveVersion         |
| int32_t | BuildVersion        |
| FString | MapName             |
| FString | MapOptions          |
| FString | SessionName         |
| int32_t | PlayDurationSeconds |
| int64_t | SaveDateTime        |
| int8_t  | SessionVisibility   |
| int32_t | EditorObjectVersion |
| FString | ModMetadata         |
| int32_t | IsModdedSave        |
+---------+---------------------+
```

This is the save header as of Update 5.
In the past, the header was shorter, but additional values were added with updates.
Each time this struct is extended the `SaveHeaderVersion` value increases, the current value is `9`.
Internally an enum `Type` is used for this number, see `FGSaveManagerInterface.h`.
The variable names are taken from the file `FGSaveSystem.h` distributed with the game files.

`SaveDateTime` is the serialization of an [FDateTime object](https://docs.unrealengine.com/en-US/API/Runtime/Core/Misc/FDateTime/index.html).
Ticks since 0001-01-01 00:00, where 1 tick is 100 nanoseconds. Satisfactory seems to use the UTC time zone.

### Chunks

The whole save data is stored in a big binary structure (see below).
This binary structure is divided into chunks which then are individually compressed with zlib.
The division into chunks is done purely on size and has nothing to do with the serialized content within this binary structure.
In the save data each chunk is prefixed by a header followed by the compressed binary data.

The chunk header has the following structure:
```
+---------+---------------------------+-----------------------------+
| int64_t | package file tag          | always `0x9E2A83C1`         |
| int64_t | max chunk size            |`always `131072`             |
| int64_t | compressed size summary   | compressed buffer size      |
| int64_t | uncompressed size summary | uncompressed buffer size    |
| int64_t | compressed size           | compressed buffer size      |
| int64_t | uncompressed size         | uncomrpessed buffer size    |
+---------+---------------------------+-----------------------------+
```
The `package file tag` is a constant magic number.
`max chunk size` is the maximum size used for (uncompressed) chunks.
This is a hardcoded constant from Unreal.
The `compressed size` refers to the size of the compressed chunk data within the save file.
The `uncompressed size` is the size of the chunk data after decompression with zlib.
Usually, all chunks (except the last one) use `max chunk size` as uncompressed size.
The sizes are stored twice with identical values (see below).
After decompression, all uncompressed chunk buffers are merged to a single large binary object, defined in the next section.

> Unreal internal details:  
> Unreal can [serialize an archive to a compressed structure](https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Core/Private/Serialization/Archive.cpp#L679).
> Unreal uses the struct FCompressedChunkInfo to store chunk header information. It is a pair of two int64_t sizes.
> A compressed archive starts with an FCompressedChunkInfo containing the [PACKAGE_FILE_TAG](https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Core/Public/UObject/ObjectVersion.h#L9) and [COMPRESSION_CHUNK_SIZE](https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Core/Public/Misc/Compression.h#L22).
> Next is an FCompressedChunkInfo with the summary compressed and uncompressed size of all data in this archive.
> That is followed by a series of FCompressedChunkInfo with the current chunks' size and the actually compressed junk data.
> There are as many chunks until the summary size is reached.
> But instead of using a single archive for the binary blob, Satisfactory uses the [FArchiveLoadCompressedProxy](https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Core/Public/Serialization/ArchiveLoadCompressedProxy.h) to store the big binary structure.
> Here the data is not stored in a single archive, but multiple archives containing just a single chunk.
> Probably this structure has the advantage to decompress the large buffer internally step by step and not all at once.
> Nevertheless, this is the explanation why in a Satisfactory save game we observe the chunk header described above, where the size seems to be stored twice.

## Decompressed binary data

The save game basically stores three different types of data.
The first ones are objects, either an actor or a basic object.
Each object has a class name in the form `/Game/FactoryGame/Buildable/Building/Foundation/Build_Foundation_8x4_01.Build_Foundation_8x4_01_C` and an additional instance name.
This allows interpreting a save file in a hierarchical way, similar to a file system.
But for now, this should just provide a general idea about interpreting a save file.
Parsing objects from the save file is more straightforward as this is basically just a list of all objects.

The second part within the save data are object properties.
Properties are attached to each object and can be of any type and number.
Properties will probably be the most laborious part of parsing the save game data.

Finally, the third type of data is just a list of object references.
This is basically a list of references of destroyed actors.
To save space within the save game, i.e. not all foliage objects are stored, but only the ones which are destroyed by the player.
And because there is no need to store any properties, this is basically just a list of object names.

Now, looking at the binary data, these three types are stored within the following structure:
```
+---------+------------------------------------------------------+
| int32_t | total size of binary data (not including this value) |
| int32_t | world object count                                   |
| ...     | objects                                              |
| int32_t | world object data count                              |
| ...     | object properties                                    |
| int32_t | destroyed actors count                               |
| ...     | destroyed actors                                     |
+---------+------------------------------------------------------+
```

The size of each object/property/destroyed object varies, therefore, the data must be parsed sequentially.
Different objects are just written one by one after each other. The following sections will explain how to parse each object.
The numbers of world objects and the number of property sections must be the same.
There is a 1:1 mapping of a property section to the world object (first properties are for the first object, ...).

### Objects

Each object starts with a common header:
```
+-----------------+------------+
| int32_t         | type       |
| FString         | class name |
| ObjectReference | reference  |
+-----------------+------------+
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
+---------+-----------------+
| FString | outer path name |
+---------+-----------------+
```

(Satisfactory internal class name: `FObjectSaveHeader`)

#### Actor

An actor has these additional fields:
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

### Destroyed actors

The destroyed actors are an array of `ObjectReference` with `destroyed actors count` number of entries.

```
+-------------------+------------------+
| ObjectReference[] | destroyed actors |
+-------------------+------------------+
```

## Properties

Each property data section itself contains up to three different sections, depending on the type of object it is attached to.

### Parent/Child information

If the object type is an actor, there is information about parent and child objects within the property data.
The structure is as follows:
```
+-------------------+------------------+
| ObjectReference   | parent reference |
| int32_t           | children count   |
| ObjectReference[] | child references |
+-------------------+------------------+
```
The array `child references` has `children count` many entries. The size may be zero.

### List of properties

Properties are stored one by one in a list, where the end of a list is always defined by a property with the name "None".
All Properties have a common header named PropertyTag and data which is different for each property type.
```
+---------------+-----------------+
| Property tag  | Property 1      |
|---------------|                 |
| Property data |                 |
+---------------+-----------------+
| Property tag  | Property 2      |
|---------------|                 |
| Property data |                 |
+---------------+-----------------+
| ...                             |
+---------------+-----------------+
| PropertyTag   | Property "None" |
+---------------+-----------------+
```

The common header (a struct named `PropertyTag`, see
[PropertyTag.h](https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Public/UObject/PropertyTag.h#L21-L33)
[PropertyTag.cpp](https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Private/UObject/PropertyTag.cpp#L81-L205))
has the following format:
```
+----------------------------------+-----------------+
| FString                          | Name            |
| if Name != "None":               |                 |
|     FString                      | Type            |
|     int32_t                      | Size            |
|     int32_t                      | ArrayIndex      |
|     if Type == "StructProperty": |                 |
|         FString                  | StructName      |
|         GUID                     | StructGuid      |
|     if Type == "BoolProperty":   |                 |
|         int8_t                   | BoolVal         |
|     if Type == "ByteProperty":   |                 |
|         FString                  | EnumName        |
|     if Type == "EnumProperty":   |                 |
|         FString                  | EnumName        |
|     if Type == "ArrayProperty":  |                 |
|         FString                  | InnerType       |
|     if Type == "SetProperty":    |                 |
|         FString                  | InnerType       |
|     if Type == "MapProperty":    |                 |
|         FString                  | InnerType       |
|         FString                  | ValueType       |
|     uint8_t                      | HasPropertyGuid |
|     if HasPropertyGuid:          |                 |
|         GUID                     | PropertyGuid    |
+----------------------------------+-----------------+
```

#### Property data

TODO

### Property extra binary data

Some classes have extra binary data after the property list. The structure is individual and determined by the class itself.
The size of the extra binary data block must be determined from the whole property section minus the previous blocks.

TODO
