CREATE PROCEDURE generate_utilization_model (@trained_model VARBINARY(max) OUTPUT)
AS
BEGIN
    EXECUTE sp_execute_external_script @language = N'Python'
        , @script = N'
import pickle
from sklearn.naive_bayes import GaussianNB
GNB = GaussianNB()
trained_model = pickle.dumps(GNB.fit(nest_data[["cParts", "dNestingTime", "dTrueArea", "ixMaterial", "ixPlateType", "ixAutoNestStrategy", "ixTorchSelection", "ixTorchSpacingType", "ixCustomPlateType"]], nest_data[["dCropUtil"]].values.ravel()))
'
        , @input_data_1 = N'
SELECT 
    cParts, 
    dNestingTime, 
    dTrueArea, 
    ixMaterial, 
    ixPlateType, 
    ixAutoNestStrategy, 
    ixTorchSelection, 
    ixTorchSpacingType, 
    ixCustomPlateType, 
    dCropUtil 
FROM 
    dbo.Nest 
    INNER JOIN dbo.AutoNest ON dbo.Nest.ixNest = dbo.AutoNest.ixNest
    INNER JOIN dbo.Material ON dbo.Nest.ixMaterial = dbo.Material.ixMaterial
    INNER JOIN dbo.PlateType ON dbo.Nest.ixPlateType = dbo.PlateType.ixPlateType
    INNER JOIN dbo.AutoNestStrategy ON dbo.AutoNest.ixAutoNestStrategy = dbo.AutoNestStrategy.ixAutoNestStrategy
    INNER JOIN dbo.TorchSelection ON dbo.AutoNest.ixTorchSelection = dbo.TorchSelection.ixTorchSelection
    INNER JOIN dbo.TorchSpacingType ON dbo.AutoNest.ixTorchSpacingType = dbo.TorchSpacingType.ixTorchSpacingType
    INNER JOIN dbo.CustomPlateType ON dbo.AutoNest.ixCustomPlateType = dbo.CustomPlateType.ixCustomPlateType
'
        , @input_data_1_name = N'nest_data'
        , @params = N'@trained_model varbinary(max) OUTPUT'
        , @trained_model = @trained_model OUTPUT;
END;
GO