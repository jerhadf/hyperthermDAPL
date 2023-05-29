-- Main query starts here, selecting from the dbo.Part table
SELECT TOP 100000
    p.ixPart,
    p.ixJobSummary,
    n.ixNest, 
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
    n.dPartArea, 
    n.dTrueArea AS dSheetTrueArea,
    -- The calculated column is added here
    (n.dPartArea / n.dArea) AS calcUtil,
    an.ixAutoNestStrategy,
    an.fAllPartsNested
FROM dbo.Part p
INNER JOIN dbo.Nest n ON p.ixJobSummary = n.ixJobSummary 
-- Use the JobSummary table to bridge between ixJobSummary and ixSession
INNER JOIN dbo.JobSummary js ON js.ixJobSummary = p.ixJobSummary
-- Finally bring in the AutoNest table
INNER JOIN dbo.AutoNest an ON an.ixSession = js.ixSession
WHERE 
    n.ixPlateType IN (0, 1) -- Exclude non-rectangular and non-circular plates
    AND n.cSafeZones = 0 -- Exclude nests with safe zones
    AND n.cMaxTorches = 1 -- Exclude multi-torch jobs
    AND n.cTimesCut > 0 -- Exclude jobs without any cuts
    AND n.dArea > 0; -- Exclude jobs where dSheetArea is 0