-- Connection 1, sử dụng biến @@SPID để  biết transaction thuộc connection 1 
SELECT @@SPID; 
-- Session ID: <đặt kết quả @@SPID ở câu select trên ở đây> 
GO 
CREATE DATABASE TestDB; 
GO 
USE TestDB; 
GO 
CREATE TABLE TestTable (  Col1 INT NOT NULL  ,Col2 INT NOT NULL ); 
INSERT TestTable (Col1, Col2) VALUES (1,10); INSERT TestTable (Col1, Col2) VALUES (2,20);
INSERT TestTable (Col1, Col2) VALUES (3,30); INSERT TestTable (Col1, Col2) VALUES (4,40); 
INSERT TestTable (Col1, Col2) VALUES (5,50); INSERT TestTable (Col1, Col2) VALUES (6,60); 

--
SET TRANSACTION ISOLATION LEVEL READ COMMITTED; 
BEGIN TRAN;  
SELECT * FROM TestTable  WHERE Col1 = 1; 
COMMIT TRAN; 

SET TRANSACTION ISOLATION LEVEL READ COMMITTED; 
BEGIN TRAN;  
SELECT * FROM TestTable  WHERE Col1 = 1;
