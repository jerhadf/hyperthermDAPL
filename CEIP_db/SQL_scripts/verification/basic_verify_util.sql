/*
This script checks that utilization is correct
Finds the percentage of records where the calculations are not correct
*/

SELECT
    (SELECT COUNT(*) FROM dbo.Nest WHERE dTrueArea <> 0 AND dCropUtil <> dPartArea / dTrueArea) * 100.0 / (SELECT COUNT(*) FROM dbo.Nest WHERE dTrueArea <> 0) AS PercentageIncorrectCropUtil,
    (SELECT COUNT(*) FROM dbo.Nest WHERE dLengthUsed <> 0 AND dWidthUsed <> 0 AND dPartArea <> dLengthUsed * dWidthUsed) * 100.0 / (SELECT COUNT(*) FROM dbo.Nest WHERE dLengthUsed <> 0 AND dWidthUsed <> 0) AS PercentageIncorrectPartArea,
    (SELECT COUNT(*) FROM dbo.Nest WHERE dLength <> 0 AND dWidth <> 0 AND dArea <> dLength * dWidth) * 100.0 / (SELECT COUNT(*) FROM dbo.Nest WHERE dLength <> 0 AND dWidth <> 0) AS PercentageIncorrectArea,
    (SELECT COUNT(*) FROM dbo.Nest WHERE dArea <> 0 AND dTrueArea <> dArea) * 100.0 / (SELECT COUNT(*) FROM dbo.Nest WHERE dArea <> 0) AS PercentageIncorrectTrueArea;