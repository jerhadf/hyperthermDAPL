-- Define the CTE that will find the first nest (minimum ixNest) for each job (ixJobSummary)
WITH FirstNest AS (
    SELECT ixJobSummary, MIN(ixNest) AS MinNest
    FROM dbo.Nest
    GROUP BY ixJobSummary
)
-- Main query starts here, selecting from the dbo.Part table
-- Limit to just the top 1 million results for testing
SELECT TOP 1000000
    p.ixPart,
    p.ixJobSummary, -- include from the PartTable only 
    p.dArea AS dPartTrueArea, 
    p.cRequired, 
    p.cNested AS cNumNested,
    p.ixMaterial, 
    p.fExtShape, 
    p.dExtArea, 
    p.dExtBoundaryDist, 
    p.dExtContainedDist, 
    p.dLgIntArea, 
    p.dLgIntBoundaryDist, 
    p.dLgIntContainedDist, 
    p.dLgExtConArea, 
    p.dLgExtConBoundaryDist, 
    p.dLgExtConContainedDist,
    n.cTimesCut,
    n.fOutput,
    n.cParts, 
    n.cSafeZones,
    n.ixPlateType, 
    n.dNestingTime, 
    n.fStrategies, 
    n.cMaxTorches,
    n.dLength AS dSheetLength,
    n.dWidth AS dSheetWidth,
    n.dArea AS dSheetArea,
    n.dLengthUsed, 
    n.dWidthUsed, 
    n.dCropUtil,
    n.dPartArea, 
    n.dTrueArea AS dSheetTrueArea
FROM dbo.Part p
-- Perform an INNER JOIN with the FirstNest CTE, using the ixJobSummary column for the join condition
INNER JOIN FirstNest fn ON p.ixJobSummary = fn.ixJobSummary
-- Perform an INNER JOIN with the dbo.Nest table, using both ixJobSummary and ixNest for the join condition
-- This join ensures that we only get rows from dbo.Nest that correspond to the first nest for each job
INNER JOIN dbo.Nest n ON fn.ixJobSummary = n.ixJobSummary AND fn.MinNest = n.ixNest;