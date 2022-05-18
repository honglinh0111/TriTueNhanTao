use QuanLySach;

drop database TestDB
-- Cau 1:
--1.1
	create procedure cau1(@MaSach char(5),
							@TenSach nvarchar(40),
							@TacGia nvarchar(20),
							@MaNhom char(5)=null,
							@MaNCC char(5)=null,
							@DonGia real=0,
							@SLTon int=0)
	as
		if not exists(select MaSach from DanhMucSach where MaSach=@MaSach)
			if(@MaNhom is null or exists(select MaNhom from NhomSach where MaNhom=@MaNhom)) and (@MaNCC is null or exists(select MaNCC from NhaCungCap 
																																		where MaNCC=@MaNCC))
				insert into DanhMucSach values(@MaSach,@TenSach,@TacGia,@MaNhom,@MaNCC,@DonGia,@SLTon)

exec cau1 'S001A','Tran Nhu','Tran Nhu','N0001','NCC03',2000,10
exec cau1 'S0001','Tran Nhu','Tran Nhu','N0001','NCC03',2000,10 

--1.2
	create procedure cau2(@MaSach char(5))
	as
		select dm.MaSach,TenSach,sum(ct.soluong) as soluongban
		from DanhMucSach as dm left join ChiTietHoaDon as ct on dm.MaSach=ct.MaSach
		where dm.MaSach=@MaSach
		group by dm.MaSach,TenSach

exec cau2 'S0002'
			
--1.3
	create function cau3()
	returns table
	as
	return (select dm.MaSach, TenSach, case
										when sum(ct.soluong) is null then 0
										else
											sum(ct.soluong)
									end as TongSoLuong
			from DanhMucSach as dm left join ChiTietHoaDon as ct on dm.MaSach=ct.MaSach
			group by dm.MaSach, TenSach)
			
	select dm.MaSach,dm.TenSach,SLTon+TongSoLuong as TongSoSach
	from DanhMucSach as dm inner join cau3() as ts on dm.MaSach=ts.MaSach	
	
--1.4
	create trigger cau4_insert
	on ChiTietHoaDon
	for insert
	as
	begin
		declare @MaSach char(5)
		declare @SoLuong int
		declare @SLTon int
		select @MaSach=MaSach,@SoLuong=SoLuong
		from inserted
		select @SLTon=SLTon from DanhMucSach where MaSach=@MaSach
		if @SLTon>=@SoLuong
			update DanhMucSach set SLTon=SLTon-@SoLuong where MaSach=@MaSach
		else
			rollback transaction
	end
	
	create trigger cau4_update
	on ChiTietHoaDon
	for update
	as
	if update(SoLuong)
	begin	
		if exists(select MaHD from inserted where SoLuong<0)	rollback transaction
		else
			begin
				update DanhMucSach
				set SLTon=SLTon-(select sum(inserted.SoLuong-deleted.SoLuong) from inserted inner join deleted on inserted.MaHD=deleted.MaHD and 
																															inserted.MaSach=deleted.MaSach
								where inserted.MaSach=DanhMucSach.MaSach
								group by inserted.MaSach)
				where MaSach in (select distinct MaSach from inserted)
				if exists(select MaSach from DanhMucSach where SLTon<0)		rollback transaction
			end
		end		

insert into HoaDon values('HD01A','NV001','KH001',2005-01-15,2005-02-17,'Can Tho')
insert into ChiTietHoaDon values('HD01A','S001A',2,35000,0)

--1.5
	create trigger cau5
	on ChiTietHoaDon
	for insert, update
	as
	if update(GiaBan)
	begin
		if exists(select inserted.MaSach from inserted inner join DanhMucSach as dm on inserted.MaSach=dm.MaSach
						where dm.DonGia<inserted.GiaBan)
			rollback transaction
	end

insert into HoaDon values('HD01A','NV001','KH001',2005-01-15,2005-02-17,'Can Tho')
insert into ChiTietHoaDon values('HD01A','S001A',2,35,0)

-- Cau  2:
--2.1
--Connection 1 khong co share lock la do: shared lock duoc giai phong ngay , khong giu cho den het giao tac nen khong tra ve ket qua
--Connection 1 khhong tra ve kq do connection 2 dang cap nhat DL va chua commit giao dich
--Khoa S da dat duoc do giao dich 2 da commit luc nay connection 1 da select (xin khoa S) thanh cong

--2.2
--Giao dich khong bi chan boi connection 2 la do: o che do read commited napshot connection 1 se phai cho ( khong bi chan) do connection 2 giu khoa X, khong co gia tri tra ve, gia tri ton tai truoc update
--Gtri tra ve la 11, ton tai sau cau lenh update, khong tra ve dirty read, cau lenh select dau tien trong connection 1 tra ve dirty read

drop database TestDB

--Cau 3:
--3.1
create login NhanVien with password='123',check_policy=on,
check_expiration=on,default_database=QuanLySach;

create login KeToan with password='123',check_policy=on,
check_expiration=on,default_database=QuanLySach;

create user NhanVien for login NhanVien;
create user KeToan for login KeToan;

--3.2
grant select,insert,update on khachhang to NhanVien;

--3.3
grant select on hoadon to KeToan;

--3.4
create procedure cau34(@MaHD char(5))
as
	select * from ChiTietHoaDon where MaHD=@MaHD ;

exec cau34 'HD002'

--KeToan: khong thuc thi duoc thu tuc


create role CT_HOADON authorization dbo

grant execute on cau34 to CT_HOADON;

exec sp_addrolemember 'CT_HOADON','KeToan'

-- Cau 4:
Use master;
-- Bỏ cơ sở dữ liệu nếu tồn tại
If DB_ID('BulkLoggedDB') Is not Null
 Drop Database BulkLoggedDB;
GO
-- Tạo cơ sở dữ liệu, đặt mô hình phục hồi bulk-logged và sao lưu đầy đủ
Create Database BulkLoggedDB;
GO
Alter Database BulkLoggedDB Set Recovery Bulk_logged;
Go
Backup Database BulkLoggedDB
 To Disk = 'C:\Program Files\Microsoft SQL Server\MSSQL11.SQLEXPRESS\MSSQL\backup\BulkLoggedDB.bak'
 With init;
GO

Use BulkLoggedDB;
Create Table dbo.AllDatabases (
 DBID int not null primary key,
 DBName sysname not null)
Go
Insert Into dbo.AllDatabases
Select database_id, name
From sys.databases;
Go

Waitfor Delay '0:02:00.000';
Select GETDATE() As TimeBeforeInsert2; -- Lưu lại điểm thời gian: 2021-04-19 09:35:38.390

Insert Into dbo.AllDatabases
Select database_id + 1000, name
From sys.databases;
Go
-- Sao lưu nhật ký
Backup Log BulkLoggedDB
 To Disk = 'C:\Program Files\Microsoft SQL Server\MSSQL11.SQLEXPRESS\MSSQL\backup\BulkLoggedDB_NoMLTrans.trn'
 With init;
Go
-- Tạo bảng thứ hai
Create Table dbo.AllDatabases2 (
 DBID int not null primary key,
 DBName sysname not null)
Go
Insert Into dbo.AllDatabases2
Select database_id, name
From sys.databases;
Go
Waitfor Delay '0:02:00.000';
Select GETDATE() As TimeBeforeInsert2; -- lưu lại điểm thời gian: 2021-04-19 09:38:15.843
GO
Select * Into dbo.Alldatabases3
From sys.databases
Go
-- Backup the log
Backup Log BulkLoggedDB
 To Disk = 'C:\Program Files\Microsoft SQL Server\MSSQL11.SQLEXPRESS\MSSQL\backup\BulkLoggedDB_WithMLTrans.trn'
 With init;
Go


Use master;
Go
-- Phục hồi sao lưu đầy đủ
Restore Database BulkLoggedDB
 From Disk = 'C:\Program Files\Microsoft SQL Server\MSSQL11.SQLEXPRESS\MSSQL\backup\BulkLoggedDB.bak'
 With Replace, Norecovery;
Go
Restore Log BulkLoggedDB
 From Disk = 'C:\Program Files\Microsoft SQL Server\MSSQL11.SQLEXPRESS\MSSQL\backup\BulkLoggedDB_NoMLTrans.trn'
 With Replace, Recovery,
 StopAt = '2021-04-19 09:35:38.390'; -- đây là thời gian mà ta ghi lại 
 -- ở bước b
Go
Select *
From BulkLoggedDB.dbo.AllDatabases
Order by DBName;
--KQ: Database 'BulkLoggedDB' cannot be opened. It is in the middle of a restore.

Restore HeaderOnly From Disk = 'C:\Program Files\Microsoft SQL Server\MSSQL11.SQLEXPRESS\MSSQL\backup\BulkLoggedDB_WithMLTrans.trn';
Go
-- Phục hồi sao lưu đầy đủ
Restore Database BulkLoggedDB
 From Disk = 'C:\Program Files\Microsoft SQL Server\MSSQL11.SQLEXPRESS\MSSQL\backup\BulkLoggedDB.bak'
 With Replace, Norecovery;
Go
Restore Log BulkLoggedDB
 From Disk = 'C:\Program Files\Microsoft SQL Server\MSSQL11.SQLEXPRESS\MSSQL\backup\BulkLoggedDB_NoMLTrans.trn'
 With Replace, NoRecovery;
Go
Restore Log BulkLoggedDB
 From Disk = 'C:\Program Files\Microsoft SQL Server\MSSQL11.SQLEXPRESS\MSSQL\backup\BulkLoggedDB_WithMLTrans.trn'
 With Replace, Recovery,
 StopAt = '2021-04-19 09:21:00.943'; -- đây là thời gian mà ta ghi lại 
 -- ở bước b
Go