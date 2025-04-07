from pathlib import Path

import satisfactory_save as s


def print_property(p: s.Property, indent=1):
    space = '  ' * indent
    print(f'{space}{p.Name.toString()} ({p.Type.toString()})')
    if isinstance(p, s.FloatProperty):
        print(f'{space}  {p.Value}')
    elif isinstance(p, s.IntProperty):
        print(f'{space}  {p.Value}')
    elif isinstance(p, s.ObjectProperty):
        print(f'{space}  Lvl: {p.Value.LevelName}')
        print(f'{space}  Path: {p.Value.PathName}')
    elif isinstance(p, s.StructProperty):
        print(f'{space}  StructName: {p.StructName.toString()}')
        struct = p.Value
        if isinstance(struct, s.PropertyStruct):
            for struct_member in struct.Data:
                print_property(struct_member, indent + 2)
        else:
            print(f'{space}    -- todo --')
    else:
        print(f'{space}  -- todo --')


def print_object(obj: s.SaveObject):
    print(f'TOC:')
    print(f'  isActor: {obj.isActor()}')
    print(f'  ClassName: {obj.BaseHeader.ClassName}')
    print(f'  Reference.LevelName: {obj.BaseHeader.Reference.LevelName}')
    print(f'  Reference.PathName: {obj.BaseHeader.Reference.PathName}')
    print(f'  ObjectFlags: {obj.BaseHeader.ObjectFlags}')
    if obj.isActor():
        print(f'  NeedTransform: {obj.Header.NeedTransform}')
        rot = obj.Header.Transform.Rotation
        trans = obj.Header.Transform.Translation
        scale = obj.Header.Transform.Scale3D
        print(f'  Transform.Rotation: [X:{rot.X} Y:{rot.Y} Z:{rot.Z} W:{rot.W}]')
        print(f'  Transform.Translation: [X:{trans.X} Y:{trans.Y} Z:{trans.Z}]')
        print(f'  Transform.Scale3D: [X:{scale.X} Y:{scale.Y} Z:{scale.Z}]')
        print(f'  WasPlacedInLevel: {obj.Header.WasPlacedInLevel}')
    else:
        print(f'  OuterPathName: {obj.Header.OuterPathName}')
    print(f'Data:')
    print(f'  SaveVersion: {obj.SaveVersion}')
    print(f'  ShouldMigrateObjectRefsToPersistent: {obj.ShouldMigrateObjectRefsToPersistent}')
    if obj.isActor():
        print(f'  Owner.LevelName: {obj.Object.Owner.LevelName}')
        print(f'  Owner.PathName: {obj.Object.Owner.PathName}')
        print(f'  Components: {list(obj.Object.Components)}')
    print(f'  Properties:')
    for p in obj.Object.Properties:
        print_property(p, 2)
    print(f'  Guid: {obj.Object.Guid}')
    print(f'  BinaryClassData: {obj.BinaryClassData}')


# Load Save
save = s.SaveGame(Path('test.sav'))

objects = save.getObjectsByClass('/Game/FactoryGame/Buildable/Building/Ladder/Build_Ladder.Build_Ladder_C')

# Print info
print('=== Object Info ===')
print_object(objects[0])
