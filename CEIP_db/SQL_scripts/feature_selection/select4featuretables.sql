SELECT *
FROM dbo.Part AS P
JOIN dbo.Nest AS N ON P.ixJobSummary = N.ixJobSummary
JOIN dbo.Material AS M ON P.ixMaterial = M.ixMaterial
JOIN dbo.AutoNest AS AN ON P.ixJobSummary = AN.ixSession;