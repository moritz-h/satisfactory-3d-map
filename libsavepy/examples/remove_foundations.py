from pathlib import Path

import satisfactory_save as s

# Load Save
save = s.SaveGame(Path('test.sav'))

objects = save.getObjectsByClass('/Game/FactoryGame/Buildable/Building/Foundation/Build_Foundation_8x4_01.Build_Foundation_8x4_01_C')

# Remove all foundations objects
status = save.removeObjects(objects)
print(status)

# Save edited save
save.save('test_edit.sav')
