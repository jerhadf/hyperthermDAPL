USE master
GO

RESTORE DATABASE CEIP
FROM DISK='C:\ms\CEIPNestingAnalysis\Database\CEIP-March10_2017.BAK'
WITH 
    MOVE 'CEIP' TO 'C:\ms\CEIPNestingAnalysis\Database\CEIP.mdf',
    MOVE 'CEIP_log' TO 'C:\ms\CEIPNestingAnalysis\Database\CEIP_log.ldf',
REPLACE,
STATS=10