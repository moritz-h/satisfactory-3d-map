# Satisfactory Save Library

A library to parse and edit save games of the game [Satisfactory](https://www.satisfactorygame.com/)
by Coffee Stain Studios.

# Install

```shell
pip install satisfactory-save
```

# Usage

```python
import satisfactory_save as s

# Load a save game
save = s.SaveGame('test.sav')

# Store a save game
save.save('test_edit.sav')

# Get save header, e.g., SessionName
print(save.mSaveHeader.SessionName)
```

A save game could be seen as mainly consisting of objects stored in several levels.
Each object is of a specific class, which is identified by a class name, e.g., a foundation is of the class `/Game/FactoryGame/Buildable/Building/Foundation/Build_Foundation_8x4_01.Build_Foundation_8x4_01_C`.
For further details about the save game structure, please refer to the detailed documentation [here](https://github.com/moritz-h/satisfactory-3d-map/blob/master/docs/SATISFACTORY_SAVE.md).

To access the save objects, the following methods exist:

```python
# List of objects of the main level (including buildings)
save.mPersistentAndRuntimeData.SaveObjects

# List of other level save objects
for i in range(len(save.mPerLevelDataMap)):
    level_name = save.mPerLevelDataMap.Keys[i]
    level_objects = save.mPerLevelDataMap.Values[i].SaveObjects

# Unified list of all objects
save.allSaveObjects()
```

To access the objects by class name, the following methods exist:

```python
save.isObjectClass('/Class/Name')
save.getObjectsByClass('/Class/Name')
```

To access the objects by path name, the following methods exist:

```python
save.isObjectPath('Path_Name')
save.getObjectsByPath('Path_Name')
```

Objects can be added and removed to a save game:

```python
# Add object, a level must be provided, -1 refers to the main level
save.addObject(object, level=-1)

# Remove object from save game
save.removeObject(object)
```

Adding and removing objects is computationally expensive because internal structures are rebuilt every time.
To overcome this limitation, an interface to add or remove lists of objects exists:

```python
save.addObjects(objects, level=-1)
save.removeObjects(objects)
```

For more details about creating objects and adding properties, some examples are available [here](https://github.com/moritz-h/satisfactory-3d-map/tree/master/libsavepy/examples).
