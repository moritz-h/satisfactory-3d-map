from pathlib import Path

import satisfactory_save as s

# Blueprint Config
print("=== Blueprint Config ===")
blueprint_cfg = s.BlueprintCfg(Path('test.sbpcfg'))
r = blueprint_cfg.record
print(f"ConfigVersion: {r.ConfigVersion}")
print(f"BlueprintDescription: {r.BlueprintDescription}")
print(f"IconID: {r.IconID}")
print(f"Color: {r.Color.R} {r.Color.G} {r.Color.B} {r.Color.A}")
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
print("Objects:")
for obj in blueprint.saveObjects:
    print(f"  {obj.ClassName}")
