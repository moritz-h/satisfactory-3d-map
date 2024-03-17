from pathlib import Path

import satisfactory_save as s


def print_property(p: s.Property, indent=1):
    space = '  ' * indent
    print(f'{space}{p.Name.toString()} ({p.Type.toString()})')
    if isinstance(p, s.FloatProperty):
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


def print_object(obj: s.SaveObjectBase):
    print(f'TOC:')
    print(f'  isActor: {obj.isActor()}')
    print(f'  ClassName: {obj.ClassName}')
    print(f'  Reference.LevelName: {obj.Reference.LevelName}')
    print(f'  Reference.PathName: {obj.Reference.PathName}')
    if obj.isActor():
        print(f'  NeedTransform: {obj.NeedTransform}')
        rot = obj.Transform.Rotation
        trans = obj.Transform.Translation
        scale = obj.Transform.Scale3D
        print(f'  Transform.Rotation: [X:{rot.X} Y:{rot.Y} Z:{rot.Z} W:{rot.W}]')
        print(f'  Transform.Translation: [X:{trans.X} Y:{trans.Y} Z:{trans.Z}]')
        print(f'  Transform.Scale3D: [X:{scale.X} Y:{scale.Y} Z:{scale.Z}]')
        print(f'  WasPlacedInLevel: {obj.WasPlacedInLevel}')
    else:
        print(f'  OuterPathName: {obj.OuterPathName}')
    print(f'Data:')
    print(f'  unk1: {obj.unk1}')
    print(f'  unk2: {obj.unk2}')
    if obj.isActor():
        print(f'  parent_reference.LevelName: {obj.parent_reference.LevelName}')
        print(f'  parent_reference.PathName: {obj.parent_reference.PathName}')
        print(f'  child_references: {list(obj.child_references)}')
    print(f'  Properties:')
    for p in obj.Properties:
        print_property(p, 2)
    print(f'  HasGuid: {obj.HasGuid}')
    if obj.HasGuid:
        print(f'  Guid: {obj.Guid}')
    print(f'  ExtraProperties: {obj.ExtraProperties}')


# Load Save
save = s.SaveGame(Path('test.sav'))

# Iterate path nodes to get 8x4 foundations
node = save.allRootNode()
for x in ['Game', 'FactoryGame', 'Buildable', 'Building', 'Foundation', 'Build_Foundation_8x4_01.Build_Foundation_8x4_01_C']:
    node = node.childNodes[x]

# Print info
print('=== Object Info ===')
print_object(node.objects[0])
