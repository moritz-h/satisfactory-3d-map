from pathlib import Path

import satisfactory_save as s

# Load Save
save = s.SaveGame(Path('test.sav'))

# Iterate path nodes to get 8x4 foundations
node = save.allRootNode()
for x in ['Game', 'FactoryGame', 'Buildable', 'Building', 'Foundation', 'Build_Foundation_8x4_01.Build_Foundation_8x4_01_C']:
    node = node.childNodes[x]

# Remove all foundations objects
status = save.removeObjects(node.objects)
print(status)

# Save edited save
save.save('test_edit.sav')
