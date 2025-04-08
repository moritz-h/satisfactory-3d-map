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
- [Objects](#objects)
  - [UObject](#uobject)
  - [AActor](#aactor)
  - [AFGBuildableConveyorBase](#afgbuildableconveyorbase)
  - [AFGConveyorChainActor](#afgconveyorchainactor)
  - [AFGBuildableWire](#afgbuildablewire)
  - [AFGCircuitSubsystem](#afgcircuitsubsystem)
  - [AFGLightweightBuildableSubsystem](#afglightweightbuildablesubsystem)
  - [AFGGameMode](#afggamemode)
  - [AFGGameState](#afggamestate)
  - [AFGPlayerState](#afgplayerstate)
  - [AFGVehicle](#afgvehicle)
  - [AFGRailroadVehicle](#afgrailroadvehicle)
  - [AFGDroneVehicle](#afgdronevehicle)
  - [UActorComponent](#uactorcomponent)
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
    - [FDateTime](#fdatetime)
  - [Satisfactory Objects](#satisfactory-objects)
    - [FObjectReferenceDisc](#fobjectreferencedisc)
    - [FFGDynamicStruct](#ffgdynamicstruct)
    - [FWorldPartitionValidationData](#fworldpartitionvalidationdata)
    - [FWPGridValidationData](#fwpgridvalidationdata)
    - [FPerStreamingLevelSaveData](#fperstreaminglevelsavedata)
    - [FPersistentAndRuntimeSaveData](#fpersistentandruntimesavedata)
    - [FObjectBaseSaveHeader](#fobjectbasesaveheader)
    - [FObjectSaveHeader](#fobjectsaveheader)
    - [FActorSaveHeader](#factorsaveheader)
    - [FUnresolvedWorldSaveData](#funresolvedworldsavedata)
    - [FInventoryItem](#finventoryitem)
    - [FConveyorBeltItem](#fconveyorbeltitem)
    - [FConveyorBeltItems](#fconveyorbeltitems)
    - [FConveyorChainSplineSegment](#fconveyorchainsplinesegment)
    - [FSplinePointData](#fsplinepointdata)
    - [FRuntimeBuildableInstanceData](#fruntimebuildableinstancedata)
    - [FUniqueNetIdRepl](#funiquenetidrepl)
    - [FVehiclePhysicsData](#fvehiclephysicsdata)
    - [FTopLevelAssetPath](#ftoplevelassetpath)
    - [FBlueprintItemAmount](#fblueprintitemamount)
    - [FLocalUserNetIdBundle](#flocalusernetidbundle)
- [Blueprints](#blueprints)
  - [BlueprintCfg File](#blueprintcfg-file)
  - [Blueprint File](#blueprint-file)
    - [Blueprint Header](#blueprint-header)
    - [Blueprint Binary Data](#blueprint-binary-data)
      - [Blueprint TOCData](#blueprint-tocdata)
      - [Blueprint BlobData](#blueprint-blobdata)

## Introduction

[Satisfactory](https://www.satisfactorygame.com/) is a game from [Coffee Stain Studios](https://www.coffeestainstudios.com/) based on the [Unreal Engine](https://www.unrealengine.com/).
Many data structures in the save game are based on the Unreal serialization system.
Satisfactory uses a few data structures specific to the game.
Most of them can be found in the C++ headers, distributed within the `CommunityResources` folder in the game's installation directory.

This documentation tries to use the original Unreal/Satisfactory type names as closely as possible.
The structure of all types and objects used in this document is referenced at the end, see [Type and Object Reference](#type-and-object-reference).

All binary data is encoded little-endian in the save.

Document Version: Satisfactory 1.1

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
+-----------------------------------------------------+-----------------------+
| int32                                               | SaveHeaderVersion     |
| int32                                               | SaveVersion           |
| int32                                               | BuildVersion          |
| if SaveHeaderVersion >= 14:                         |                       |
|     FString                                         | SaveName              |
| FString                                             | MapName               |
| FString                                             | MapOptions            |
| if SaveHeaderVersion >= 2 && SaveHeaderVersion < 4: |                       |
|     int32                                           | sessionIDInt          |
| if SaveHeaderVersion >= 4:                          |                       |
|     FString                                         | SessionName           |
| if SaveHeaderVersion >= 3:                          |                       |
|     int32                                           | PlayDurationSeconds   |
| if SaveHeaderVersion >= 4:                          |                       |
|     FDateTime                                       | SaveDateTime          |
| if SaveHeaderVersion >= 5:                          |                       |
|     int8                                            | SessionVisibility     | value is not used anymore
| if SaveHeaderVersion >= 7:                          |                       |
|     int32                                           | EditorObjectVersion   |
| if SaveHeaderVersion >= 8:                          |                       |
|     FString                                         | ModMetadata           |
|     bool                                            | IsModdedSave          |
| if SaveHeaderVersion >= 10:                         |                       |
|     FString                                         | SaveIdentifier        |
| if SaveHeaderVersion >= 11:                         |                       |
|     bool                                            | IsPartitionedWorld    |
| if SaveHeaderVersion >= 12:                         |                       |
|     FMD5Hash                                        | SaveDataHash          |
| if SaveHeaderVersion >= 13:                         |                       |
|     bool                                            | IsCreativeModeEnabled |
+-----------------------------------------------------+-----------------------+
```

The save header did change over time.
This is considered in `SaveHeaderVersion`, the version increases every time this struct is changed.
For Update 8 and 1.0 the header version was `13`, for 1.1 it is `14`.
Internally, an enum `Type` is used for this number, see `FGSaveManagerInterface.h` distributed with the game files.
The variable names are taken from the struct `FSaveHeader` in `FGSaveManagerInterface.h`.

> Source Reference: `FGSaveManagerInterface.h` - `struct FSaveHeader`

### Chunks

The save data is stored in a binary structure (see below).
This binary structure is divided into chunks, which are then individually compressed with zlib.
The division into chunks is done purely on size and has nothing to do with the serialized content within this binary structure.
In the save data, each chunk is prefixed by a header, followed by the compressed binary data.

The chunk header has the following structure:

```
+---------------+---------------------------+----------------------------------------------+
| int32         | PACKAGE_FILE_TAG          | always `0x9E2A83C1`                          |
| int32         | archive header            | 0x00000000: v1 header, 0x22222222: v2 header |
| int64         | max chunk size            | always `131072`                              |
| if v2 header: |                           |                                              |
|     uint8     | CompressorNum             | compression algorithm, 3: zlib               |
| int64         | compressed size summary   | compressed buffer size                       |
| int64         | uncompressed size summary | uncompressed buffer size                     |
| int64         | compressed size           | compressed buffer size                       |
| int64         | uncompressed size         | uncompressed buffer size                     |
+---------------+---------------------------+----------------------------------------------+
```

`PACKAGE_FILE_TAG` is a constant magic number.
`archive header` was introduced with the Unreal 5 upgrade and marks if CompressorNum exists.
`max chunk size` is the maximum size used for (uncompressed) chunks.
This is a hardcoded constant from Unreal.
`CompressorNum` is the index of the used compression algorithm, which currently seems to be always zlib (value = 3).
`compressed size` refers to the size of the compressed chunk data within the save file.
`uncompressed size` is the size of the chunk data after decompression with zlib.
All chunks, except the last one, use `max chunk size` as uncompressed size.
The sizes are stored twice with identical values (see below).
After decompression, all uncompressed chunk buffers are merged into a single large binary object, defined in the next section.

> Unreal internal details:  
> Unreal can [serialize an archive to a compressed structure](https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Core/Private/Serialization/Archive.cpp#L679).
> Unreal uses the struct FCompressedChunkInfo to store chunk header information. It is a pair of two int64 sizes.
> A compressed archive starts with an FCompressedChunkInfo containing the [PACKAGE_FILE_TAG](https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Core/Public/UObject/ObjectVersion.h#L9) and [COMPRESSION_CHUNK_SIZE](https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Core/Public/Misc/Compression.h#L22).
> Next is an FCompressedChunkInfo with the summary compressed and uncompressed size of all data in this archive.
> That is followed by a series of FCompressedChunkInfo with the current chunks' size and the actual compressed chunk data.
> There are as many chunks until the summary size is reached.
> But instead of using a single archive for the binary blob, Satisfactory uses the [FArchiveLoadCompressedProxy](https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Core/Public/Serialization/ArchiveLoadCompressedProxy.h) to store the big binary structure.
> Here, the data is not stored in a single archive but multiple archives containing just a single chunk.
> Probably, this structure has the advantage of decompressing the large buffer internally step by step and not all at once.
> Nevertheless, this explains why the buffer size is stored twice in the chunk header.

## Decompressed binary data

With Update 6 and again Update 8, the format changed quite a bit; only the new format will be described here.
The new format stores data for each level separately, where a level refers to a part of the game map.

The binary data layout follows the following format:

```
+-------------------------------------------+------------------------------------------------------+
| int64                                     | total size of binary data (not including this value) |
| FWorldPartitionValidationData             | SaveGameValidationData                               |
| TMap<FString, FPerStreamingLevelSaveData> | mPerLevelDataMap                                     |
| FPersistentAndRuntimeSaveData             | mPersistentAndRuntimeData                            |
| FUnresolvedWorldSaveData                  | mUnresolvedWorldSaveData                             |
+-------------------------------------------+------------------------------------------------------+
```

Structs are defined in `FGSaveSession.h`.
FString in `mPerLevelDataMap` is the `levelName`.

[`FPerStreamingLevelSaveData`](#fperstreaminglevelsavedata) and [`FPersistentAndRuntimeSaveData`](#fpersistentandruntimesavedata) contain binary buffers named `TOCBlob64` and `DataBlob64`, described in detail below.
Within these, all objects of a level are stored.
The result is a list of all objects `TArray<UObject*> objects`.
However, information about each object is separated.
`TOCBlob64` stores metainformation about each object, such as the class name.
`DataBlob64` contains the internal data of each object, i.e., a list of properties.

Each object has a class name in the form `/Game/FactoryGame/Buildable/Building/Foundation/Build_Foundation_8x4_01.Build_Foundation_8x4_01_C` and an additional instance name.
This allows for the interpretation of a save file in a hierarchical way, similar to a file system.

The size of each object varies.
Therefore, the data must be parsed sequentially.
Different objects are just written one by one after each other.
The number of objects in the TOCBlob and DataBlob are the same (first object in TOCBlob corresponds to first object in DataBlob, ...).

### TOCBlob

```
+-----------------------------------------------------+---------------------------+
| int32                                               | numObjects                |
| for i = 1 to numObjects:                            |                           |
|     bool                                            | isActor                   |
|     if isActor:                                     |                           |
|         FActorSaveHeader                            | objectHeader              |
|     else:                                           |                           |
|         FObjectSaveHeader                           | objectHeader              |
| if remaining data:                                  |                           |
|     if within FPerStreamingLevelSaveData:           |                           |
|         TArray<FObjectReferenceDisc>                | DestroyedActors           |
|     if within FPersistentAndRuntimeSaveData:        |                           |
|         TMap<FString, TArray<FObjectReferenceDisc>> | LevelToDestroyedActorsMap |
+-----------------------------------------------------+---------------------------+
```

`DestroyedActors` is not always present.
It needs to be determined if the TOCBlob buffer has already been completely read after parsing the object headers.

> Internally, the object headers are stored as `FGenericObjectSaveHeader`.
> The serialization code can be found in `FWPSaveDataMigrationContext.h` in the game headers.

### DataBlob

```
+--------------------------+-------------------------------------+
| int32                    | numObjects                          |
| for i = 1 to numObjects: |                                     |
|     int32                | SaveVersion                         |
|     bool                 | ShouldMigrateObjectRefsToPersistent |
|     TArray<uint8>        | Data                                | binary object data
+--------------------------+-------------------------------------+
```

The size of the `Data` array can be used to skip over the current object.
This allows only partially reading data of some objects or skipping of unknown objects.

> Internally, this data is stored in the `FObjectSaveData` struct.
> The serialization code can be found in `FWPSaveDataMigrationContext.h` in the game headers.

## Objects

Each object corresponds to an instance of a class representing any type of object.
The state of each instance is serialized using the Unreal serialization system.
All objects use a common `UObject` class as base, but each class can add additional binary save data.
However, most classes use the Unreal reflection system to store their properties.
Each UObject contains a `List of Properties` that stores all properties that utilize that reflection system.
There are only a few classes that add custom binary data.

Parsing the `List of Properties` is probably the most complex part of save parsing.
Therefore, the details are in a separate section below.
The size of the class-specific binary data is the remaining buffer, of which the total size is given by the containing TArray.

The second most commonly used class is `AActor`.
If `isActor` is true in the TOCBlob, the object is always an `AActor` instance or a class derived from `AActor`.

> Unreal internal details:  
> The UObject class has a virtual method `Serialize`, that can be overwritten by child classes to serialize data.
> Each class that overwrites the `Serialize` method usually calls the parent `Serialize` method, so child classes usually only add additional data.

> Note:
> The actual classes in the game source files may have additional classes in their inheritance tree.
> Here, only classes adding data to the serialization are mentioned.

### UObject

```
+--------------------+------------+
| List of Properties | Properties |
| bool               | HasGuid    | (only observed false values so far)
| if HasGuid:        |            |
|     FGuid          | Guid       |
+--------------------+------------+
```

- [Reference to HasGuid source](https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Private/UObject/Obj.cpp#L1399).

### AActor

```
+------------------------------+----------------+
| FObjectReferenceDisc         | Owner          |
| TArray<FObjectReferenceDisc> | Components     |
| UObject                      | -> see UObject |
+------------------------------+----------------+
```

### AFGBuildableConveyorBase

```
+--------------------+---------------+
| AActor             | -> see AActor |
| FConveyorBeltItems | mItems        |
+--------------------+---------------+
```

The following classes are instances of this:

```
/Game/FactoryGame/Buildable/Factory/ConveyorBeltMk1/Build_ConveyorBeltMk1.Build_ConveyorBeltMk1_C
/Game/FactoryGame/Buildable/Factory/ConveyorBeltMk2/Build_ConveyorBeltMk2.Build_ConveyorBeltMk2_C
/Game/FactoryGame/Buildable/Factory/ConveyorBeltMk3/Build_ConveyorBeltMk3.Build_ConveyorBeltMk3_C
/Game/FactoryGame/Buildable/Factory/ConveyorBeltMk4/Build_ConveyorBeltMk4.Build_ConveyorBeltMk4_C
/Game/FactoryGame/Buildable/Factory/ConveyorBeltMk5/Build_ConveyorBeltMk5.Build_ConveyorBeltMk5_C
/Game/FactoryGame/Buildable/Factory/ConveyorBeltMk6/Build_ConveyorBeltMk6.Build_ConveyorBeltMk6_C
/Game/FactoryGame/Buildable/Factory/ConveyorLiftMk1/Build_ConveyorLiftMk1.Build_ConveyorLiftMk1_C
/Game/FactoryGame/Buildable/Factory/ConveyorLiftMk2/Build_ConveyorLiftMk2.Build_ConveyorLiftMk2_C
/Game/FactoryGame/Buildable/Factory/ConveyorLiftMk3/Build_ConveyorLiftMk3.Build_ConveyorLiftMk3_C
/Game/FactoryGame/Buildable/Factory/ConveyorLiftMk4/Build_ConveyorLiftMk4.Build_ConveyorLiftMk4_C
/Game/FactoryGame/Buildable/Factory/ConveyorLiftMk5/Build_ConveyorLiftMk5.Build_ConveyorLiftMk5_C
/Game/FactoryGame/Buildable/Factory/ConveyorLiftMk6/Build_ConveyorLiftMk6.Build_ConveyorLiftMk6_C
```

### AFGConveyorChainActor

```
+-------------------------------------+----------------------+
| AActor                              | -> see AActor        |
| FObjectReferenceDisc                | mFirstConveyor       |
| FObjectReferenceDisc                | mLastConveyor        |
| TArray<FConveyorChainSplineSegment> | mChainSplineSegments |
| float                               | mTotalLength         |
| int32                               | mNumItems            |
| int32                               | mLeadItemIndex       |
| int32                               | mTailItemIndex       |
| TArray<FConveyorBeltItem>           | mConveyorChainItems  |
+-------------------------------------+----------------------+
```

The following classes are instances of this:

```
/Script/FactoryGame.FGConveyorChainActor
/Script/FactoryGame.FGConveyorChainActor_RepSizeHuge
/Script/FactoryGame.FGConveyorChainActor_RepSizeLarge
/Script/FactoryGame.FGConveyorChainActor_RepSizeMedium
/Script/FactoryGame.FGConveyorChainActor_RepSizeNoCull
```

### AFGBuildableWire

```
+-------------------------+---------------+
| AActor                  | -> see AActor |
| FObjectReferenceDisc[2] | mConnections  | (fixed-sized array of length two)
+-------------------------+---------------+
```

The following classes are instances of this:

```
/Game/FactoryGame/Buildable/Factory/PowerLine/Build_PowerLine.Build_PowerLine_C
/Game/FactoryGame/Events/Christmas/Buildings/PowerLineLights/Build_XmassLightsLine.Build_XmassLightsLine_C
```

### AFGCircuitSubsystem

```
+-----------------------------------+---------------+
| AActor                            | -> see AActor |
| TMap<int32, FObjectReferenceDisc> | mCircuits     |
+-----------------------------------+---------------+
```

The following classes are instances of this:

```
/Game/FactoryGame/-Shared/Blueprint/BP_CircuitSubsystem.BP_CircuitSubsystem_C
```

### AFGLightweightBuildableSubsystem

```
+-------------------------------------------------------------------+--------------------------------+
| AActor                                                            | -> see AActor                  |
| if SaveVersion >= 48:                                             |                                |
|     int32                                                         | currentLightweightVersion      | set to 1 for older saves
| TMap<FObjectReferenceDisc, TArray<FRuntimeBuildableInstanceData>> | mBuildableClassToInstanceArray |
+-------------------------------------------------------------------+--------------------------------+
```

The following classes are instances of this:

```
/Script/FactoryGame.FGLightweightBuildableSubsystem
```

### AFGGameMode

```
+------------------------------+------------------------+
| AActor                       | -> see AActor          |
| TArray<FObjectReferenceDisc> | rawPlayerStatePointers |
+------------------------------+------------------------+
```

The following classes are instances of this:

```
/Game/FactoryGame/-Shared/Blueprint/BP_GameMode.BP_GameMode_C
```

### AFGGameState

```
+------------------------------+------------------------+
| AActor                       | -> see AActor          |
| TArray<FObjectReferenceDisc> | rawPlayerStatePointers |
+------------------------------+------------------------+
```

The following classes are instances of this:

```
/Game/FactoryGame/-Shared/Blueprint/BP_GameState.BP_GameState_C
```

### AFGPlayerState

```
+------------------+---------------+
| AActor           | -> see AActor |
| FUniqueNetIdRepl | Id            |
+------------------+---------------+
```

The following classes are instances of this:

```
/Game/FactoryGame/Character/Player/BP_PlayerState.BP_PlayerState_C
```

### AFGVehicle

```
+-----------------------------+--------------------+
| AActor                      | -> see AActor      |
| TArray<FVehiclePhysicsData> | mStoredPhysicsData |
+-----------------------------+--------------------+
```

The following classes are instances of this:

```
/Game/FactoryGame/Buildable/Vehicle/Cyberwagon/Testa_BP_WB.Testa_BP_WB_C
/Game/FactoryGame/Buildable/Vehicle/Explorer/BP_Explorer.BP_Explorer_C
/Game/FactoryGame/Buildable/Vehicle/Golfcart/BP_Golfcart.BP_Golfcart_C
/Game/FactoryGame/Buildable/Vehicle/Golfcart/BP_GolfcartGold.BP_GolfcartGold_C
/Game/FactoryGame/Buildable/Vehicle/Tractor/BP_Tractor.BP_Tractor_C
/Game/FactoryGame/Buildable/Vehicle/Truck/BP_Truck.BP_Truck_C
```

### AFGRailroadVehicle

```
+----------------------+----------------------+
| AFGVehicle           | -> see AFGVehicle    |
| FObjectReferenceDisc | mCoupledVehicleFront |
| FObjectReferenceDisc | mCoupledVehicleBack  |
+----------------------+----------------------+
```

The following classes are instances of this:

```
/Game/FactoryGame/Buildable/Vehicle/Train/Locomotive/BP_Locomotive.BP_Locomotive_C
/Game/FactoryGame/Buildable/Vehicle/Train/Wagon/BP_FreightWagon.BP_FreightWagon_C
```

### AFGDroneVehicle

```
+-----------------------+-------------------+
| AFGVehicle            | -> see AFGVehicle |
| bool                  | has_mActiveAction |
| if has_mActiveAction: |                   |
|     FDroneAction      | mActiveAction     |
| TArray<FDroneAction>  | mActionQueue      |
+-----------------------+-------------------+
```

FDroneAction is defined as:

```
+--------------------+------------------+
| FName              | actionStructName |
| List of Properties | action           |
+--------------------+------------------+
```

The following classes are instances of this:

```
/Game/FactoryGame/Buildable/Factory/DroneStation/BP_DroneTransport.BP_DroneTransport_C
```

### UActorComponent

```
+--------------------------------+-----------------------+
| UObject                        | -> see UObject        |
| TArray<FSimpleMemberReference> | UCSModifiedProperties | (only observed empty array so far)
+--------------------------------+-----------------------+
```

> Note:
> Only save games with an empty `UCSModifiedProperties` array has been observed so far.
> Therefore, the type `FSimpleMemberReference` is unused.
> Data can be parsed as int32 with zero check.

The following classes are instances of this:

```
/Script/FactoryGame.FGDroneMovementComponent
/Script/FactoryGame.FGFactoryConnectionComponent
/Script/FactoryGame.FGFactoryLegsComponent
/Script/FactoryGame.FGHealthComponent
/Script/FactoryGame.FGInventoryComponent
/Script/FactoryGame.FGInventoryComponentEquipment
/Script/FactoryGame.FGInventoryComponentTrash
/Script/FactoryGame.FGPipeConnectionComponent
/Script/FactoryGame.FGPipeConnectionComponentHyper
/Script/FactoryGame.FGPipeConnectionFactory
/Script/FactoryGame.FGPowerConnectionComponent
/Script/FactoryGame.FGPowerInfoComponent
/Script/FactoryGame.FGRailroadTrackConnectionComponent
/Script/FactoryGame.FGShoppingListComponent
/Script/FactoryGame.FGTrainPlatformConnection
```

## Properties

### List of Properties

Properties are stored one by one in a list, where the end of a list is always defined by a property with the name "None".
All Properties have a common header named PropertyTag and data, which differs for each property type.

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
| `Int8Property`       | `int8`                 |
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
| `Int64Property`       | `int64`                |
| `IntProperty`         | `int32`                |
| `NameProperty`        | `FName`                |
| `ObjectProperty`      | `FObjectReferenceDisc` |

In addition, `StructProperty` is used as type.
But maps have the problem that no information about which struct type is used is being serialized to the save game.
It is only possible to workaround this by manually creating a list of struct types using the parent class name and property name.
So far, the following types have been observed:

| Parent Class                                                                               | Name                         | Key | Value | Struct Type                               | Notes             |
|--------------------------------------------------------------------------------------------|------------------------------|-----|-------|-------------------------------------------|-------------------|
| `/Game/FactoryGame/Buildable/Factory/TruckStation/Build_TruckStation.Build_TruckStation_C` | `mDockingVehicleStatistics`  |     | x     | `DockingVehicleStatistics`                |                   |
| `/Game/FactoryGame/Events/BP_EventSubsystem.BP_EventSubsystem_C`                           | `mStoredCalendarData`        |     | x     | `CalendarData`                            |                   |
| `/Game/FactoryGame/Events/BP_EventSubsystem.BP_EventSubsystem_C`                           | `mCalendarData`              |     | x     | `CalendarData`                            |                   |
| `/Game/FactoryGame/Events/BP_EventSubsystem.BP_EventSubsystem_C`                           | `mCalendarsOpenedByPlayers`  |     | x     | `PlayerStateSetWrapper`                   |                   |
| `/Script/FactoryGame.FGFoliageRemovalSubsystem`                                            | `mSaveData`                  | x   |       | `IntVector`                               |                   |
| `/Script/FactoryGame.FGFoliageRemovalSubsystem`                                            | `mSaveData`                  |     | x     | `FoliageRemovalSaveDataPerCell`           |                   |
| `/Script/FactoryGame.FGFoliageRemovalSubsystem`                                            | `mUnresolvedSaveData`        | x   |       | `IntVector`                               |                   |
| `/Script/FactoryGame.FGFoliageRemovalSubsystem`                                            | `mUnresolvedSaveData`        |     | x     | `FoliageRemovalUnresolvedSaveDataPerCell` |                   |
| `/Script/FactoryGame.FGStatisticsSubsystem`                                                | `mItemsManuallyCraftedCount` |     | x     | `MappedItemAmount`                        |                   |
| `/Script/FactoryGame.FGStatisticsSubsystem`                                                | `mItemsPickedUp`             |     | x     | `MappedItemAmount`                        |                   |
| `/Script/FactoryGame.FGStatisticsSubsystem`                                                | `mActorsBuiltCount`          |     | x     | `ActorBuiltData`                          |                   |
| `FoliageRemovalSaveDataPerCell`                                                            | `SaveDataMap`                |     | x     | `FoliageRemovalSaveDataForFoliageType`    |                   |
| `FoliageRemovalUnresolvedSaveDataPerCell`                                                  | `SaveDataMap`                |     | x     | `FoliageRemovalSaveDataForFoliageType`    |                   |
| `LBBalancerData`                                                                           | `mIndexMapping`              |     | x     | `LBBalancerIndexing`                      | Mod LoadBalancers |

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

| InnerType        | T                      |
|------------------|------------------------|
| `ObjectProperty` | `FObjectReferenceDisc` |
| `UInt32Property` | `uint32`               |

In addition, `StructProperty` is used as type.
Similar to maps, sets have the problem that no information about which struct type is used is being serialized to the save game.
It is only possible to workaround this by manually creating a list of struct types using the parent class name and property name.
So far, the following types have been observed:

| Parent Class                               | Name                | Struct Type |
|--------------------------------------------|---------------------|-------------|
| `/Script/FactoryGame.FGFoliageRemoval`     | `mRemovalLocations` | `Vector`    |
| `/Script/FactoryGame.FGScannableSubsystem` | `mDestroyedPickups` | `Guid`      |
| `/Script/FactoryGame.FGScannableSubsystem` | `mLootedDropPods`   | `Guid`      |

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
`DockingVehicleStatistics`,
`DroneDockingStateInfo`,
`DroneTripInformation`,
`ElevatorFloorStopInfo`,
`FactoryCustomizationColorSlot`,
`FactoryCustomizationData`,
`FactoryTickFunction`,
`FeetOffset`,
`FGCachedConnectedWire`,
`FGDroneFuelRuntimeData`,
`FGPortalCachedFactoryTickData`,
`FloatInterval`,
`FloatRange`,
`FloatRangeBound`,
`FoliageRemovalSaveDataForFoliageType`,
`FoliageRemovalSaveDataPerCell`,
`FoliageRemovalUnresolvedSaveDataPerCell`,
`FoundationSideSelectionFlags`,
`GCheckmarkUnlockData`,
`GlobalColorPreset`,
`HardDriveData`,
`HeadlightParams`,
`HighlightedMarkerPair`,
`Hotbar`,
`InstanceData`,
`InventoryStack`,
`InventoryToRespawnWith`,
`ItemAmount`,
`ItemFoundData`,
`KAggregateGeom`,
`KConvexElem`,
`LBBalancerData` (Mod LoadBalancers),
`LightSourceControlData`,
`LocalUserNetIdBundle`,
`MapMarker`,
`MappedItemAmount`,
`MaterialCachedExpressionData`,
`MaterialInstanceBasePropertyOverrides`,
`MaterialParameterInfo`,
`MeshUVChannelInfo`,
`MessageData`,
`MiniGameResult`,
`ParticleMap`,
`PersistentGlobalIconId`,
`PhaseCost`,
`PlayerCustomizationData`,
`PlayerRules`,
`PlayerStateSetWrapper`,
`PointerToUberGraphFrame`,
`PrefabIconElementSaveData`,
`PrefabTextElementSaveData`,
`ProjectAssemblyLaunchSequenceValue`,
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
`ShoppingListClassEntry`,
`ShoppingListSettings`,
`SpawnData`,
`SplinePointData`,
`SplitterSortRule`,
`StaticMaterial`,
`StaticParameterSet`,
`StaticSwitchParameter`,
`StreamingTextureBuildInfo`,
`StringPair`,
`SubCategoryMaterialDefault`,
`SwatchGroupData`,
`SwitchData`,
`TextureParameterValue`,
`TimerHandle`,
`TimeTableStop`,
`TireTrackDecalDetails`,
`TopLevelAssetPath`,
`TrainDockingRuleSet`,
`TrainSimulationData`,
`Transform`,
`Vector_NetQuantize`,
`WeightmapLayerAllocationInfo`,
`WireInstance`,
`WorldPartitionRuntimeCellDebugInfo`

As property structs seem more common than binary structs, save game parsers may just try to parse unknown structs as property structs.

### Binary Structs

The following structs are binary structs with the type described in the table:

| Struct Name             | Type                     | Description                                                            | Notes               |
|-------------------------|--------------------------|------------------------------------------------------------------------|---------------------|
| `Box`                   | `FBox`                   | `FVector Min`<br>`FVector Max`<br>`uint8 IsValid`                      |                     |
| `ClientIdentityInfo`    | `FClientIdentityInfo`    | `FString OfflineId`<br>`TMap<uint8, TArray<uint8>> AccountIds`         |                     |
| `Color`                 | `FColor`                 | `uint8 B`<br>`uint8 G`<br>`uint8 R`<br>`uint8 A`                       |                     |
| `FluidBox`              | `FFluidBox`              | `float Value`                                                          |                     |
| `Guid`                  | `FGuid`                  | [FGuid](#fguid)                                                        |                     |
| `IntPoint`              | `FIntPoint`              | `int32 X`<br>`int32 Y`                                                 |                     |
| `IntVector`             | `FIntVector`             | `int32 X`<br>`int32 Y`<br>`int32 Z`                                    |                     |
| `InventoryItem`         | `FInventoryItem`         | [FInventoryItem](#finventoryitem)                                      |                     |
| `LBBalancerIndexing`    | `FLBBalancerIndexing`    | `int32 mNormalIndex`<br>`int32 mOverflowIndex`<br>`int32 mFilterIndex` | (Mod LoadBalancers) |
| `LinearColor`           | `FLinearColor`           | `float R`<br>`float G`<br>`float B`<br>`float A`                       |                     |
| `Quat`                  | `FQuat`                  | `double X`<br>`double Y`<br>`double Z`<br>`double W`                   |                     |
| `RailroadTrackPosition` | `FRailroadTrackPosition` | `FObjectReferenceDisc Track`<br>`float Offset`<br>`float Forward`      |                     |
| `Rotator`               | `FRotator`               | `double Pitch`<br>`double Yaw`<br>`double Roll`                        |                     |
| `SoftClassPath`         | `FSoftObjectPath`        | TODO                                                                   |                     |
| `Vector2D`              | `FVector2D`              | `double X`<br>`double Y`                                               |                     |
| `Vector4`               | `FVector4`               | `double X`<br>`double Y`<br>`double Z`<br>`double W`                   |                     |
| `Vector`                | `FVector`                | `double X`<br>`double Y`<br>`double Z`                                 |                     |

## Type and Object Reference

### Basic types

Integer types are all named by their size in bits, e.g. int8, int32, int64, uint8, uint32, uint64, etc.
Floating point numbers are either defined as float (32-bit) or double (64-bit).
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
In addition, variants templating the size exist, i.e., using int64 `TArray<T, int64>`:

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

TMap is a template, and the size of a single key and value items is defined by the underlying type.

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

> [Unreal Docs](https://docs.unrealengine.com/en-US/API/Runtime/Core/Misc/FGuid/index.html)

#### FMD5Hash

```
+---------------+----------+
| bool          | bIsValid |
| if bIsValid:  |          |
|     uint8[16] | Bytes    |
+---------------+----------+
```

#### FDateTime

```
+-------+-------+
| int64 | Ticks |
+-------+-------+
```

[Unreal Docs](https://docs.unrealengine.com/en-US/API/Runtime/Core/Misc/FDateTime/index.html)  
Ticks since 0001-01-01 00:00, where 1 tick is 100 nanoseconds. Satisfactory seems to use the UTC time zone.

### Satisfactory Objects

#### FObjectReferenceDisc

Another common type used within the save data is `FObjectReferenceDisc`, defined in the following way:

```
+---------+------------+
| FString | level name |
| FString | path name  |
+---------+------------+
```

- Satisfactory internal struct, header file: `FGObjectReference.h`

#### FFGDynamicStruct

```
+--------------------------+------------------+
| bool                     | bHasValidStruct  |
| if bHasValidStruct:      |                  |
|     FObjectReferenceDisc | ScriptStruct     |
|     int32                | savedPayloadSize | (size of StructInstance)
|     List of Properties   | StructInstance   |
+--------------------------+------------------+
```

> Unreal archives internally can select binary, native, or tagged property serialization for the content of StructInstance, see
> https://github.com/EpicGames/UnrealEngine/blob/5.3.2-release/Engine/Source/Runtime/CoreUObject/Private/UObject/Class.cpp#L2761.
> Here, it is assumed that the game will always use tagged properties.

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
| if SaveVersion >= 51:        |                 | (SaveVersion from SaveHeader)
|     int32                    | SaveVersion     | (this SaveVersion is used for parsing TOCBlob64 and DataBlob64)
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
+-----------------------+-------------+
| FString               | ClassName   |
| FObjectReferenceDisc  | Reference   |
| if SaveVersion >= 49: |             |
|     uint32            | ObjectFlags |
+-----------------------+-------------+
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
+----------+-------------+-------------------------------------------+
| float[4] | Rotation    | quaternion describing object rotation     |
| float[3] | Translation | world position of object (in centi meter) |
| float[3] | Scale3D     | object scale                              |
+----------+-------------+-------------------------------------------+
```

#### FUnresolvedWorldSaveData

```
+------------------------------+-----------------+
| TArray<FObjectReferenceDisc> | DestroyedActors |
+------------------------------+-----------------+
```

#### FInventoryItem

```
+--------------------------+----------------------+
| FObjectReferenceDisc     | ItemClass            |
| if SaveVersion >= 43:    |                      | (see SaveVersion from parent object)
|     FFGDynamicStruct     | ItemState            |
| else:                    |                      |
|     FObjectReferenceDisc | LegacyItemStateActor |
+--------------------------+----------------------+
```

> The internal type for ItemClass is `TSubclassOf<class UFGItemDescriptor>`, which is serialized as `FObjectReferenceDisc`.

#### FConveyorBeltItem

```
+----------------+--------+
| FInventoryItem | Item   |
| float          | Offset |
+----------------+--------+
```

#### FConveyorBeltItems

```
+---------------------------+-------+
| TArray<FConveyorBeltItem> | Items |
+---------------------------+-------+
```

#### FConveyorChainSplineSegment

```
+--------------------------+-------------------+
| FObjectReferenceDisc     | ChainActor        |
| FObjectReferenceDisc     | ConveyorBase      |
| TArray<FSplinePointData> | SplinePointData   |
| float                    | OffsetAtStart     |
| float                    | StartsAtLength    |
| float                    | EndsAtLength      |
| int32                    | FirstItemIndex    |
| int32                    | LastItemIndex     |
| int32                    | IndexInChainArray |
+--------------------------+-------------------+
```

#### FSplinePointData

```
+---------+---------------+
| FVector | Location      |
| FVector | ArriveTangent |
| FVector | LeaveTangent  |
+---------+---------------+
```

#### FRuntimeBuildableInstanceData

```
+------------------------------------+----------------------------------------------------+
| FTransform                         | Transform                                          |
| FObjectReferenceDisc               | CustomizationData.SwatchDesc                       |
| FObjectReferenceDisc               | CustomizationData.MaterialDesc                     |
| FObjectReferenceDisc               | CustomizationData.PatternDesc                      |
| FObjectReferenceDisc               | CustomizationData.SkinDesc                         |
| FLinearColor                       | CustomizationData.OverrideColorData.PrimaryColor   |
| FLinearColor                       | CustomizationData.OverrideColorData.SecondaryColor |
| FObjectReferenceDisc               | CustomizationData.OverrideColorData.PaintFinish    |
| uint8                              | CustomizationData.PatternRotation                  |
| FObjectReferenceDisc               | BuiltWithRecipe                                    |
| FObjectReferenceDisc               | BlueprintProxy                                     |
| if currentLightweightVersion >= 2: |                                                    | currentLightweightVersion is a member from the parent AFGLightweightBuildableSubsystem
|     FFGDynamicStruct               | TypeSpecificData                                   |
+------------------------------------+----------------------------------------------------+
```

`FTransform` is defined as:

```
+-----------+-------------+-------------------------------------------+
| double[4] | Rotation    | quaternion describing object rotation     |
| double[3] | Translation | world position of object (in centi meter) |
| double[3] | Scale3D     | object scale                              |
+-----------+-------------+-------------------------------------------+
```

#### FUniqueNetIdRepl

```
+------------------------------------------+--------------------+
| uint8                                    | EncodingFlags      |
| if EncodingFlags & 1:                    |                    |
|     if !EncodingFlags & 2:               |                    |
|         if EncodingFlags >> 3 == 30:     |                    |
|             uint8                        | OnlineServicesType |
|             TArray<uint8>                | ReplicationData    |
|         else:                            |                    |
|             if EncodingFlags >> 3 == 31: |                    |
|                 FString                  | TypeString         |
|             if EncodingFlags >> 3 != 0:  |                    |
|                 uint8                    | EncodedSize        |
|                 uint8[EncodedSize]       | TempBytes          |
| else:                                    |                    |
|     if EncodingFlags >> 3 == 31:         |                    |
|         FString                          | TypeString         |
|     if EncodingFlags >> 3 != 0:          |                    |
|         FString                          | Contents           |
+------------------------------------------+--------------------+
```

#### FVehiclePhysicsData

```
+---------+----------------------+
| FName   | BoneName             |
| FVector | BodyState.Position   |
| FQuat   | BodyState.Quaternion |
| FVector | BodyState.AngVel     |
| FVector | BodyState.LinVel     |
| uint8   | BodyState.Flags      |
+---------+----------------------+
```

#### FTopLevelAssetPath

```
+-------+-------------+
| FName | PackageName |
| FName | AssetName   |
+-------+-------------+
```

#### FBlueprintItemAmount

```
+----------------------+-----------+
| FObjectReferenceDisc | ItemClass |
| int32                | Amount    |
+----------------------+-----------+
```

#### FLocalUserNetIdBundle

```
+---------+---------------------------+
| FString | AssociatedAccountIdString |
| FString | DisplayName               |
| FString | BackendName               |
+---------+---------------------------+
```

## Blueprints

Blueprints consist of two files, a config file (`*.sbpcfg`) and the blueprint file itself (`*.sbp`).

### BlueprintCfg File

```
+-----------------------------------+----------------------+
| int32                             | ConfigVersion        |
| FString                           | BlueprintDescription |
| int32                             | IconID.IconID        |
| FLinearColor                      | Color                |
| if ConfigVersion >= 3:            |                      |
|     FTopLevelAssetPath            | IconID.IconLibrary   |
| if ConfigVersion >= 4:            |                      |
|     TArray<FLocalUserNetIdBundle> | LastEditedBy         |
+-----------------------------------+----------------------+
```

This documentation is only valid if `ConfigVersion` equals `2`, `3`, or `4`!
`IconID` is a struct named `FPersistentGlobalIconId`, but serialized element wise dependent on `ConfigVersion`.

> The data except `ConfigVersion` is internally stored as struct `FBlueprintRecord`.

### Blueprint File

The blueprint file is very similar to the [General file structure](#general-file-structure) of the save file format.
There is a blueprint header followed by binary data, which is stored using the same compressed chunks as in save games above.

#### Blueprint Header

```
+------------------------------+---------------+
| int32                        | HeaderVersion |
| int32                        | SaveVersion   |
| int32                        | BuildVersion  |
| FIntVector                   | Dimensions    |
| TArray<FBlueprintItemAmount> | Cost          |
| TArray<FObjectReferenceDisc> | RecipeRefs    |
+------------------------------+---------------+
```

This documentation is only valid if `HeaderVersion` equals `2`!

> The data except `HeaderVersion` is internally stored as struct `FBlueprintHeader`.

#### Blueprint Binary Data

The blueprint binary data is similar to the structure of save games, but slightly different.
Here, the format is described very shortly with focusing on differences.
More detailed information can be found in the [Decompressed binary data](#decompressed-binary-data) section above.

The binary data layout follows the following format:

```
+----------------------+------------------------------------------------------+
| int32                | total size of binary data (not including this value) |
| TArray<uint8, int32> | TOCData                                              |
| TArray<uint8, int32> | BlobData                                             |
+----------------------+------------------------------------------------------+
```

Objects within a blueprint are stored in similar TOC/Data blob like objects in a save game level.

##### Blueprint TOCData

```
+-----------------------------------------------------+---------------------------+
| int32                                               | numObjects                |
| for i = 1 to numObjects:                            |                           |
|     bool                                            | isActor                   |
|     if isActor:                                     |                           |
|         FActorSaveHeader                            | objectHeader              |
|     else:                                           |                           |
|         FObjectSaveHeader                           | objectHeader              |
+-----------------------------------------------------+---------------------------+
```

##### Blueprint BlobData

```
+--------------------------+-------------------------------------+
| int32                    | numObjects                          |
| for i = 1 to numObjects: |                                     |
|     TArray<uint8>        | Data                                |
+--------------------------+-------------------------------------+
```

For parsing of `Data`, see [Objects](#objects).
