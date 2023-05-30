/* 
FINAL FEATURE SELECTION SCRIPT  - FILTERED 

Selects all of the columns needed for our training data 
Performs all of the basic filtering operations to select only the data needed

OUTPUT:

TO GET THE RESULTS: 
SELECT * FROM dbo.Features --> exporting the resulting table as a .csv file 
*/ 

-- Define the CTE that will find the first AutoNest (minimum ixAutoNesStrategy) for each job (ixJobSummary)
WITH FirstAutoNest AS (
    SELECT js.ixJobSummary, MIN(an.ixAutoNest) AS MinAutoNest, an.ixAutoNestStrategy, an.fAllPartsNested
    FROM dbo.AutoNest an
    INNER JOIN dbo.JobSummary js ON an.ixSession = js.ixSession
    GROUP BY js.ixJobSummary, an.ixAutoNestStrategy, an.fAllPartsNested
),
-- Define the CTE that will find the first nest (minimum ixNest) for each job (ixJobSummary)
WITH FirstNest AS (
    SELECT ixJobSummary, MIN(ixNest) AS MinNest, COUNT(DISTINCT ixNest) AS NestCount
    FROM dbo.Nest
    GROUP BY ixJobSummary
),
-- Define the CTE that performs all the other filtering of the database
FilteredNest AS (
    SELECT n.*
    FROM dbo.Nest n
    INNER JOIN FirstNest fn ON n.ixJobSummary = fn.ixJobSummary AND n.ixNest = fn.MinNest
    WHERE 
    fn.NestCount > 1 -- only include jobs where there are at least two nests
    AND n.ixPlateType = 0 -- Include only rectangular plates. n.ixPlateType IN (0, 1) to include circular plates (1)
    AND n.cSafeZones = 0 -- Exclude nests with safe zones
    AND n.cMaxTorches <= 1 -- Exclude multi-torch jobs
    AND n.cTimesCut <> 0 -- Exclude jobs without any cuts (where cTimesCut = 0)
    AND n.dArea <> 0 -- Exclude jobs where dSheetArea is 0
)
-- Main query (selecting columns) starts here
SELECT
    p.ixJobSummary,
    fn.ixNest,
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
    -- below - from Nesting, may not need as features
    fn.cTimesCut,
    fn.fOutput,
    fn.cParts, 
    fn.cSafeZones,
    fn.ixPlateType, 
    fn.dNestingTime, 
    fn.fStrategies, 
    fn.cMaxTorches,
    fn.dLength AS dSheetLength,
    fn.dWidth AS dSheetWidth,
    fn.dArea AS dSheetArea,
    fn.dLengthUsed, 
    fn.dWidthUsed,
    fn.dPartArea, 
    fn.dTrueArea AS dSheetTrueArea,
    100 * (fn.dPartArea / NULLIF(fn.dArea, 0)) AS calcUtil, -- calculated utilization 
    -- n.dCropUtil, -- utilization - the same as the above, slightly less precise 
    -- add in the columns from the AutoNest table 
    fnan.ixAutoNestStrategy,
    fnan.fAllPartsNested
FROM dbo.Part p
INNER JOIN FilteredNest fn ON p.ixJobSummary = fn.ixJobSummary
INNER JOIN FirstAutoNest fnan ON p.ixJobSummary = fnan.ixJobSummary
WHERE EXISTS (
    SELECT 1
    FROM dbo.JobSummary js
    INNER JOIN dbo.Session s ON s.ixSession = js.ixSession
    INNER JOIN dbo.ProductVersion pv ON pv.ixProductVersion = s.ixProductVersion
    INNER JOIN dbo.Product pr ON pr.ixProduct = pv.ixProduct
    WHERE js.ixJobSummary = p.ixJobSummary
        AND (pr.sName IS NULL OR (pr.sName LIKE 'ProNest%' AND pr.sName NOT LIKE '%LT%' AND pr.ixProductType = 0))
)
AND ixAutoNestStrategy IN (
    SELECT an.ixAutoNestStrategy
    FROM dbo.AutoNest an
    INNER JOIN dbo.JobSummary js ON an.ixSession = js.ixSession
    WHERE js.ixJobSummary = p.ixJobSummary
);
