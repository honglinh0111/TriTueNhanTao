use QuanLySach;

--4.1 Liệt kê thông tin khách hàng (Họ tên, Địa chỉ, Tên công ty)
	-- đặt mua nhóm sách ‘Tin học’ 
	-- và tên công ty của khách hàng chính là tên của Nhà cung cấp.
	-- buoc 1: Lay ma nhom sach mon hoc
create view BAI1VIEW as(
select distinct a.*
from(
	select kh.TenKh, kh.DiaChi, kh.TenCT, ncc.MaNCC
	from KhachHang as kh join HoaDon as hd
	on kh.MaKH=hd.MaKH join ChiTietHoaDon as ct
	on hd.MaHD=ct.MaHD join DanhMucSach as dm
	on ct.MaSach=dm.MaSach join NhomSach as ns
	on dm.MaNhom=ns.MaNhom join NhaCungCap as ncc on ncc.MaNCC=dm.MaNCC
	where ns.TenNhom = N'Tin học') a join NhaCungCap on a.MaNCC=NhaCungCap.MaNCC
	where NhaCungCap.TenNCC=a.TenCT);

select *from BAI1VIEW;
--4.2 Trong năm 2005, những sách nào được đặt mua chỉ đúng một lần. 
	--buoc1. Chon nhung quyen sach chi dat mua mot lan
create view BAI2VIEW as(
select b.MaSach, dm.TenSach 
from
	(select MaSach, count(MaSach) as sosach
	from ChiTietHoaDon group by MaSach) b -- dem so lan dat hang cua moi quyen sach
	join DanhMucSach as dm	on b.MaSach=dm.MaSach --de lay ten sach
	join ChiTietHoaDon  as ct on ct.MaSach=b.MaSach -- de co MaHD de chon ngayDH
	join HoaDon as hd on hd.MaHD=ct.MaHD
where b.sosach=1 and year(hd.NgayDH)=2005);

select * from BAI2VIEW

--4.3 Cho biết tổng số tiền mà công ty thu được trong mỗi tháng của năm 2005 (thời gian tính theo ngày đặt hàng)
create view BAI3VIEW as(
SELECT MONTH(NgayDH) AS thang, SUM(soluong*giaban-soluong*giaban*GiamGia/100) as DoanhThu
from HoaDon as hd INNER JOIN ChiTietHoaDon as ct
ON hd.MaHD=ct.MaHD
WHERE year(NgayDH)=2005
GROUP BY month(NgayDH) );
select * from BAI3VIEW

--4.4 Cho biết tổng số sách của mỗi danh mục sách có trong công ty (tổng số lượng sách hiện có và đã bán)
create view BAI4VIEW AS (
select dm.Masach,
	case
		when sum(dm.SLTon) is null then 0
		else sum(dm.SLTon)
	end as TongSoLuong
from  DanhMucSach as  dm left outer join ChiTietHoaDon as ct
on ct.MaSach = dm.MaSach
group by dm.Masach);

select * from BAI4VIEW
--4.5 
--buoc1:tinh tong so luong sach cua moi HD
select MaHD, sum(SoLuong) as s1 into #cau51
from ChiTietHoaDon as ct
group by MaHD;

--buoc2:tinh tong so luong sach cua moi nhan vien
select nv.MaNV, sum(c51.s1) as s2 into #cau52
from #cau51 as c51 join HoaDon as hd on c51.MaHD=hd.MaHD
join NhanVien as nv on nv.MaNV=hd.MaNV
group by nv.MaNV;

--buoc3: tim max

select HoLot, TenNV, s2
from #cau52 join NhanVien on #cau52.MaNV=NhanVien.MaNV
where s2 = (select max(s2) from #cau52));


--cau 4.6 Hang hoa nao co so luong duoc dat mua it nhat
--SoLuong hang hoa da dat cua moi hoa don
select ct.MaHD, sum(ct.SoLuong) as s1 into #cau61
from ChiTietHoaDon as ct
group by ct.MaHD;
select * from #cau61
--join voi bang hoadon
select c61.MaHD, c61.s1 
from #cau61 as c61 
where c61.s1 =(select min(s1) from #cau61);

--cau 4.7--join k group by
select dm.MaSach, dm.TenSach,
		sum(case month(ngayDH) when 1 then ct.SoLuong else 0 end) as T1,
		sum(case month(ngayDH) when 2 then ct.SoLuong else 0 end) as T2,
		sum(case month(ngayDH) when 3 then ct.SoLuong else 0 end) as T3,
		sum(case month(ngayDH) when 4 then ct.SoLuong else 0 end) as T4,
		sum(case month(ngayDH) when 5 then ct.SoLuong else 0 end) as T5,
		sum(case month(ngayDH) when 6 then ct.SoLuong else 0 end) as T6,
		sum(case month(ngayDH) when 7 then ct.SoLuong else 0 end) as T7,
		sum(case month(ngayDH) when 8 then ct.SoLuong else 0 end) as T8,
		sum(case month(ngayDH) when 9 then ct.SoLuong else 0 end) as T9,
		sum(case month(ngayDH) when 10 then ct.SoLuong else 0 end) as T10,
		sum(case month(ngayDH) when 11 then ct.SoLuong else 0 end) as T11,
		sum(case month(ngayDH) when 12 then ct.SoLuong else 0 end) as T12,
		sum(ct.SoLuong)as CaNam
from DanhMucSach as dm inner join ChiTietHoaDon as ct on dm.MaSach=ct.MaSach
inner join HoaDon as hd on hd.MaHD=ct.MaHD
where year(hd.NgayDH)=2005
group by dm.MaSach, dm.TenSach;

--cau 4.8 Tăng lương lên gấp rưỡi so với tổng lương (luongCB+ phụ cấp) 
--cho những nhân viên bán được số lượng sách nhiều hơn 100 trong năm 2005. 
--Thông tin hiển thị: Họ tên nhân viên, tổng lương (luongCB+ phụ cấp) hiện tại 
--và tổng lương sau khi tăng.
--
--buoc1: so sach nhan vien bang trong nam 2005
select nv.MaNV, sum(ct.SoLuong) as s1 into #cau8
	from ChiTietHoaDon as ct inner join HoaDon as hd
	on ct.MaHD=hd.MaHD inner join NhanVien as nv
	on nv.MaNV=hd.MaNV
where year(hd.ngayDH)=2005
group by nv.MaNV;
--buoc2: chon nhan vien co so luong lon hon 100
select c8.MaNV into #cau81
from #cau8 as c8
where c8.s1 > 100;
--tinh tong luong cua nhan vien do
select nv.HoLot, nv.TenNV, sum(nv.LuongCB+nv.PhuCap) as LuongHienTai, sum(nv.LuongCB+nv.PhuCap)+0.5*sum(nv.LuongCB+nv.PhuCap) as LuongKhiTang
from #cau8 as c8 join NhanVien as nv
on c8.MaNV=nv.MaNV
 where nv.MaNV = (select MANV from #cau81) 
group by nv.HoLot, nv.TenNV

--4.9Tăng phụ cấp thêm 50% cho những nhân viên bán được sách nhiều nhất.
-- Thông tin hiển thị: Họ tên nhân viên, phụ cấp hiện tại và phụ cấp sau khi tăng.
--buoc1: so sach ban moi nhan vien 
select nv.MaNV, sum(ct.SoLuong) as s1 into #cau9
	from ChiTietHoaDon as ct inner join HoaDon as hd
	on ct.MaHD=hd.MaHD inner join NhanVien as nv
	on nv.MaNV=hd.MaNV
group by nv.MaNV; 
 select * from #cau9
 --nhan vien ban nhiu sach nhat
 select c9.MaNV into #cau91
 from #cau9 as c9
 where c9.s1= (select max(s1) from #cau9);
 --tang luong
 select nv.MaNV, nv.HoLot, nv.TenNV, sum(nv.LuongCB+nv.PhuCap) as phucapHienTai, sum(nv.LuongCB+1.5*nv.PhuCap) as phucapHienTai
 from #cau9 as c9 join NhanVien as nv
 on c9.MaNV=nv.MaNV
 where nv.MaNV = (select MANV from #cau91) 
 group by nv.MaNV,nv.HoLot, nv.TenNV;
 
 --4.10 Liệt kê nhân viên có ghi hóa đơn cho tất cả các khách hàng.
 --tat ca KH da dat hoa don
select hd.MaKH into #cau100 from HoaDon as hd

 --cac KH ma NV da lap hoa don
select hd.MaNV, hd.MaHD
from  HoaDon as hd 
where hd.MaKH =all (select *from #cau100)

 --4.11) Liệt kê các hóa đơn có chi tiết hóa đơn chứa tất cả các nhóm sách. 
 --Buoc 1: 
 select dm.MaNhom, dm.MaSach into #cau11a
 from DanhMucSach as dm
 group by dm.MaNhom, dm.MaSach
 order by dm.MaNhom

 select ct.MaHD, ct.MaSach into #cau11b
 from ChiTietHoaDon as ct
 group by ct.MaHD, ct.MaSach 
 order by ct.MaHD

select * from #cau11a;

select c11b.MaHD
from #cau11b as c11b
where c11b.MaSach in (select c11a.MaSach from #cau11a as c11a where c11a.MaNhom =ALL (select dm.MaNhom from DanhMucSach as dm));

--4.12) Liệt kê các khách hàng có mua tất cả sách của tác giả 'Nguyễn Minh Trung'
select dm.MaSach, dm.TenSach into #cau12a
from DanhMucSach as dm
where dm.TacGia = N'Nguyễn Minh Trung';

select *from #cau12a;

select kh.MaKH, kh.TenKh
from KhachHang as kh join HoaDon as hd
on kh.MaKH=hd.MaKH join ChiTietHoaDon as ct
on hd.MaHD=ct.MaHD 
where ct.MaSach = ALL (select #cau12a.MaSach from #cau12a);