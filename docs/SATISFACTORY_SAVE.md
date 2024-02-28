# Satisfactory Save Game - File Structure

Documentation of the Satisfactory save game file structure.

## Content

- [Introduction](#introduction)
- [General file structure](#general-file-structure)
  - [Save header](#save-header)
  - [Chunks](#chunks)
- [Decompressed binary data](#decompressed-binary-data)
  - [TOCBlob](#tocblob)
  - [DataBlob](#datablob)
- [Properties](#properties)
  - [List of Properties](#list-of-properties)
  - [Simple Types](#simple-types)
    - [BoolProperty](#boolproperty)
    - [ByteProperty](#byteproperty)
    - [EnumProperty](#enumproperty)
    - [NameProperty](#nameproperty)
    - [ObjectProperty](#objectproperty)
    - [SoftObjectProperty](#softobjectproperty)
    - [StrProperty](#strproperty)
    - [TextProperty](#textproperty)
  - [Numeric Types](#numeric-types)
    - [IntProperty](#intproperty)
    - [Int8Property](#int8property)
    - [Int64Property](#int64property)
    - [UInt32Property](#uint32property)
    - [UInt64Property](#uint64property)
    - [FloatProperty](#floatproperty)
    - [DoubleProperty](#doubleproperty)
  - [Container Types](#container-types)
    - [ArrayProperty](#arrayproperty)
    - [MapProperty](#mapproperty)
    - [SetProperty](#setproperty)
    - [StructProperty](#structproperty)
- [Structs](#structs)
  - [Property Structs](#property-structs)
  - [Binary Structs](#binary-structs)
- [Class-Specific Binary Data](#class-specific-binary-data)
- [Type and Object Reference](#type-and-object-reference)
  - [Basic types](#basic-types)
  - [Containers](#containers)
    - [TArray](#tarray)
    - [TMap](#tmap)
  - [Unreal Objects](#unreal-objects)
    - [FString](#fstring)
    - [FName](#fname)
    - [FText](#ftext)
    - [FGuid](#fguid)
    - [FMD5Hash](#fmd5hash)
  - [Satisfactory Objects](#satisfactory-objects)
    - [FObjectReferenceDisc](#fobjectreferencedisc)
    - [FWorldPartitionValidationData](#fworldpartitionvalidationdata)
    - [FWPGridValidationData](#fwpgridvalidationdata)
    - [FPerStreamingLevelSaveData](#fperstreaminglevelsavedata)
    - [FPersistentAndRuntimeSaveData](#fpersistentandruntimesavedata)
    - [FObjectBaseSaveHeader](#fobjectbasesaveheader)
    - [FObjectSaveHeader](#fobjectsaveheader)
    - [FActorSaveHeader](#factorsaveheader)
    - [FUnresolvedWorldSaveData](#funresolvedworldsavedata)

## Introduction

[Satisfactory](https://www.satisfactorygame.com/) is a game from [Coffee Stain Studios](https://www.coffeestainstudios.com/) and based on the [Unreal Engine](https://www.unrealengine.com/).
Many data structures in the save game are based on the Unreal serialization system.
Satisfactory uses a few data structures specific to the game.
Most of them can be found in the C++ headers, distributed within the `CommunityResources` folder in the game's installation directory.

This documentation tries to use the original Unreal/Satisfactory type names as closely as possible.
The structure of all types and objects used in this document is referenced at the end, see [Type and Object Reference](#type-and-object-reference).

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
Each time this struct is extended, the `SaveHeaderVersion` value increases, the current value is `13`.
Internally, an enum `Type` is used for this number, see `FGSaveManagerInterface.h` distributed with the game files.
The variable names are taken from the struct `FSaveHeader` in `FGSaveManagerInterface.h`.

`SaveDateTime` is the serialization of an [FDateTime object](https://docs.unrealengine.com/en-US/API/Runtime/Core/Misc/FDateTime/index.html).
Ticks since 0001-01-01 00:00, where 1 tick is 100 nanoseconds. Satisfactory seems to use the UTC time zone.

Source Reference: `FGSaveManagerInterface.h` - `struct FSaveHeader`

### Chunks

The whole save data is stored in a binary structure (see below).
This binary structure is divided into chunks, which are then individually compressed with zlib.
The division into chunks is done purely on size and has nothing to do with the serialized content within this binary structure.
In the save data, each chunk is prefixed by a header, followed by the compressed binary data.

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
After decompression, all uncompressed chunk buffers are merged into a single large binary object, defined in the next section.

> Unreal internal details:  
> Unreal can [serialize an archive to a compressed structure](https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Core/Private/Serialization/Archive.cpp#L679).
> Unreal uses the struct FCompressedChunkInfo to store chunk header information. It is a pair of two int64 sizes.
> A compressed archive starts with an FCompressedChunkInfo containing the [PACKAGE_FILE_TAG](https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Core/Public/UObject/ObjectVersion.h#L9) and [COMPRESSION_CHUNK_SIZE](https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Core/Public/Misc/Compression.h#L22).
> Next is an FCompressedChunkInfo with the summary compressed and uncompressed size of all data in this archive.
> That is followed by a series of FCompressedChunkInfo with the current chunks' size and the actual compressed junk data.
> There are as many chunks until the summary size is reached.
> But instead of using a single archive for the binary blob, Satisfactory uses the [FArchiveLoadCompressedProxy](https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Core/Public/Serialization/ArchiveLoadCompressedProxy.h) to store the big binary structure.
> Here, the data is not stored in a single archive, but multiple archives containing just a single chunk.
> Probably, this structure has the advantage of decompressing the large buffer internally step by step and not all at once.
> Nevertheless, this is probably why the buffer size seems to be stored twice in the chunk header.

## Decompressed binary data

With Update 6 and again Update 8, the format changed quite a bit, here, only the new format will be described.
The new format stores data for each level separately, where a level refers to a part of the game map.

The binary data layout follows the following format:
```
+-------------------------------------------+------------------------------------------------------+
| int64                                     | total size of binary data (not including this value) |
| FWorldPartitionValidationData             | ValidationData                                       |
| TMap<FString, FPerStreamingLevelSaveData> | mPerLevelDataMap                                     |
| FPersistentAndRuntimeSaveData             | mPersistentAndRuntimeData                            |
| FUnresolvedWorldSaveData                  | mUnresolvedWorldSaveData                             |
+-------------------------------------------+------------------------------------------------------+
```

Structs are defined in `FGSaveSession.h`.
FString in `mPerLevelDataMap` is the `levelName`.

[`FPerStreamingLevelSaveData`](#fperstreaminglevelsavedata) and [`FPersistentAndRuntimeSaveData`](#fpersistentandruntimesavedata) contain binary buffers named `TOCBlob64` and `DataBlob64`, described in detail below.
Within these, all objects of a level are stored.
The result is basically a list of all objects `TArray<UObject*> objects`.
However, information about each object is separated.
While the `TOCBlob` stores metainformation about each object, such as class name, the `DataBlob` contains all properties and class-specific data of each object.

Objects are either actors or basic objects.
Each object has a class name in the form `/Game/FactoryGame/Buildable/Building/Foundation/Build_Foundation_8x4_01.Build_Foundation_8x4_01_C` and an additional instance name.
This allows interpreting a save file in a hierarchical way, similar to a file system.

The size of each object varies.
Therefore, the data must be parsed sequentially.
Different objects are just written one by one after each other.
The number of objects in the TOCBlob and DataBlob are the same (first object in TOCBlob corresponds to first object in DataBlob, ...).

### TOCBlob

```
+-----------------------------------+-----------------+
| int32                             | numObjects      |
| for i = 1 to numObjects:          |                 |
|     bool                          | isActor         |
|     if isActor:                   |                 |
|         FActorSaveHeader          | objectHeader    |
|     else:                         |                 |
|         FObjectSaveHeader         | objectHeader    |
| if remaining data:                |                 |
|      TArray<FObjectReferenceDisc> | DestroyedActors |
+-----------------------------------+-----------------+
```

`DestroyedActors` is not always present.
It needs to be determined if the TOCBlob buffer has already been completely read after parsing the objects.

Serialization of the object part can be found in `FWPSaveDataMigrationContext.h`.

### DataBlob

```
+--------------------------+--------------------+
| int32                    | numObjects         |
| for i = 1 to numObjects: |                    |
|     int32                | version            | Seems to be save version per object, not verified yet.
|     int32                | unknown (0)        | Unknown, but always observed zero so far.
|     TArray<uint8>        | binary object data |
+--------------------------+--------------------+
```

Each object corresponds to an instance of a class representing any type of object.
The state of each instance is serialized using the Unreal serialization system.
Since each class can overwrite the serialization to any custom need, the binary object data, in theory, can have an arbitrary format depending on which class this object is of.
In practice, however, since all objects inherit from a common UObject type, the start of every object data follows a common pattern, just with a few classes of the game adding some additional data after this common part.
If the object is an actor, there are additional parent and child references at the beginning.
Further, most classes use the Unreal reflection system to store their properties.
Therefore, most data is in the `List of Properties`, which is in the common part of all objects.
Only very few classes make use of custom binary data (see [Class-Specific Binary Data](#class-specific-binary-data)).
Parsing the `List of Properties` is probably the most complex part of save parsing.
Therefore, the details are in a separate section below.
The size of the class-specific binary data is the remaining buffer, of which the total size is given by the containing TArray.

Here is the common structure used by all objects:

```
+----------------------------------+----------------------------+
| if isActor:                      |                            | (from TOCBlob)
|     FObjectReferenceDisc         | parent reference           |
|     TArray<FObjectReferenceDisc> | child references           |
| List of Properties               | properties                 |
| bool                             | HasGuid                    | (only observed false values so far)
| if HasGuid:                      |                            |
|     FGuid                        | Guid                       |
| if remaining data:               |                            |
|     ...                          | class-specific binary data |
+----------------------------------+----------------------------+
```

- [Reference to HasGuid source](https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Private/UObject/Obj.cpp#L1399).

## Properties

### List of Properties

Properties are stored one by one in a list, where the end of a list is always defined by a property with the name "None".
All Properties have a common header named PropertyTag and data, which is different for each property type.
```
+---------------+-----------------+
| PropertyTag   | Property 1      |
|---------------|                 |
| Property data |                 |
+---------------+-----------------+
| PropertyTag   | Property 2      |
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
| FName                            | Name            |
| if Name != "None":               |                 |
|     FName                        | Type            |
|     int32                        | Size            |
|     int32                        | ArrayIndex      |
|     if Type == "StructProperty": |                 |
|         FName                    | StructName      |
|         GUID                     | StructGuid      |
|     if Type == "BoolProperty":   |                 |
|         int8                     | BoolVal         |
|     if Type == "ByteProperty":   |                 |
|         FName                    | EnumName        |
|     if Type == "EnumProperty":   |                 |
|         FName                    | EnumName        |
|     if Type == "ArrayProperty":  |                 |
|         FName                    | InnerType       |
|     if Type == "SetProperty":    |                 |
|         FName                    | InnerType       |
|     if Type == "MapProperty":    |                 |
|         FName                    | InnerType       |
|         FName                    | ValueType       |
|     uint8                        | HasPropertyGuid |
|     if HasPropertyGuid:          |                 |
|         GUID                     | PropertyGuid    |
+----------------------------------+-----------------+
```

### Simple Types

#### BoolProperty

BoolProperty has no additional data.
The value is stored in the `BoolVal` field of the PropertyTag.

#### ByteProperty

The type of the value is stored in `Tag.EnumName`.

```
+----------------------------+-----------+
| if Tag.EnumName == "None": |           |
|     int8                   | valueByte |
| else:                      |           |
|     FName                  | valueName |
+----------------------------+-----------+
```

#### EnumProperty

The type of the enum is stored in `Tag.EnumName`.

```
+-------+-------+
| FName | value |
+-------+-------+
```

#### NameProperty

```
+-------+-------+
| FName | value |
+-------+-------+
```

#### ObjectProperty

```
+----------------------+-------+
| FObjectReferenceDisc | value |
+----------------------+-------+
```

#### SoftObjectProperty

TODO
<!--
Serialization starts here:
https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Private/UObject/PropertySoftObjectPtr.cpp#L46-L75
Following the serialization path seems to end here:
https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Private/UObject/SoftObjectPath.cpp#L244-L245
-->

#### StrProperty

```
+---------+-------+
| FString | value |
+---------+-------+
```

#### TextProperty

```
+-------+-------+
| FText | value |
+-------+-------+
```

### Numeric Types

#### IntProperty

```
+-------+-------+
| int32 | value |
+-------+-------+
```

#### Int8Property

```
+------+-------+
| int8 | value |
+------+-------+
```

#### Int64Property

```
+-------+-------+
| int64 | value |
+-------+-------+
```

#### UInt32Property

```
+--------+-------+
| uint32 | value |
+--------+-------+
```

#### UInt64Property

```
+--------+-------+
| uint64 | value |
+--------+-------+
```

#### FloatProperty

```
+-------+-------+
| float | value |
+-------+-------+
```

#### DoubleProperty

```
+--------+-------+
| double | value |
+--------+-------+
```

### Container Types

#### ArrayProperty

The type of the array is defined by `Tag.InnerType`.
For most types, the format of the data is:
```
+-----------+--------+
| TArray<T> | values |
+-----------+--------+
```

With using the following types:

| InnerType            | T                      |
|----------------------|------------------------|
| `BoolProperty`       | `int8`                 |
| `ByteProperty`       | `int8`                 |
| `EnumProperty`       | `FName`                |
| `FloatProperty`      | `float`                |
| `Int64Property`      | `int64`                |
| `IntProperty`        | `int32`                |
| `InterfaceProperty`  | `FObjectReferenceDisc` |
| `NameProperty`       | `FName`                |
| `ObjectProperty`     | `FObjectReferenceDisc` |
| `SoftObjectProperty` | `FSoftObjectPath`      |
| `StrProperty`        | `FString`              |

The type `StructProperty` has a custom format:

```
+---------------------+-------------+
| int32               | count       |
| PropertyTag         | innerTag    | (always type StructProperty)
| for i = 1 to count: |             |
|     Struct_T        | struct data |
+---------------------+-------------+
```

The type of the struct data is stored in `innerTag.StructName`.
For more information about structs, see [Structs](#structs).

#### MapProperty

The type of the map key is in `Tag.InnerType` and the type of the values `Tag.ValueType`.
The layout of the data is:
```
+-------------------------+-----------------+
| int32                   | NumKeysToRemove |
| if NumKeysToRemove > 0: |                 |
|     ... (unknown)       |                 | (not observed in save games)
| TMap<Key_T, Value_T>    | map data        |
+-------------------------+-----------------+
```

With using the following types:

| InnerType / ValueType | T                      |
|-----------------------|------------------------|
| `ByteProperty`        | `int8`                 |
| `EnumProperty`        | `FName`                |
| `FloatProperty`       | `float`                |
| `IntProperty`         | `int32`                |
| `NameProperty`        | `FName`                |
| `ObjectProperty`      | `FObjectReferenceDisc` |

In addition, `StructProperty` is used as type.
But maps have the problem that no information about which struct type is used is being serialized to the save game.
It is only possible to workaround this by manually creating a list of struct types using the parent class name and property name.
So far, the following types have been observed:

| Parent Class                                                     | Name                  | Key | Value | Struct Type                               | Notes             |
|------------------------------------------------------------------|-----------------------|-----|-------|-------------------------------------------|-------------------|
| `/Game/FactoryGame/Events/BP_EventSubsystem.BP_EventSubsystem_C` | `mStoredCalendarData` |     | x     | `CalendarData`                            |                   |
| `/Game/FactoryGame/Events/BP_EventSubsystem.BP_EventSubsystem_C` | `mCalendarData`       |     | x     | `CalendarData`                            |                   |
| `/Script/FactoryGame.FGFoliageRemovalSubsystem`                  | `mSaveData`           | x   |       | `IntVector`                               |                   |
| `/Script/FactoryGame.FGFoliageRemovalSubsystem`                  | `mSaveData`           |     | x     | `FoliageRemovalSaveDataPerCell`           |                   |
| `/Script/FactoryGame.FGFoliageRemovalSubsystem`                  | `mUnresolvedSaveData` | x   |       | `IntVector`                               |                   |
| `/Script/FactoryGame.FGFoliageRemovalSubsystem`                  | `mUnresolvedSaveData` |     | x     | `FoliageRemovalUnresolvedSaveDataPerCell` |                   |
| `/Script/FactoryGame.FGStatisticsSubsystem`                      | `mActorsBuiltCount`   |     | x     | `ActorBuiltData`                          |                   |
| `FoliageRemovalSaveDataPerCell`                                  | `SaveDataMap`         |     | x     | `FoliageRemovalSaveDataForFoliageType`    |                   |
| `FoliageRemovalUnresolvedSaveDataPerCell`                        | `SaveDataMap`         |     | x     | `FoliageRemovalSaveDataForFoliageType`    |                   |
| `LBBalancerData`                                                 | `mIndexMapping`       |     | x     | `LBBalancerIndexing`                      | Mod LoadBalancers |

For more information about structs, see [Structs](#structs).

#### SetProperty

The type of the set is defined by `Tag.InnerType`.
The layout of the data is:
```
+-----------------------------+---------------------+
| int32                       | NumElementsToRemove |
| if NumElementsToRemove > 0: |                     |
|     ... (unknown)           |                     | (not observed in save games)
| TArray<T>                   | values              |
+-----------------------------+---------------------+
```

With using the following types:

| InnerType        | T        |
|------------------|----------|
| `UInt32Property` | `uint32` |

In addition, `StructProperty` is used as type.
Similar to maps, sets have the problem that no information about which struct type is used is being serialized to the save game.
It is only possible to workaround this by manually creating a list of struct types using the parent class name and property name.
So far, the following types have been observed:

| Parent Class                           | Name                | Struct Type |
|----------------------------------------|---------------------|-------------|
| `/Script/FactoryGame.FGFoliageRemoval` | `mRemovalLocations` | `Vector`    |

For more information about structs, see [Structs](#structs).

#### StructProperty

The type of the struct is stored in `Tag.StructName`.
See [Structs](#structs) for the data layout of individual structs.

## Structs

Structs are typically objects containing multiple variables and could be interpreted as their own type.
From a save game serialization perspective, they could be sorted into two groups.
The first group uses the Unreal reflection system to store their values ([Property Structs](#property-structs)), and the others use their own binary serialization ([Binary Structs](#binary-structs)).

### Property Structs

Property structs are stored using the Unreal reflection system.
The format is exactly the same as a [`List of Properties`](#list-of-properties).

The following struct names were observed to be property structs:

`ActorBuiltData`,
`ActorTickFunction`,
`BlueprintCategoryRecord`,
`BlueprintRecord`,
`BlueprintSubCategoryRecord`,
`BodyInstance`,
`BoomBoxPlayerState`,
`BoxSphereBounds`,
`CalendarData`,
`CollisionResponse`,
`CustomizationDescToRecipeData`,
`DroneDockingStateInfo`,
`DroneTripInformation`,
`FactoryCustomizationColorSlot`,
`FactoryCustomizationData`,
`FactoryTickFunction`,
`FeetOffset`,
`FloatInterval`,
`FloatRange`,
`FloatRangeBound`,
`FoliageRemovalSaveDataForFoliageType`,
`FoliageRemovalSaveDataPerCell`,
`FoliageRemovalUnresolvedSaveDataPerCell`,
`FoundationSideSelectionFlags`,
`HeadlightParams`,
`Hotbar`,
`InstanceData`,
`InventoryStack`,
`ItemAmount`,
`ItemFoundData`,
`LBBalancerData` (Mod LoadBalancers),
`LightSourceControlData`,
`MapMarker`,
`MaterialCachedExpressionData`,
`MaterialInstanceBasePropertyOverrides`,
`MaterialParameterInfo`,
`MeshUVChannelInfo`,
`MessageData`,
`MiniGameResult`,
`ParticleMap`,
`PhaseCost`,
`PlayerRules`,
`PointerToUberGraphFrame`,
`PrefabIconElementSaveData`,
`PrefabTextElementSaveData`,
`RecipeAmountStruct`,
`RemovedInstance`,
`RemovedInstanceArray`,
`ResearchData`,
`ResearchTime`,
`ResourceSinkHistory`,
`ResponseChannel`,
`ScalarParameterValue`,
`ScannableObjectData`,
`ScannableResourcePair`,
`SchematicCost`,
`SpawnData`,
`SplinePointData`,
`SplitterSortRule`,
`StaticMaterial`,
`StaticParameterSet`,
`StaticSwitchParameter`,
`StringPair`,
`SubCategoryMaterialDefault`,
`TextureParameterValue`,
`TimerHandle`,
`TimeTableStop`,
`TireTrackDecalDetails`,
`TrainDockingRuleSet`,
`TrainSimulationData`,
`Transform`,
`Vector_NetQuantize`,
`WireInstance`

As property structs seem more common than binary structs, save game parsers may just try to parse unknown structs as property structs.

### Binary Structs

The following structs are binary structs with the type described in the table:

| Struct Name             | Type                     | Description                                                              | Notes               |
|-------------------------|--------------------------|--------------------------------------------------------------------------|---------------------|
| `Box`                   | `FBox`                   | `FVector Min`<br>`FVector Max`<br>`uint8 IsValid`                        |                     |
| `Color`                 | `FColor`                 | `uint8 B`<br>`uint8 G`<br>`uint8 R`<br>`uint8 A`                         |                     |
| `FluidBox`              | `FFluidBox`              | `float Value`                                                            |                     |
| `Guid`                  | `FGuid`                  | [FGuid](#fguid)                                                          |                     |
| `IntPoint`              | `FIntPoint`              | `int32 X`<br>`int32 Y`                                                   |                     |
| `IntVector`             | `FIntVector`             | `int32 X`<br>`int32 Y`<br>`int32 Z`                                      |                     |
| `InventoryItem`         | `FInventoryItem`         | `int32 unknown`?<br>`FString class_name`?<br>`FObjectReferenceDisc ref`? |                     |
| `LBBalancerIndexing`    | `FLBBalancerIndexing`    | `int32 mNormalIndex`<br>`int32 mOverflowIndex`<br>`int32 mFilterIndex`   | (Mod LoadBalancers) |
| `LinearColor`           | `FLinearColor`           | `float R`<br>`float G`<br>`float B`<br>`float A`                         |                     |
| `Quat`                  | `FQuat`                  | `double X`<br>`double Y`<br>`double Z`<br>`double W`                     |                     |
| `RailroadTrackPosition` | `FRailroadTrackPosition` | `FObjectReferenceDisc Track`<br>`float Offset`<br>`float Forward`        |                     |
| `Rotator`               | `FRotator`               | `double Pitch`<br>`double Yaw`<br>`double Roll`                          |                     |
| `SoftClassPath`         | `FSoftObjectPath`        | TODO                                                                     |                     |
| `Vector2D`              | `FVector2D`              | `double X`<br>`double Y`                                                 |                     |
| `Vector`                | `FVector`                | `double X`<br>`double Y`<br>`double Z`                                   |                     |

## Class-Specific Binary Data

TODO

## Type and Object Reference

### Basic types

Integer types are all named by their size in bits, e.g. int8, int32, int64, uint8, uint32, uint64, etc.
Floating point numbers are either defined as float (32 bit) or double (64 bit).
Bools are stored as uint32 in the save game.

### Containers

#### TArray

`TArray<T>` is defined in the following way:
```
+--------+-------+
| int32  | num   |
| T[num] | items |
+--------+-------+
```

TArray is a template, and the size of a single item is defined by the underlying type.
In addition, variants templating the size exist, i.e. using int64 `TArray<T, int64>`:
```
+--------+-------+
| int64  | num   |
| T[num] | items |
+--------+-------+
```

#### TMap

TMap<Key_T, Value_T>:
```
+--------------------+-------+
| int32              | size  |
| for i = 1 to size: |       |
|     Key_T          | key   |
|     Value_T        | value |
+--------------------+-------+
```

TMap is a template and the size of a single key and value items is defined by the underlying type.

### Unreal Objects

#### FString

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

#### FName

FName is serialized as [`FString`](#fstring) in the save game.

#### FText

```
+-----------------------------------+---------------------------+
| uint32                            | Flags                     |
| int8 (ETextHistoryType)           | HistoryType               |
| if historyType == -1:             |                           | (ETextHistoryType::None)
|     bool                          | HasCultureInvariantString |
|     if HasCultureInvariantString: |                           |
|         FString                   | TextData                  |
| if historyType == 0:              |                           | (ETextHistoryType::Base)
|     FString                       | Namespace                 |
|     FString                       | Key                       |
|     FString                       | SourceString              |
| if historyType == ?:              |                           |
|     ... TODO                      |                           | (not observed in save games)
+-----------------------------------+---------------------------+
```

 - [Reference to ETextFlag](https://github.com/EpicGames/UnrealEngine/blob/5.2.1-release/Engine/Source/Runtime/Core/Public/Internationalization/Text.h#L37-L47)
 - [Reference to ETextHistoryType](https://github.com/EpicGames/UnrealEngine/blob/5.2.1-release/Engine/Source/Runtime/Core/Private/Internationalization/TextHistory.h#L22-L40)
 - [Reference to serialization source code](https://github.com/EpicGames/UnrealEngine/blob/5.2.1-release/Engine/Source/Runtime/Core/Private/Internationalization/Text.cpp#L823)

#### FGuid

```
+--------+---+
| uint32 | A |
| uint32 | B |
| uint32 | C |
| uint32 | D |
+--------+---+
```

#### FMD5Hash

```
+---------------+----------+
| bool          | bIsValid |
| if bIsValid:  |          |
|     uint8[16] | Bytes    |
+---------------+----------+
```

### Satisfactory Objects

#### FObjectReferenceDisc

Another common type used within the save data is an `FObjectReferenceDisc`, defined in the following way:
```
+---------+------------+
| FString | level name |
| FString | path name  |
+---------+------------+
```

- Satisfactory internal struct, header file: `FGObjectReference.h`

#### FWorldPartitionValidationData

```
+------------------------------------+-------+
| TMap<FName, FWPGridValidationData> | Grids |
+------------------------------------+-------+
```

#### FWPGridValidationData

```
+-----------------------+------------+
| int32                 | CellSize   |
| uint32                | GridHash   |
| TMap<FName, uint32_t> | CellHashes |
+-----------------------+------------+
```

#### FPerStreamingLevelSaveData

```
+------------------------------+-----------------+
| TArray<uint8, int64>         | TOCBlob64       |
| TArray<uint8, int64>         | DataBlob64      |
| TArray<FObjectReferenceDisc> | DestroyedActors |
+------------------------------+-----------------+
```

#### FPersistentAndRuntimeSaveData

```
+---------------------------------------------+---------------------------+
| TArray<uint8, int64>                        | TOCBlob64                 |
| TArray<uint8, int64>                        | DataBlob64                |
| TMap<FString, TArray<FObjectReferenceDisc>> | LevelToDestroyedActorsMap |
+---------------------------------------------+---------------------------+
```

#### FObjectBaseSaveHeader

```
+----------------------+-----------+
| FString              | ClassName |
| FObjectReferenceDisc | Reference |
+----------------------+-----------+
```

Defined in `FGActorSaveHeaderTypes.h`.

#### FObjectSaveHeader

```
+-----------------------+---------------+
| FObjectBaseSaveHeader | BaseHeader    |
| FString               | OuterPathName |
+-----------------------+---------------+
```

Defined in `FGActorSaveHeaderTypes.h`.

#### FActorSaveHeader

```
+-----------------------+------------------+
| FObjectBaseSaveHeader | ObjectHeader     |
| bool                  | NeedTransform    |
| FTransform3f          | Transform        |
| bool                  | WasPlacedInLevel |
+-----------------------+------------------+
```

Defined in `FGActorSaveHeaderTypes.h`.

`FTransform3f` is defined as:

```
+----------+---------------------+-------------------------------------------+
| float[4] | rotation            | quaternion describing object rotation     |
| float[3] | position            | world position of object (in centi meter) |
| float[3] | scale               | object scale                              |
+----------+---------------------+-------------------------------------------+
```

#### FUnresolvedWorldSaveData

```
+------------------------------+-----------------+
| TArray<FObjectReferenceDisc> | DestroyedActors |
+------------------------------+-----------------+
```
