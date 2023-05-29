-- Select the final list of features for the ML model

-- Starting the query by selecting the required columns from dbo.Part table
SELECT P.ixPart, P.ixJobSummary, P.dArea, P.cRequired, P.cNested, P.ixMaterial, P.fExtShape, 
       P.dExtArea, P.dExtBoundaryDist, P.dExtContainedDist, P.dLgIntArea, P.dLgIntBoundaryDist, 
       P.dLgIntContainedDist, P.dLgExtConArea, P.dLgExtConBoundaryDist, P.dLgExtConContainedDist,

       -- Selecting the required columns from dbo.Nest table
       N.ixJobSummary, N.cParts, N.cSafeZones, N.dNestingTime, N.fStrategies, N.dLength, N.dWidth, 
       N.dArea, N.dLengthUsed, N.dWidthUsed, N.dPartArea, N.dTrueArea,

       -- Selecting the required columns from dbo.AutoNest table
       AN.ixAutoNestStrategy, AN.ixSession, AN.sSafeZones, AN.fAllPartsNested

-- Starting with dbo.Part table
FROM dbo.Part P

-- Joining dbo.Part with dbo.Nest on ixJobSummary column
INNER JOIN dbo.Nest N ON P.ixJobSummary = N.ixJobSummary

-- Joining dbo.Nest with dbo.JobSummary to bridge the ixSession and ixJobSummary columns
INNER JOIN dbo.JobSummary JS ON N.ixJobSummary = JS.ixJobSummary

-- Joining dbo.JobSummary with dbo.AutoNest on ixSession column to bring in AutoNest data
INNER JOIN dbo.AutoNest AN ON JS.ixSession = AN.ixSession;