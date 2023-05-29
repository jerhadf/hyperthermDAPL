-- Define the CTE that will find the first nest (minimum ixNest) for each job (ixJobSummary)
WITH FirstNest AS (
    -- also count the number of distincts nests within the job as NestCount (used later)
    SELECT ixJobSummary, MIN(ixNest) AS MinNest, COUNT(DISTINCT ixNest) AS NestCount
    FROM dbo.Nest
    GROUP BY ixJobSummary
)

-- Main query starts here, selecting from the dbo.Part table
SELECT
    p.ixJobSummary,
    n.ixNest,
    p.ixPart,
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
    -- n.dCropUtil -- removed, don't need since we have it calculated (identical but more precise) 
    n.dPartArea, 
    n.dTrueArea AS dSheetTrueArea,
    100 * (n.dPartArea / n.dArea) AS calcUtil, -- calculated utilization 
    an.ixAutoNestStrategy,
    an.fAllPartsNested
INTO Features -- specify your table name here
FROM dbo.Part p

-- Perform an INNER JOIN with the FirstNest CTE, using the ixJobSummary column for the join condition
INNER JOIN FirstNest fn ON p.ixJobSummary = fn.ixJobSummary

-- Perform an INNER JOIN with the dbo.Nest table, using both ixJobSummary and ixNest for the join condition
-- This join ensures that we only get rows from dbo.Nest that correspond to the first nest for each job
INNER JOIN dbo.Nest n ON fn.ixJobSummary = n.ixJobSummary AND fn.MinNest = n.ixNest

-- Use the JobSummary table to bridge between ixJobSummary and ixSession
INNER JOIN dbo.JobSummary js ON js.ixJobSummary = p.ixJobSummary
-- Finally bring in the AutoNest table
INNER JOIN dbo.AutoNest an ON an.ixSession = js.ixSession

-- Add joins to incorporate product information
INNER JOIN dbo.Session s ON s.ixSession = js.ixSession
INNER JOIN dbo.ProductVersion pv ON pv.ixProductVersion = s.ixProductVersion
INNER JOIN dbo.Product pr ON pr.ixProduct = pv.ixProduct

WHERE
    -- only include jobs where there are at least two nests, OR where not all parts were nested
    fn.NestCount > 1 OR (fn.NestCount = 1 AND n.cParts < p.cRequired) OR (fn.NestCount = 1 AND an.fAllPartsNested = 0)
    AND n.ixPlateType = 0 -- Include only rectangular plates. can do n.ixPlateType IN (0, 1) to include circular plates (1)
    AND n.cSafeZones = 0 -- Exclude nests with safe zones
    AND n.cMaxTorches <= 1 -- Exclude multi-torch jobs
    AND n.cTimesCut <> 0 -- Exclude jobs without any cuts (where cTimesCut = 0)
    AND n.dArea <> 0 -- Exclude jobs where dSheetArea is 0
    -- Add product specifications - only include certain ProNest softwares 
    AND (pr.sName IS NULL 
         OR (pr.sName LIKE 'ProNest%' AND pr.sName NOT LIKE '%LT%' AND pr.ixProductType = 0));