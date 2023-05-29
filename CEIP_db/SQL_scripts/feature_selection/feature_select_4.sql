SELECT
    JS.ixJobSummary,
    P.ixPart,
    P.dLength,
    P.dWidth,
    P.dArea,
    P.cRequired,
    P.cNested,
    N.ixNest,
    N.cParts,
    N.dNestingTime,
    N.dCropUtil,
    N.dTrueArea,
    M.ixMaterial,
    M.dThickness,
    PT.ixPlateType,
    AN.ixAutoNest,
    AN.ixAutoNestStrategy,
    AN.dtStart AS AutoNestStart,
    AN.dtEnd AS AutoNestEnd
FROM 
    JobSummary AS JS
JOIN 
    Part AS P ON JS.ixJobSummary = P.ixJobSummary
JOIN 
    Nest AS N ON JS.ixJobSummary = N.ixJobSummary
JOIN 
    Material AS M ON P.ixMaterial = M.ixMaterial
JOIN 
    PlateType AS PT ON N.ixPlateType = PT.ixPlateType