USE TestDB; 

SELECT   resource_type  ,request_mode  ,request_status 
FROM sys.dm_tran_locks 
WHERE resource_database_id = DB_ID('TestDB')  AND 
request_session_id = 51  AND 
request_mode IN ('S', 'X')  AND resource_type <> 'DATABASE'; 

SELECT  resource_type, request_mode, request_status
 FROM sys.dm_tran_locks 
 WHERE resource_database_id = DB_ID('TestDB')  AND 
 request_session_id = 51  AND 
 request_mode IN ('S', 'X') AND resource_type <> 'DATABASE'; 

 SELECT  resource_type  ,request_mode  ,request_status 
 FROM sys.dm_tran_locks 
 WHERE resource_database_id = DB_ID('TestDB')  AND 
 request_session_id = 51  AND 
 request_mode IN ('S', 'X')  AND resource_type <> 'DATABASE';