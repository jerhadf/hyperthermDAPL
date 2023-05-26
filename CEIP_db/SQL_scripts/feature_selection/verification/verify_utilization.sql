/*
This script selects the # of records from dbo.Nest where the trueArea is NOT 0, 
AND where the dCropUtil is NOT equal to the PartArea over the TrueArea. 

The result is the # of records that have "incorrect" utilization calculations. 

Result: 7,315,437 (7 million records)
*/

-- SELECT COUNT(*) 
-- FROM dbo.Nest
-- WHERE dTrueArea <> 0 AND dCropUtil <> dPartArea / dTrueArea;

/* Step 1: Count the rows where the values are 0 or N/A for each column. 
   We're doing this to identify potential data issues that could affect our calculations. */

SELECT 
    (SELECT COUNT(*) FROM dbo.Nest WHERE cUtilization = 0 OR cUtilization IS NULL)*100.0 / COUNT(*) as Percentage_cUtilization_Zero_Or_Null,
    (SELECT COUNT(*) FROM dbo.Nest WHERE dPartArea = 0 OR dPartArea IS NULL)*100.0 / COUNT(*) as Percentage_dPartArea_Zero_Or_Null,
    (SELECT COUNT(*) FROM dbo.Nest WHERE dTrueArea = 0 OR dTrueArea IS NULL)*100.0 / COUNT(*) as Percentage_dTrueArea_Zero_Or_Null,
    (SELECT COUNT(*) FROM dbo.Nest WHERE dLengthUsed = 0 OR dLengthUsed IS NULL)*100.0 / COUNT(*) as Percentage_dLengthUsed_Zero_Or_Null,
    (SELECT COUNT(*) FROM dbo.Nest WHERE dWidthUsed = 0 OR dWidthUsed IS NULL)*100.0 / COUNT(*) as Percentage_dWidthUsed_Zero_Or_Null
FROM dbo.Nest;

/* Step 2: Remove rows where the values are 0 or N/A. 
   These rows could skew the results of our calculations, so we're excluding them. 
   Here we're creating a temporary table to hold the filtered data. */

SELECT * INTO #Nest_Clean
FROM dbo.Nest 
WHERE 
    cUtilization != 0 AND cUtilization IS NOT NULL AND 
    dPartArea != 0 AND dPartArea IS NOT NULL AND 
    dTrueArea != 0 AND dTrueArea IS NOT NULL AND 
    dLengthUsed != 0 AND dLengthUsed IS NOT NULL AND 
    dWidthUsed != 0 AND dWidthUsed IS NOT NULL;


/* Step 3: Verify the equations. 
   We're counting the rows where the equations do not hold true. 
   The results are expressed as percentages of the total number of rows in our cleaned data. */

/* This tolerance level is used to avoid floating point precision issues. 
ABS(x) returns the absolute value of x, and @tolerance is the tolerance level. 
If the absolute difference between the left and right side of each equation is greater than the tolerance level, 
then that equation is considered incorrect for that row.
*/ 
DECLARE @tolerance float; 
SET @tolerance = 0.00001;

SELECT 
    (SELECT COUNT(*) FROM #Nest_Clean WHERE ABS(dCropUtil - CAST(dPartArea AS float) / CAST(dTrueArea AS float)) > @tolerance)*100.0 / COUNT(*) as Percentage_dCropUtil_Incorrect,
    (SELECT COUNT(*) FROM #Nest_Clean WHERE ABS(dPartArea - dLengthUsed * dWidthUsed) > @tolerance)*100.0 / COUNT(*) as Percentage_dPartArea_Incorrect,
    (SELECT COUNT(*) FROM #Nest_Clean WHERE ABS(dArea - dLength * dWidth) > @tolerance)*100.0 / COUNT(*) as Percentage_dArea_Incorrect,
    (SELECT COUNT(*) FROM #Nest_Clean WHERE ABS(dTrueArea - dArea) > @tolerance)*100.0 / COUNT(*) as Percentage_dTrueArea_Incorrect
FROM #Nest_Clean;