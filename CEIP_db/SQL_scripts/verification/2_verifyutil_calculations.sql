/*
This script selects the # of records from dbo.Nest where the trueArea is NOT 0, 
AND where the dCropUtil is NOT equal to the PartArea over the TrueArea. 

The result is the # of records that have "incorrect" utilization calculations. 

Result: 7,315,437 (7 million records)
*/

-- SELECT COUNT(*)
-- FROM dbo.Nest
-- WHERE dTrueArea <> 0 AND dCropUtil <> dPartArea / dTrueArea;

/* Step 1 and 2 are executed in the 1_verifyutil_0orNull.sql script file - run that first */

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