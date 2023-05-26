CREATE PROCEDURE predict_utilization (@model_name VARCHAR(100))
AS
BEGIN
    DECLARE @gnb_model VARBINARY(max) = (
            SELECT model
            FROM utilization_models
            WHERE model_name = @model_name
            );

    EXECUTE sp_execute_external_script @language = N'Python'
        , @script = N'
import pickle
utilization_model = pickle.loads(gnb_model)
utilization_pred = utilization_model.predict(nest_data[["cParts", "dNestingTime", "dTrueArea", "ixMaterial", "ixPlateType", "ixAutoNestStrategy", "ixTorchSelection", "ixTorchSpacingType", "ixCustomPlateType"]])
nest_data["PredictedUtilization"] = utilization_pred
OutputDataSet = nest_data[["ixNest","dCropUtil","PredictedUtilization"]] 
'
        , @input_data_1 = N'
SELECT 
    ixNest,
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
        , @params = N'@gnb_model varbinary(max)'
        , @gnb_model = @gnb_model
    WITH RESULT SETS((
                "ixNest" INT
              , "dCropUtil" FLOAT
              , "PredictedUtilization" FLOAT
                ));
END;
GO