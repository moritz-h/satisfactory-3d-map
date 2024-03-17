from pathlib import Path

import satisfactory_save as s


def make_foundation():
    f = s.SaveActor()
    f.ClassName = '/Game/FactoryGame/Buildable/Building/Foundation/Build_Foundation_8x4_01.Build_Foundation_8x4_01_C'
    f.Reference.LevelName = 'Persistent_Level'
    f.Reference.PathName = 'Persistent_Level:PersistentLevel.Build_Foundation_8x4_01_C_1234567890'
    f.NeedTransform = True
    f.WasPlacedInLevel = False
    f.Transform.Rotation = (0.0, 0.0, 0.0, 1.0)
    f.Transform.Translation = (0.0, 0.0, 0.0)
    f.Transform.Scale3D = (1.0, 1.0, 1.0)
    f.unk1 = 42
    f.unk2 = 0
    f.parent_reference = s.FObjectReferenceDisc('Persistent_Level', 'Persistent_Level:PersistentLevel.BuildableSubsystem')

    p1 = s.StructProperty('mCustomizationData', s.PropertyStruct('FactoryCustomizationData'))
    p1.StructName = 'FactoryCustomizationData'
    p1a = s.ObjectProperty('SwatchDesc')
    p1a.Value.PathName = '/Game/FactoryGame/Buildable/-Shared/Customization/Swatches/SwatchDesc_Slot16.SwatchDesc_Slot16_C'
    p1.Value.Data.append(p1a)

    p2 = s.ObjectProperty('mBuiltWithRecipe')
    p2.Value.PathName = '/Game/FactoryGame/Recipes/Buildings/Foundations/Recipe_Foundation_8x4_01.Recipe_Foundation_8x4_01_C'

    p3 = s.FloatProperty('mBuildTimeStamp', 0.0)

    f.Properties.append(p1)
    f.Properties.append(p2)
    f.Properties.append(p3)

    return f


# Load Save
save = s.SaveGame(Path('test.sav'))

# Pyramid Settings
center = (-60000.0, 240000.0, 0.0)
size = 20

next_id = 1234567890
objects = []

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

            # Make new foundation actor
            f = make_foundation()
            f.Transform.Translation = (x, y, z)

            # PathName must be unique
            name = f'Persistent_Level:PersistentLevel.Build_Foundation_ConcretePolished_8x4_C_{next_id}'
            while save.isObjectPath(name):
                next_id += 1
                name = f'Persistent_Level:PersistentLevel.Build_Foundation_ConcretePolished_8x4_C_{next_id}'
            next_id += 1
            f.Reference.PathName = name

            objects.append(f)

# Add objects to save
status = save.addObjects(objects)
print(status)

# Save edited save
save.save('test_edit.sav')
