-- Define the CTE that will find the first nest (minimum ixNest) for each job (ixJobSummary)
WITH FirstNest AS (
    SELECT ixJobSummary, MIN(ixNest) AS MinNest
    FROM dbo.Nest
    GROUP BY ixJobSummary
), 
-- Define the CTE that maps ixJobSummary to ixSession for the first nest of each job
FirstNestSessions AS (
    SELECT fn.ixJobSummary, js.ixSession
    FROM FirstNest fn
    INNER JOIN dbo.JobSummary js ON fn.ixJobSummary = js.ixJobSummary
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
    n.dTrueArea,
    an.ixAutoNestStrategy, 
    an.ixSession, 
    an.sSafeZones, 
    an.fAllPartsNested
FROM dbo.Part p
-- Perform an INNER JOIN with the FirstNestSessions CTE, using the ixJobSummary column for the join condition
INNER JOIN FirstNestSessions fns ON p.ixJobSummary = fns.ixJobSummary
-- Perform an INNER JOIN with the dbo.Nest table, using both ixJobSummary and ixNest for the join condition
-- This join ensures that we only get rows from dbo.Nest that correspond to the first nest for each job
INNER JOIN dbo.Nest n ON fns.ixJobSummary = n.ixJobSummary AND fns.MinNest = n.ixNest
-- Perform an INNER JOIN with the dbo.AutoNest table, using the ixSession from FirstNestSessions for the join condition
-- This join will add the desired columns from dbo.AutoNest without creating additional rows
INNER JOIN dbo.AutoNest an ON fns.ixSession = an.ixSession;