# CREATE A JSON FILE FOR THE MATERIAL TYPES
# read in the material.csv file and convert it to a json file called MaterialTypes.json
# and save it in the same directory as this script

# read in the material.csv as a dataframe
import pandas as pd
import json
df = pd.read_csv('CEIP_csv/Material.csv')

# print the head of the df
# print(df.head())

# count the number of non-NaN values in each column (see results below)
# print(df.count()) 
# ixMaterial - 16454
# sName - 16453
# dThickness - 16454
# sGrade - 4834

# drop the rows where sName is NaN
df = df.dropna(subset=['sName'])

# convert all the sName names to lowercase
df['sName'] = df['sName'].str.lower()

# convert 'ixMaterial' to numeric
df['ixMaterial'] = pd.to_numeric(df['ixMaterial'], errors='coerce')

# extract the ixMaterial number value, and make it the index of the dictionary 
df = df.set_index('ixMaterial')

# convert the dataframe to a dictionary
material_types = df.to_dict(orient='index')  # use 'index' instead of 'records'

# combine the items where the sName is the same and average the dThickness
# keep the sName so it's still present in the values of the dictionary
# df = df.groupby('sName').mean()

# alternative: combine the values where dThickness is within 0.1, average them 

# # # save the dictionary as a json file
with open('CEIP_csv/MaterialTypes.json', 'w') as f:
    json.dump(material_types, f, indent=4)
    
# Load the JSON file back into a Python dictionary
with open('CEIP_csv/MaterialTypes.json', 'r') as f:
    material_types = json.load(f)

# Convert string keys back to integers
material_types = {int(k): v for k, v in material_types.items()}

# Find items with 'steel' in the sName
steel_items = [v for k, v in material_types.items() if 'steel' in v['sName']]

# Find indices of items with 'steel' in the sName
steel_indices = [k for k, v in material_types.items() if 'steel' in v['sName']]

print(steel_items)
print(steel_indices)
print(f"There are {len(steel_items)} items with 'steel' in the sName.")
# 720 items with steel in the name 

# save the steel items dictionary as a json file
with open('CEIP_csv/SteelMaterials.json', 'w') as f:
    json.dump(steel_items, f, indent=4)