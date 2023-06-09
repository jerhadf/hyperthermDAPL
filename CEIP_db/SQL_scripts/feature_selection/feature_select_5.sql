/* 
SEMI-FINAL FEATURE SELECTION SCRIPT (5th iteration)

Selects all of the columns needed for our training data 
Performs all of the basic filtering operations to select only the data needed

OUTPUT: 15,397,214 rows from the database (~100 million rows originally, so this is ~15%). 

TO GET THE RESULTS: 
SELECT * FROM dbo.Features --> exporting the resulting table as a .csv file 
 
SELECT COUNT(*) FROM dbo.Part => 30,970,284 rows 
SELECT COUNT(*) FROM dbo.Nest => 8,711,803 rows 
SELECT COUNT(*) FROM dbo.AutoNest => 1,911,518 rows 
NUMBER OF DISTINCT JOBS: 
SELECT COUNT(DISTINCT ixJobSummary) AS JobCount FROM dbo.Part => 2,637,599 jobs 
SELECT COUNT(DISTINCT ixJobSummary) AS JobCount FROM dbo.Nest => 3,688,620 jobs 
NUMBER OF JOBS SHARED BETWEEN dbo.NEST & dbo.Part => 2,537,550 
Jobs in dbo.Part but not in dbo.Nest => 49 
Jobs in dbo.Nest but not in dbo.Part => 1,051,070 
*/ 

-- Define the CTE that will find the first nest (minimum ixNest) for each job (ixJobSummary)
WITH FirstNest AS (
    -- also count the number of distinct nests within the job as NestCount (used later)
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

-- Main query starts here, selecting from the dbo.Part table
SELECT TOP 10000
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
    an.ixAutoNestStrategy,
    an.fAllPartsNested
-- INTO Features -- specify your table name here
FROM dbo.Part p
-- Perform an INNER JOIN with the FilteredNest CTE, using the ixJobSummary column for the join condition
INNER JOIN FilteredNest fn ON p.ixJobSummary = fn.ixJobSummary
-- Use the JobSummary table to bridge between ixJobSummary and ixSession
INNER JOIN dbo.JobSummary js ON js.ixJobSummary = p.ixJobSummary
-- Finally bring in the AutoNest table
INNER JOIN dbo.AutoNest an ON an.ixSession = js.ixSession
-- Add joins to incorporate product information
INNER JOIN dbo.Session s ON s.ixSession = js.ixSession
INNER JOIN dbo.ProductVersion pv ON pv.ixProductVersion = s.ixProductVersion
INNER JOIN dbo.Product pr ON pr.ixProduct = pv.ixProduct
WHERE
    -- Add product specifications - only include certain ProNest softwares 
    pr.sName IS NULL 
    OR (pr.sName LIKE 'ProNest%' AND pr.sName NOT LIKE '%LT%' AND pr.ixProductType = 0); 