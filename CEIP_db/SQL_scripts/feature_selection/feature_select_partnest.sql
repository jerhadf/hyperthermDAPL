-- Define the CTE that will find the first nest (minimum ixNest) for each job (ixJobSummary)
WITH FirstNest AS (
    SELECT ixJobSummary, MIN(ixNest) AS MinNest
    FROM dbo.Nest
    GROUP BY ixJobSummary
)
-- Main query starts here, selecting from the dbo.Part table
SELECT 
    p.ixPart, 
    p.ixJobSummary, 
    p.dArea, 
    p.cRequired, 
    p.cNested, 
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
    n.ixJobSummary, 
    n.cParts, 
    n.cSafeZones, 
    n.dNestingTime, 
    n.fStrategies, 
    n.dLength, 
    n.dWidth, 
    n.dArea, 
    n.dLengthUsed, 
    n.dWidthUsed, 
    n.dPartArea, 
    n.dTrueArea
FROM dbo.Part p
-- Perform an INNER JOIN with the FirstNest CTE, using the ixJobSummary column for the join condition
INNER JOIN FirstNest fn ON p.ixJobSummary = fn.ixJobSummary
-- Perform an INNER JOIN with the dbo.Nest table, using both ixJobSummary and ixNest for the join condition
-- This join ensures that we only get rows from dbo.Nest that correspond to the first nest for each job
INNER JOIN dbo.Nest n ON fn.ixJobSummary = n.ixJobSummary AND fn.MinNest = n.ixNest;