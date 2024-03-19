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
```

As a high-level overview to motivate the library interface, a save game could be seen as mainly consisting of objects stored in several levels.
Each object is of a specific class, which is identified by a class name, which could be interpreted similarly to a file system path, e.g., a foundation is of the class `/Game/FactoryGame/Buildable/Building/Foundation/Build_Foundation_8x4_01.Build_Foundation_8x4_01_C`.
For further details about the save game structure, please refer to the detailed documentation [here](https://github.com/moritz-h/satisfactory-3d-map/blob/master/docs/SATISFACTORY_SAVE.md).

To get a list of objects, the following methods exist:

```python
# List of levels with save objects
save.perLevelData()[0].save_objects

# List of objects of the main level (including buildings)
save.persistentAndRuntimeData().save_objects

# Unified list of all objects
save.allSaveObjects().save_objects
```

To access the objects by class in a file-system-like interface, the following methods exist:

```python
# Return a list of root nodes for all levels
save.levelRootNodes()

# Return the root node for the main level
save.persistentAndRuntimeRootNode()

# Return the root node of all objects
save.allRootNode()
```

Each node has the following interface:

```python
node.childNodes  # Map using the "directory" name as key and nodes as value
node.objects     # List of save objects
```

Objects can be added and removed to a save game:

```python
# Add object, a level must be provided, -1 refers to the main level
save.addObject(object, level=-1)

# Remove object from save game
save.removeObject(object)
```

Adding and removing objects is computationally expensive because the file-system-like structures are rebuilt every time.
To overcome this limitation, an interface to add or remove lists of objects exists:

```python
save.addObjects(objects, level=-1)
save.removeObjects(objects)
```

For more details about creating objects and adding properties, some examples are available [here](https://github.com/moritz-h/satisfactory-3d-map/tree/master/libsavepy/examples).
