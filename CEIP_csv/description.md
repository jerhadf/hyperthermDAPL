# CEIP_csv folder description
This folder contains the csv files exported from the CEIP database using SQL queries. 
The names of each .csv file matches the name of the table in the database exactly. 
For example, the file `AutoNest.csv` contains the data from the `dbo.AutoNest` table in the database. This data was selected using the SQL query `SELECT * FROM dbo.AutoNest`. 

* `AutoNest.csv` - from `dbo.AutoNest` table, automatic nesting data
* `AutoNestStrategy.csv` - from `dbo.AutoNestStrategy` table, connected to the AutoNest table via the ixAutoNestStrategy column, tells us what the different automatic nesting strategy numbers mean 
  * A JSON of these strategies is in the `AutoNestStrategy.json` file, where the values are the number IDs of the strategies (corresponding to the `ixAutoNestStrategy` column in the AutoNest table) and the keys are the names of the strategies.
* `Material.csv` - from the `dbo.Material` table, shows what each of the `ixMaterial` IDs mean 
  * This is converted to a JSON as `MaterialTypes.json`, with the steel materials as `SteelMaterials.json`
* `Nest.csv` - from `dbo.Nest` table, nesting data
* `Part.csv` - from `dbo.Part` table, part data
* `Performance.csv` - from `dbo.Performance` table, data on performance of the CEIP program and its data collection, NOT used for any future analysis or features. 