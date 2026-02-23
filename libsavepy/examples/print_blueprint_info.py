from pathlib import Path

import satisfactory_save as s

# Blueprint Config
print("=== Blueprint Config ===")
blueprint_cfg = s.BlueprintCfg(Path('test.sbpcfg'))
r = blueprint_cfg.record
print(f"ConfigVersion: {r.ConfigVersion}")
print(f"BlueprintDescription: {r.BlueprintDescription}")
print(f"IconID.IconLibrary.PackageName: {r.IconID.IconLibrary.PackageName.toString()}")
print(f"IconID.IconLibrary.AssetName: {r.IconID.IconLibrary.AssetName.toString()}")
print(f"IconID.IconID: {r.IconID.IconID}")
print(f"Color: {r.Color.R} {r.Color.G} {r.Color.B} {r.Color.A}")
print(f"LastEditedBy_v4 [size: {len(r.LastEditedBy_v4)}]:")
for i in r.LastEditedBy_v4:
    print(f"  AssociatedAccountIdString: {i.AssociatedAccountIdString} DisplayName: {i.DisplayName} BackendName: {i.BackendName}")
print(f"LastEditedBy_v6  ServiceProvider: {r.LastEditedBy_v6.ServiceProvider} PlayerInfoTableIndex: {r.LastEditedBy_v6.PlayerInfoTableIndex}")
print()

# Blueprint
print("=== Blueprint ===")
blueprint = s.Blueprint(Path('test.sbp'))
h = blueprint.header
print(f"HeaderVersion: {h.HeaderVersion}")
print(f"SaveVersion: {h.SaveVersion}")
print(f"BuildVersion: {h.BuildVersion}")
print(f"Dimensions: {h.Dimensions.X} {h.Dimensions.Y} {h.Dimensions.Z}")
print("Cost:")
for c in h.Cost:
    print(f"  {c.ItemClass.LevelName} {c.ItemClass.PathName} {c.Amount}")
print("RecipeRefs:")
for r in h.RecipeRefs:
    print(f"  {r.LevelName} {r.PathName}")
print("SaveObjectVersionData:")
print(f"  SaveObjectVersionDataVersion: {h.SaveObjectVersionData.SaveObjectVersionDataVersion}")
print(f"  PackageFileVersion.FileVersionUE4: {h.SaveObjectVersionData.PackageFileVersion.FileVersionUE4}")
print(f"  PackageFileVersion.FileVersionUE5: {h.SaveObjectVersionData.PackageFileVersion.FileVersionUE5}")
print(f"  LicenseeVersion: {h.SaveObjectVersionData.LicenseeVersion}")
print(f"  EngineVersion.Major: {h.SaveObjectVersionData.EngineVersion.Major}")
print(f"  EngineVersion.Minor: {h.SaveObjectVersionData.EngineVersion.Minor}")
print(f"  EngineVersion.Patch: {h.SaveObjectVersionData.EngineVersion.Patch}")
print(f"  EngineVersion.Changelist: {h.SaveObjectVersionData.EngineVersion.Changelist}")
print(f"  EngineVersion.Branch: {h.SaveObjectVersionData.EngineVersion.Branch}")
print("  CustomVersionContainer.Versions:")
for v in h.SaveObjectVersionData.CustomVersionContainer.Versions:
    print(f"    {v.Key.toString()}: {v.Version}")
print("Objects:")
for obj in blueprint.saveObjects:
    print(f"  {obj.BaseHeader.ClassName}")
