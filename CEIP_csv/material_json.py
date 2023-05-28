# CREATE A JSON FILE FOR THE MATERIAL TYPES
# read in the material.csv file and convert it to a json file called MaterialTypes.json
# and save it in the same directory as this script

# read in the material.csv as a dataframe
import pandas as pd
df = pd.read_csv('material.csv')

# value counts 


# convert the dataframe to a dictionary
material_types = df.to_dict(orient='records')

