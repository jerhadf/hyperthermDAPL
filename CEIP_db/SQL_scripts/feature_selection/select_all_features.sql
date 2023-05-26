SELECT 
    JS.ixJobSummary,
    JS.ixSession,
    P.ixPart,
    P.dLength,
    P.dWidth,
    P.dArea,
    P.cRequired,
    P.cNested,
    P.fExtShape,
    P.dExtArea,
    P.dExtBoundaryDist,
    P.dExtContainedDist,
    P.dLgIntArea,
    P.dLgIntBoundaryDist,
    P.dLgIntContainedDist,
    N.ixNest,
    N.cParts,
    N.cTimesCut,
    N.cMaxTorches,
    N.dMaxTorchSpacing,
    N.dNestingTime,
    N.dCropUtil,
    N.dTrueArea,
    M.ixMaterial,
    M.dThickness,
    M.sGrade,
    PT.ixPlateType,
    PT.sName AS PlateTypeName,
    AN.ixAutoNest,
    AN.ixAutoNestStrategy,
    AN.dtStart AS AutoNestStart,
    AN.dtEnd AS AutoNestEnd,
    ANS.ixAutoNestStrategy AS ANS_ixAutoNestStrategy,
    ANS.sName AS AutoNestStrategyName,
    TS.ixTorchSelection,
    TST.ixTorchSpacingType,
    CPT.ixCustomPlateType,
    CPT.sName AS CustomPlateTypeName
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
JOIN 
    AutoNest AS AN ON JS.ixJobSummary = AN.ixSession
JOIN 
    AutoNestStrategy AS ANS ON AN.ixAutoNestStrategy = ANS.ixAutoNestStrategy
JOIN 
    TorchSelection AS TS ON AN.ixTorchSelection = TS.ixTorchSelection
JOIN 
    TorchSpacingType AS TST ON AN.ixTorchSpacingType = TST.ixTorchSpacingType
JOIN 
    CustomPlateType AS CPT ON AN.ixCustomPlateType = CPT.ixCustomPlateType;