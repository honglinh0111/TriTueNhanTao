use QuanLySach;

drop database TestDB
 -- CHƯƠNG 2
 --1) Tạo thủ tục thường trú có chức năng bổ sung thêm một bản ghi mới cho bảng DANHMUCSACH 
 --(kiểm tra tính hợp lệ của dữ liệu cần bổ sung: không trùng khoá chính và đảm bảo toàn vẹn 
 -- tham chiếu trước khi insert).
 --Khoa chinh ko trung
 create procedure cau1(@MaSach char(5),
						@TenSach nvarchar(40),
						@TacGia nvarchar(20),
						@MaNhom char(5)=null,
						@MaNCC char(5)=null,
						@DonGia real=0,
						@SLTon int=0)
as 
	if not exists (select MaSach from DanhMucSach where MaSach=@MaSach)
		if (@MaNhom is null or exists(select MaNhom from NhomSach where MaNhom=@MaNhom)) and (@MaNCC is null or exists(select MaNCC from NhaCungCap where MaNCC=@MaNCC))
			insert into DanhMucSach values (@MaSach,@TenSach,@TacGia,@MaNhom,@MaNCC,@DonGia,@SLTon)

drop proc cau1

exec cau1 'S001A','Tran Nhu','Tran Nhu','N0001','NCC03',2000,10 -- chay duoc nhung ko them vao csdl duoc

--2) Tạo thủ tục thường trú có chức năng thống kê tổng số lượng sách bán được của một danh mục sách
 --có mã bất kỳ (mã sách cần thống kê là tham số của thủ tục). 
 create procedure cau2( @masach char(5))
 as
	select dm.MaSach, dm.TenSach, sum(CT.SoLuong) as N'Tổng sách đã bán' 
	from DanhMucSach as dm left join ChiTietHoaDon as ct
	on dm.MaSach=ct.MaSach
	where dm.MaSach = @masach
	group by dm.MaSach, dm.TenSach

 drop proc cau2;

 exec cau2 'S0002'

 --3) Viết hàm trả về một bảng trong đó cho biết tổng số lượng sách (hiện có và đã bán) 
 --của mỗi danh mục sách là bao nhiêu.
 create function cau3()
 returns table
 as
	return ( select dm.MaSach, dm.TenSach, case	
												when sum(ct.SoLuong) is null then 0
											else
												sum(ct.SoLuong)
											end as TongSoLuongDaBan
			from DanhMucSach as dm left join ChiTietHoaDon as ct
			 on dm.MaSach = ct.MaSach
			 group by dm.MaSach, dm.TenSach)

drop function cau3

select dm.MaSach,dm.TenSach,SLTon+TongSoLuongDaBan as TongSoSach
	from DanhMucSach as dm inner join cau3() as ts on dm.MaSach=ts.MaSach	

--4.1 Khi một bản ghi mới được bổ sung vào bảng này thì giảm số lượng sách hiện có 
--nếu số lượng sách hiện có lớn hơn hoặc bằng số lượng sách được bán ra.
-- Ngược lại thì huỷ bỏ thao tác bổ sung
	create trigger cau4_insert
	on [dbo].[ChiTietHoaDon]
	for insert
	as
	begin
		declare @MaSach char(5)
		declare @SoLuong int
		declare @SLTon int
		select @MaSach=MaSach , @SoLuong= SoLuong
		from inserted
		select @SLTon=SLTon from DanhMucSach where MaSach = @MaSach
		if @SLTon >= @SoLuong
			update DanhMucSach set SLTon=SLTon-@SoLuong where MaSach=@MaSach
		else
			rollback transaction
	end

	create trigger cau4_update
	on chitiethoadon
	for update
	as
	if update(soluong)
	begin
		if exists(select MaHD from inserted where soluong<0) rollback transaction
		else
			begin 
				update DanhMucSach
				set SLTon = SLTon - (select sum (inserted.soluong - deleted.soluong) 
									from inserted inner join deleted 
									on inserted.MaHD=deleted.MaHD and deleted.MaSach = inserted.MaSach
									where inserted.MaSach=DanhMucSach.MaSach
									group by inserted.MaSach)
				where MaSach in (select distinct MaSach from inserted)
				if exists (select masach from DanhMucSach where SLTon <0) rollback tran
			end
	end

	--thuc hien lenh
	insert into HoaDon values('HD01C','NV001','KH001',2005-01-15,2005-02-17,'Can Tho');
	insert into ChiTietHoaDon values('HD01C','S0001',800,15000,0); -- khong them dc vì sluong mua vượt quá sl tồn
	insert into ChiTietHoaDon values('HD01C','S0001',1,15000,0);

--5) Viết trigger cho bảng CHITIETHOADON để sao cho chỉ chấp nhận giá sách bán ra phải nhỏ hơn hoặc bằng
-- giá gốc (giá của cuốn sách trong bảng DANHMUCSACH).
--ct.giaban <= dm.dongia =>

	create trigger cau5 on chitiethoadon
	for insert, update
	as 
	if update(GiaBan)
	begin 
		if exists (select inserted.GiaBan  
					from inserted inner join DanhMucSach as dm 
					on dm.MaSach=inserted.MaSach
					where dm.DonGia < inserted.GiaBan)		rollback tran		
	end

	insert into HoaDon values('HD01B','NV001','KH001',2005-01-15,2005-02-17,'Can Tho');
	insert into ChiTietHoaDon values('HD01B','S0001',2,35,0);

--Cau 2:
--2.1
--Connection 1 khong co share lock la do: share lock duoc giai phong ngay, khong giu cho den het thao tac nen khong tra ve ket qua
--connecttion 1 khong tra ve kqua do connection 2 dang cap nhat DL va chua commit giao dich
--Khoa s da dat duoc do giao dich 2 da commit luc nay connection 1 da select (xin khoa S) thanh cong
--2.2
--Giao dich khong bi chan boi connection 2 la do: o che do read commited napshot connection 1 se phai cho ( khong bi chan) do connection 2 giu khoa X, khong co gia tri tra ve, gia tri ton tai truoc update
--Gtri tra ve la 11, ton tai sau cau lenh update, khong tra ve dirty read, cau lenh select dau tien trong connection 1 tra ve dirty read

DROP DATABASE TestDB; 
--2.6
--cau lenh trong conection 1 khong bi chan ma dang doi connection 2 commit de thuc hien
--(dang doi khoa X)
--2.10
--gia tri ton sau update la 11

drop database TestDB

--Cau 3
--3-1 tao tai khoan
create login NhanVien with password='123', check_policy=ON,
check_expiration = on, default_database=QuanLySach;

create login KeToan with password='123', check_policy=ON,
check_expiration = on, default_database=QuanLySach;

create user NhanVien for login NhanVien;
create user KeToan for login KeToan;

--3-2
grant select, insert, update on khachhang to NhanVien;

--3-3
grant select on hoadon to KeToan;

--3-4
create procedure sp_chitiethoadon (@MaHD char(5))
as
	select * from ChiTietHoaDon where MaHD = @MaHD ;

--drop proc sp_chitiethoadon;

exec sp_chitiethoadon 'HD001';
-- Khi đăng nhập bằng tài khoản kế toán thì khi gọi thủ tục sp_chitiethoadon, sẽ liệt kê ra thông tin của hóa đơn đã gọi
--
create role CT_HOADON authorization dbo
grant execute on sp_chitiethoadon to CT_HOADON
exec sp_addrolemember 'CT_HOADON', 'KeToan';
--ket qua thuc hien duoc

--Câu 4
--4-1
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

--4-2
Use BulkLoggedDB; 
Create Table dbo.AllDatabases (    
 DBID int not null primary key,     
 DBName sysname not null) 
Go 
Insert Into dbo.AllDatabases 
Select database_id, name 
From sys.databases; 
Go 
-- Đợi hai phút 
Waitfor Delay '0:02:00.000'; 
Select GETDATE() As TimeBeforeInsert2; -- Lưu lại điểm thời gian 
--2021-04-19 09:36:01.270
 
Insert Into dbo.AllDatabases 
Select database_id + 1000, name 
From sys.databases; 
Go 

-- Sao lưu nhật ký 
Backup Log BulkLoggedDB     
To Disk = 'C:\Program Files\Microsoft SQL Server\MSSQL11.SQLEXPRESS\MSSQL\Backup\BulkLoggedDB_NoMLTrans.trn'     
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
Select GETDATE() As TimeBeforeInsert2; -- lưu lại điểm thời gian 
--2021-04-19 09:38:56.923
GO 

Select * Into dbo.Alldatabases3 
From sys.databases
Go 
----
-- Backup the log 
Backup Log BulkLoggedDB     
To Disk = 'C:\Program Files\Microsoft SQL Server\MSSQL11.SQLEXPRESS\MSSQL\Backup\BulkLoggedDB_WithMLTrans.trn'     
With init; 
Go 
--4-3
Use master; 
Go -- Phục hồi sao lưu đầy đủ 
Restore Database BulkLoggedDB 
  From Disk = 'C:\Program Files\Microsoft SQL Server\MSSQL11.SQLEXPRESS\MSSQL\Backup\BulkLoggedDB.bak'     
  With Replace, Norecovery; 
go

Restore Log BulkLoggedDB
     From Disk = 'C:\Program Files\Microsoft SQL Server\MSSQL11.SQLEXPRESS\MSSQL\Backup\BulkLoggedDB_NoMLTrans.trn'
	 With Replace, Recovery,
	StopAt = '2021-04-19 09:38:56.923'; -- đây là thời gian mà ta ghi lại   
	 -- ở bước b 
Go

Select * 
From BulkLoggedDB.dbo.AllDatabases 
Order by DBName; 
--KQ: Database 'BulkLoggedDB' cannot be opened. It is in the middle of a restore.

--4-4
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











