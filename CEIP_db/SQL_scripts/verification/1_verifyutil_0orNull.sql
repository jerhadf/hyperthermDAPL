/* Step 1: Count the rows where the values are 0 or N/A for each column. 
   We're doing this to identify potential data issues that could affect our calculations. */

SELECT 
    (SELECT COUNT(*) FROM dbo.Nest WHERE dCropUtil = 0 OR dCropUtil IS NULL)*100.0 / COUNT(*) as Percentage_dCropUtil_Zero_Or_Null,
    (SELECT COUNT(*) FROM dbo.Nest WHERE dPartArea = 0 OR dPartArea IS NULL)*100.0 / COUNT(*) as Percentage_dPartArea_Zero_Or_Null,
    (SELECT COUNT(*) FROM dbo.Nest WHERE dTrueArea = 0 OR dTrueArea IS NULL)*100.0 / COUNT(*) as Percentage_dTrueArea_Zero_Or_Null,
    (SELECT COUNT(*) FROM dbo.Nest WHERE dLengthUsed = 0 OR dLengthUsed IS NULL)*100.0 / COUNT(*) as Percentage_dLengthUsed_Zero_Or_Null,
    (SELECT COUNT(*) FROM dbo.Nest WHERE dWidthUsed = 0 OR dWidthUsed IS NULL)*100.0 / COUNT(*) as Percentage_dWidthUsed_Zero_Or_Null
FROM dbo.Nest;

/* Step 2: Remove rows where the values are 0 or N/A. 
   These rows could skew the results of our calculations, so we're excluding them. 
   Here we're creating a temporary table to hold the filtered data. */

SELECT * INTO #Nest_Clean -- Create the cleaned table
FROM dbo.Nest 
WHERE
    dCropUtil != 0 AND dCropUtil IS NOT NULL AND 
    dPartArea != 0 AND dPartArea IS NOT NULL AND 
    dTrueArea != 0 AND dTrueArea IS NOT NULL AND 
    dLengthUsed != 0 AND dLengthUsed IS NOT NULL AND 
    dWidthUsed != 0 AND dWidthUsed IS NOT NULL;