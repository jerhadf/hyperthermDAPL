# CREATE A JSON FILE FOR THE AUTONEST STRATEGIES

strategies = '''
0	Rectangular
1	Rectangular Optimization
2	Strategy 1
3	Strategy 2
4	Strategy 3
5	Strategy 4
6	Strategy 5
7	Strategy 6
8	Strategy 7
9	Strategy 8
10	Strategy 9
11	Strategy 10
12	IntelliChoice
'''
# using the list of strategies above, convert the list to a json file called AutoNestStrategy.json 
# and save it in the same directory as this script

import json 

# convert the string to a list (split on newlines) 
strategies = strategies.split('\n')

# strip whitespace, formatting, tabs, remove empty lists 
strategies = [x.strip() for x in strategies if x != '']

# split each list item on \t (the tab character)
strategies = [x.split('\t') for x in strategies]

# replace the spaces in the values with underscores
strategies = [[x[0], x[1].replace(' ', '_')] for x in strategies]

# convert the list of lists to a dictionary
strategies = dict(strategies)
print(strategies)

# convert the dictionary to a json file
with open('AutoNestStrategy.json', 'w') as f:
    json.dump(strategies, f, indent=4)
    
# CREATE A JSON FILE FOR THE MATERIAL TYPES
# read in the material.csv file and convert it to a json file called MaterialTypes.json
# and save it in the same directory as this script

# read in the material.csv as a DataFrame
import pandas as pd
df = pd.read_csv('material.csv')

# convert the dataframe to a dictionary
material_types = df.to_dict(orient='records')