-- Define the CTE that will find the first nest (minimum ixNest) for each job (ixJobSummary)
WITH FirstNest AS (
    SELECT ixJobSummary, MIN(ixNest) AS MinNest
    FROM dbo.Nest
    GROUP BY ixJobSummary
)
-- Main query starts here, selecting from the dbo.Part table
-- Limit to just the top 1 million results for testing by adding "TOP 1000000"
SELECT
    p.ixPart,
    p.ixJobSummary, -- include from the PartTable only 
    p.dArea AS dPartTrueArea, 
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
    n.fStrategies, 
    n.dLength AS dSheetLength,
    n.dWidth AS dSheetWidth,
    n.dArea AS dSheetArea,
    -- important: can use the below for info, but not for training! wouldn't be available in production
    n.cTimesCut,
    n.dLengthUsed, 
    n.dWidthUsed, 
    n.dCropUtil,
    n.dPartArea,
    n.cParts, 
    n.dNestingTime, 
    -- The calculated column is added here - calculate the utilization Manually 
    (n.dPartArea / n.dArea) AS calcUtil
INTO PartNestFeatures -- specify your table name here
FROM dbo.Part p
-- Perform an INNER JOIN with the FirstNest CTE, using the ixJobSummary column for the join condition
INNER JOIN FirstNest fn ON p.ixJobSummary = fn.ixJobSummary
-- Perform an INNER JOIN with the dbo.Nest table, using both ixJobSummary and ixNest for the join condition
-- This join ensures that we only get rows from dbo.Nest that correspond to the first nest for each job
INNER JOIN dbo.Nest n ON fn.ixJobSummary = n.ixJobSummary AND fn.MinNest = n.ixNest
WHERE 
    -- can do n.ixPlateType IN (0, 1) to include only rectangular plates (0) and circular plates (1). 
    n.ixPlateType = 0 -- Include only rectangular plates
    AND n.cSafeZones = 0 -- Exclude nests containing safe zones
    AND n.cMaxTorches <= 1 -- Exclude multi-torch jobs (only ones where MaxTorches <1)
    AND n.cTimesCut <> 0 -- Exclude records with no cutting (where TimesCut != 0)
    AND n.dArea <> 0 -- Exclude jobs where dSheetArea is 0