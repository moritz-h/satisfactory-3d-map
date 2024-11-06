from pathlib import Path

import satisfactory_save as s

# Load Save
save = s.SaveGame(Path('test.sav'))

# Pyramid Settings
center = (-60000.0, 240000.0, 0.0)
size = 20

class_name = '/Game/FactoryGame/Buildable/Building/Foundation/Build_Foundation_8x4_01.Build_Foundation_8x4_01_C'

lbs = save.getObjectsByPath('Persistent_Level:PersistentLevel.LightweightBuildableSubsystem')[0].Object
instance_data_list = lbs.mBuildableClassToInstanceArray[s.FObjectReferenceDisc('', class_name)]

for idx_z in range(size):
    z = center[2] + idx_z * 400.0
    for idx_y in range(-size + 1, size):
        if abs(idx_y) >= size - idx_z:
            continue
        y = center[1] + idx_y * 800.0
        for idx_x in range(-size + 1, size):
            if abs(idx_x) >= size - idx_z:
                continue
            x = center[0] + idx_x * 800.0

            # Make new foundation
            f = s.FRuntimeBuildableInstanceData()
            f.Transform.Rotation = (0.0, 0.0, 0.0, 1.0)
            f.Transform.Translation = (x, y, z)
            f.Transform.Scale3D = (1.0, 1.0, 1.0)
            f.CustomizationData.SwatchDesc.PathName = '/Game/FactoryGame/Buildable/-Shared/Customization/Swatches/SwatchDesc_Slot16.SwatchDesc_Slot16_C'
            f.CustomizationData.MaterialDesc.PathName = ''
            f.CustomizationData.PatternDesc.PathName = ''
            f.CustomizationData.SkinDesc.PathName = ''
            f.CustomizationData.OverrideColorData.PrimaryColor = (0.0, 0.0, 0.0, 1.0)
            f.CustomizationData.OverrideColorData.SecondaryColor = (0.0, 0.0, 0.0, 1.0)
            f.CustomizationData.OverrideColorData.PaintFinish.PathName = ''
            f.CustomizationData.PatternRotation = 0
            f.BuiltWithRecipe.PathName = '/Game/FactoryGame/Recipes/Buildings/Foundations/Recipe_Foundation_8x4_01.Recipe_Foundation_8x4_01_C'
            f.BlueprintProxy.LevelName = ''
            f.BlueprintProxy.PathName = ''

            instance_data_list.append(f)

# Save edited save
save.save('test_edit.sav')
