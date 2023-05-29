-- Define the CTE that will find the first nest (minimum ixNest) for each job (ixJobSummary)
WITH FirstNest AS (
    SELECT ixJobSummary, MIN(ixNest) AS MinNest, ixSession
    FROM dbo.Nest
    GROUP BY ixJobSummary, ixSession
)
-- Main query starts here, selecting from the dbo.Part table
SELECT TOP 100000 -- limit to 100,000 rows 
    p.ixPart, 
    p.ixJobSummary AS ixJobSummary_Part,  -- renamed to avoid conflict with Nest table
    p.dArea, 
    p.cRequired, 
    p.cNested, 
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
    n.dLength, 
    n.dWidth, 
    n.dArea, 
    n.dLengthUsed, 
    n.dWidthUsed, 
    n.dCropUtil,
    n.dPartArea, 
    n.dTrueArea,
    an.ixAutoNestStrategy,
    an.fAllPartsNested
FROM dbo.Part p
-- Perform an INNER JOIN with the FirstNest CTE, using the ixJobSummary column for the join condition
INNER JOIN FirstNest fn ON p.ixJobSummary = fn.ixJobSummary
-- Perform an INNER JOIN with the dbo.Nest table, using both ixJobSummary and ixNest for the join condition
-- This join ensures that we only get rows from dbo.Nest that correspond to the first nest for each job
INNER JOIN dbo.Nest n ON fn.ixJobSummary = n.ixJobSummary AND fn.MinNest = n.ixNest
-- Perform an INNER JOIN with the dbo.AutoNest table, using the ixSession column for the join condition
INNER JOIN dbo.AutoNest an ON n.ixSession = an.ixSession;