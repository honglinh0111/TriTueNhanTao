--con2
USE TestDB; 
-- Connection 2 
BEGIN TRAN;  
UPDATE TestTable SET Col2 = Col2 + 1 
 WHERE Col1 = 1

 COMMIT TRAN 


