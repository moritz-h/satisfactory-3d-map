# Satisfactory Save Game - File Structure

Documentation of the Satisfactory save game file structure.

## Content

- [Introduction](#introduction)
- [General file structure](#general-file-structure)
  - [Save header](#save-header)
  - [Chunks](#chunks)
- [Decompressed binary data](#decompressed-binary-data)
- [Properties](#properties)
- [Common Types](#common-types)
  - [Basic types](#basic-types)
  - [FString](#fstring)
  - [TArray](#tarray)
  - [FObjectReferenceDisc](#fobjectreferencedisc)
  - [FGuid](#fguid)
  - [FMD5Hash](#fmd5hash)

## Introduction

[Satisfactory](https://www.satisfactorygame.com/) is a game from [Coffee Stain Studios](https://www.coffeestainstudios.com/) and based on the [Unreal Engine](https://www.unrealengine.com/).
Many data structures in the save game are directly based on the Unreal serialization system.
Satisfactory uses a few data structures specific to the game.
Most of them could be found in the C++ headers, which are distributed with in the `CommunityResources` folder in the installation directory of the game.

This documentation tries to use the original Unreal/Satisfactory type names as closely as possible.
There is a documentation of most basic types at the end of this document, see [Common Types](#common-types).

All binary data is encoded little-endian in the save.

Document Version: Satisfactory - Update 8

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
+----------+-----------------------+
| int32    | SaveHeaderVersion     |
| int32    | SaveVersion           |
| int32    | BuildVersion          |
| FString  | MapName               |
| FString  | MapOptions            |
| FString  | SessionName           |
| int32    | PlayDurationSeconds   |
| int64    | SaveDateTime          |
| int8     | SessionVisibility     |
| int32    | EditorObjectVersion   |
| FString  | ModMetadata           |
| bool     | IsModdedSave          |
| FString  | SaveIdentifier        |
| bool     | IsPartitionedWorld    |
| FMD5Hash | SaveDataHash          |
| bool     | IsCreativeModeEnabled |
+----------+-----------------------+
```

This is the save header as of Update 8.
In the past, the header was shorter, but additional values were added with updates.
Each time this struct is extended the `SaveHeaderVersion` value increases, the current value is `13`.
Internally an enum `Type` is used for this number, see `FGSaveManagerInterface.h` distributed with the game files.
The variable names are taken from the struct `FSaveHeader` in `FGSaveManagerInterface.h`.

`SaveDateTime` is the serialization of an [FDateTime object](https://docs.unrealengine.com/en-US/API/Runtime/Core/Misc/FDateTime/index.html).
Ticks since 0001-01-01 00:00, where 1 tick is 100 nanoseconds. Satisfactory seems to use the UTC time zone.

### Chunks

The whole save data is stored in a big binary structure (see below).
This binary structure is divided into chunks which then are individually compressed with zlib.
The division into chunks is done purely on size and has nothing to do with the serialized content within this binary structure.
In the save data each chunk is prefixed by a header followed by the compressed binary data.

The chunk header has the following structure:
```
+---------------+---------------------------+----------------------------------------------+
| int32         | PACKAGE_FILE_TAG          | always `0x9E2A83C1`                          |
| int32         | archive header            | 0x00000000: v1 header, 0x22222222: v2 header |
| int64         | max chunk size            |`always `131072`                              |
| if v2 header: |                           |                                              |
|     uint8     | CompressorNum             | compression algorithm, 3: zlib               |
| int64         | compressed size summary   | compressed buffer size                       |
| int64         | uncompressed size summary | uncompressed buffer size                     |
| int64         | compressed size           | compressed buffer size                       |
| int64         | uncompressed size         | uncomrpessed buffer size                     |
+---------------+---------------------------+----------------------------------------------+
```
`PACKAGE_FILE_TAG` is a constant magic number.
`archive header` was introduced with the Unreal 5 upgrade and marks if CompressorNum exists.
`max chunk size` is the maximum size used for (uncompressed) chunks.
This is a hardcoded constant from Unreal.
`CompressorNum` is the index of the used compression algorithm, currently seems to be always zlib (value = 3).
`compressed size` refers to the size of the compressed chunk data within the save file.
`uncompressed size` is the size of the chunk data after decompression with zlib.
Usually, all chunks (except the last one) use `max chunk size` as uncompressed size.
The sizes are stored twice with identical values (see below).
After decompression, all uncompressed chunk buffers are merged to a single large binary object, defined in the next section.

> Unreal internal details:  
> Unreal can [serialize an archive to a compressed structure](https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Core/Private/Serialization/Archive.cpp#L679).
> Unreal uses the struct FCompressedChunkInfo to store chunk header information. It is a pair of two int64 sizes.
> A compressed archive starts with an FCompressedChunkInfo containing the [PACKAGE_FILE_TAG](https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Core/Public/UObject/ObjectVersion.h#L9) and [COMPRESSION_CHUNK_SIZE](https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Core/Public/Misc/Compression.h#L22).
> Next is an FCompressedChunkInfo with the summary compressed and uncompressed size of all data in this archive.
> That is followed by a series of FCompressedChunkInfo with the current chunks' size and the actually compressed junk data.
> There are as many chunks until the summary size is reached.
> But instead of using a single archive for the binary blob, Satisfactory uses the [FArchiveLoadCompressedProxy](https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Core/Public/Serialization/ArchiveLoadCompressedProxy.h) to store the big binary structure.
> Here the data is not stored in a single archive, but multiple archives containing just a single chunk.
> Probably this structure has the advantage to decompress the large buffer internally step by step and not all at once.
> Nevertheless, this is probably the reason why the buffer size seems to be stored twice in the chunk header.

## Decompressed binary data

With Update 6 and again Update 8 the format changed quite a bit, here only the new format will be described.
The new format stores data for each level separately, where a level refers to a part of the game map.

The binary data layout follows the following format:
```
+---------------------------------------------+----------------------------+
| int64                                       | total size of binary data  |
|                                             | (not including this value) |
+---------------------------------------------+----------------------------+
| int32                                       | num levels                 |
+---------------------------------------------+----------------------------+
| TODO validation data ...
+---------------------------------------------+----------------------------+
|                                             | Level 1:                   |
| FString                                     | level name                 |
| TArray<uint8>                               | TOCBlob                    | in FPerLevelSaveData
| TArray<uint8>                               | DataBlob                   | in FPerLevelSaveData
| TArray<FObjectReferenceDisc>                | DestroyedActors            | in FPerLevelSaveData
+---------------------------------------------+----------------------------+
| ... (num level times)                       |                            |
|                                             |                            |
+---------------------------------------------+----------------------------+
| TArray<uint8>                               | TOCBlob                    | in FPersistentAndRuntimeSaveData
| TArray<uint8>                               | DataBlob                   | in FPersistentAndRuntimeSaveData
| TMap<FString, TArray<FObjectReferenceDisc>> | LevelToDestroyedActorsMap  | in FPersistentAndRuntimeSaveData (TODO not 100% sure, always empty ???)
| TArray<FObjectReferenceDisc>                | DestroyedActors            | in FUnresolvedWorldSaveData
+---------------------------------------------+----------------------------+
```

Structs are defined in `FGSaveSession.h`.
The level data in the beginning could be seen as serialized version of a `TMap<FString, FPerLevelSaveData>`.
The string is the name of the map tile, and its data within a `FPerLevelSaveData` struct.
The final TOCBlob and DataBlob stores everything not related to a map tile (i.e. all buildings).

#### TOCBlob

```
+-------+------------------------------------------------------+
| int32 | world object count                                   |
| ...   | objects                                              |
| int32 | destroyed actors count                               |
| ...   | destroyed actors                                     |
+-------+------------------------------------------------------+
```

Objects are either an actor or a basic object.
Each object has a class name in the form `/Game/FactoryGame/Buildable/Building/Foundation/Build_Foundation_8x4_01.Build_Foundation_8x4_01_C` and an additional instance name.
This allows interpreting a save file in a hierarchical way, similar to a file system.
But for now, this should just provide a general idea about interpreting a save file.
Parsing objects from the save file is more straightforward as this is basically just a list of all objects.

DestroyedActors is just a list of object references.
This is basically a list of references of destroyed actors.
To save space within the save game, i.e. not all foliage objects are stored, but only the ones which are destroyed by the player.
And because there is no need to store any properties, this is basically just a list of object names.

The size of each object/property/destroyed object varies, therefore, the data must be parsed sequentially.
Different objects are just written one by one after each other.
The following sections will explain how to parse each object.
The numbers of world objects and the number of property sections must be the same.
There is a 1:1 mapping of a property section to the world object (first properties are for the first object, ...).

#### DataBlob
```
+-------+------------------------------------------------------+
| int32 | world object data count                              |
| ...   | object properties                                    |
+-------+------------------------------------------------------+
```

Properties are attached to each object and can be of any type and number.
Properties will probably be the most laborious part of parsing the save game data.

#### DestroyedActors

This list seems identical to the DestroyedActors list within TOCData.
It is not known why the list is duplicated in the save, yet.

### Objects

Each object starts with a common header:
```
+----------------------+------------+
| int32                | type       |
| FString              | class name |
| FObjectReferenceDisc | reference  |
+----------------------+------------+
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
| int32    | need_transform      |                                           |
| float[4] | rotation            | quaternion describing object rotation     |
| float[3] | position            | world position of object (in centi meter) |
| float[3] | scale               | object scale                              |
| int32    | was placed in level |                                           |
+----------+---------------------+-------------------------------------------+
```

(Satisfactory internal class name: `FActorSaveHeader`)

### Properties

Each property section has the following structure:
```
+--------+------------------------------+
| int32  | length                       |
| char[] | binary data of size `length` |
+--------+------------------------------+
```

The parsing of the binary properties structure is probably the most complex part of save parsing.
Therefore, the details are moved to the separate properties section below.

### Destroyed actors

The destroyed actors are an array of `FObjectReferenceDisc` with `destroyed actors count` number of entries.

```
+------------------------+------------------+
| FObjectReferenceDisc[] | destroyed actors |
+------------------------+------------------+
```

## Properties

Each property data section itself contains up to three different sections, depending on the type of object it is attached to.

### Parent/Child information

If the object type is an actor, there is information about parent and child objects within the property data.
The structure is as follows:
```
+------------------------+------------------+
| FObjectReferenceDisc   | parent reference |
| int32                  | children count   |
| FObjectReferenceDisc[] | child references |
+------------------------+------------------+
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
|     int32                        | Size            |
|     int32                        | ArrayIndex      |
|     if Type == "StructProperty": |                 |
|         FString                  | StructName      |
|         GUID                     | StructGuid      |
|     if Type == "BoolProperty":   |                 |
|         int8                     | BoolVal         |
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
|     uint8                        | HasPropertyGuid |
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

## Common Types

### Basic types

Integer types are all named by their size in bits, e.g. int8, int32, int64, uint8, uint32, uint64, etc.
Floating point numbers are either defined as float (32 bit) or double (64 bit).
Bools stored as int32 in the save game.

### FString

`FString` is an Unreal class for strings.
It has the following binary structure:
```
+--------+---------------+
| int32  | string length |
| char[] | data          |
+--------+---------------+

length = 0: data is empty.
length > 0: data is a `char` array with the size `length`, representing a null-terminated ASCII string.
length < 0: data is a `char16` array with the size `-length`, representing a null-terminated UTF-16 string.
```

- [Reference to serialization source code](https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Core/Private/Containers/String.cpp#L1367-L1495)
- [Reference to Unreal Documentation](https://docs.unrealengine.com/en-US/ProgrammingAndScripting/ProgrammingWithCPP/UnrealArchitecture/StringHandling/CharacterEncoding/index.html)

### TArray

TArray<T> is defined in the following way:
```
+--------+-------+
| int32  | num   |
| T[num] | items |
+--------+-------+
```

TArray is a template and the size of a single item is defined by the underlying type.

### FObjectReferenceDisc

Another common type used within the save data is an `FObjectReferenceDisc`, defined in the following way:
```
+---------+------------+
| FString | level name |
| FString | path name  |
+---------+------------+
```

- Satisfactory internal struct, header file: `FGObjectReference.h`

### FGuid

```
+--------+---+
| uint32 | A |
| uint32 | B |
| uint32 | C |
| uint32 | D |
+--------+---+
```

### FMD5Hash

```
+---------------+----------+
| FString       | bIsValid |
| if bIsValid:  |          |
|     uint8[16] | Bytes    |
+---------------+----------+
```
