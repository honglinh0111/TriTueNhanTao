use QuanLySach;

select  count(ct.masach) from chitiethoadon as ct inner join DanhMucSach as dm
 on ct.MaSach=dm.MaSach
 group by ct.MaSach
 
--cau 1
--KH dat mua nhom sach tin hoc 
create view Cau1View as(
select distinct kh.TenKh, kh.DiaChi, kh.TenCT from Khachhang as kh 
	join hoadon as hd on kh.MaKH=hd.MaKH
	join ChiTietHoaDon as ct on ct.MaHD=hd.MaHD
	join DanhMucSach as dm on dm.MaSach=ct.MaSach
	join NhomSach as ns on ns.MaNhom=dm.MaNhom
	where ns.TenNhom=N'Tin Học' and kh.TenCT in (select TenNCC from NhaCungCap)
)
select *from Cau1View;

--cau 2
--trong nam 2005 co sach nao dat mua,, dat mua 1 lan
create view Cau2View as(
select dm.TenSach, count(dm.MaSach) as solanxh
from DanhMucSach as dm join ChiTietHoaDon as ct on ct.MaSach=dm.MaSach
join HoaDon as hd on hd.MaHD=ct.MaHD
group by dm.TenSach
having count(dm.MaSach) = 1
)
select *from Cau1View;

--cau 3
--so tien cong ty thu duoc moi thang cua nam 2005
create view Cau3View as(
select month(hd.NgayGH) as Thang, sum(ct.SoLuong*ct.GiaBan) as doanh_thu_moi_thang
from ChiTietHoaDon as ct join HoaDon as hd on hd.MaHD=ct.MaHD
where year(hd.NgayDH)= 2005
group by month(hd.NgayGH)
)
select *from Cau3View;

--cau 4
--tong so luong sach cua moi danh muc sach trong cty (hien co va da ban)
create view Cau4View as(
select dm.MaSach, dm.TenSach,
		dm.SLTon + 
		case 
			when sum(ct.SoLuong) is null then 0
			else sum(ct.SoLuong)
		end as tongsoluong
from DanhMucSach as dm left outer join ChiTietHoaDon as ct on dm.MaSach=ct.MaSach
group by dm.MaSach, dm.TenSach,	dm.SLTon
)
select *from Cau4View;
--cau 5
--nhan vien nao ban nhieu sach nhat -- slong do la bn
create view Cau5View as(
select  nv.HoLot,nv.TenNV, sum(ct.SoLuong) as sosach
from NhanVien as nv inner join HoaDon as hd on nv.MaNV = hd.MaNV
inner join ChiTietHoaDon as ct on ct.MaHD=hd.MaHD
group by  nv.HoLot, nv.TenNV
having sum(ct.SoLuong) >=ALL(select sum(soluong)
							from NhanVien as nv inner join HoaDon as hd
										on nv.MaNV = hd.MaNV
										inner join ChiTietHoaDon as ct 
										on ct.MaHD=hd.MaHD
										group by nv.TenNV)
)
select *from Cau5View;
--cau 6
create view Cau6View as(
select hd.MaHD, sum(ct.SoLuong) as sl_dat_mua 
from DanhMucSach as dm inner join ChiTietHoaDon as ct on dm.MaSach = ct.MaSach
inner join HoaDon as hd on hd.MaHD = ct.MaHD
group by hd.MaHD 
having  sum(ct.SoLuong) <= ALL( select  sum(ct.SoLuong) as sl_dat_mua 
								from DanhMucSach as dm inner join ChiTietHoaDon as ct on dm.MaSach = ct.MaSach
								inner join HoaDon as hd on hd.MaHD = ct.MaHD
								group by hd.MaHD )
)
select *from Cau6View;
--cau 7
create view Cau7View as(
select dm.MaSach, dm.TenSach,
	sum(case month(hd.NgayDH) when 1 then ct.SoLuong else 0 end) as Thang1,
	sum(case month(hd.NgayDH) when 2 then ct.SoLuong else 0 end) as Thang2,
	sum(case month(hd.NgayDH) when 3 then ct.SoLuong else 0 end) as Thang3,
	sum(case month(hd.NgayDH) when 4 then ct.SoLuong else 0 end) as Thang4,
	sum(case month(hd.NgayDH) when 5 then ct.SoLuong else 0 end) as Thang5,
	sum(case month(hd.NgayDH) when 6 then ct.SoLuong else 0 end) as Thang6,
	sum(case month(hd.NgayDH) when 7 then ct.SoLuong else 0 end) as Thang7,
	sum(case month(hd.NgayDH) when 8 then ct.SoLuong else 0 end) as Thang8,
	sum(case month(hd.NgayDH) when 9 then ct.SoLuong else 0 end) as Thang9,
	sum(case month(hd.NgayDH) when 10 then ct.SoLuong else 0 end) as Thang10,
	sum(case month(hd.NgayDH) when 11 then ct.SoLuong else 0 end) as Thang11,
	sum(case month(hd.NgayDH) when 12 then ct.SoLuong else 0 end) as Thang12,
	sum(ct.SoLuong) as CaNam
from DanhMucSach as dm join ChiTietHoaDon as ct on ct.MaSach = dm.MaSach
join HoaDon as hd on hd.MaHD = ct.MaHD
where year(hd.NgayDH) = 2005
group by dm.MaSach, dm.TenSach
)
select *from Cau7View;
--cau 8
alter table nhanvien add TongLuong float(10);

update NhanVien
set TongLuong = (LuongCB+PhuCap)*1.5
where NhanVien.MaNV = 
		(select nv. MaNV
		from NhanVien as nv inner join HoaDon as hd on nv.MaNV = hd.MaNV
							inner join ChiTietHoaDon as ct on hd.MaHD= ct.MaHD
		where year(hd.NgayDH) = 2005
		group by nv.MaNV
		having sum(SoLuong) > 100 
		)
select *from NhanVien
create view Cau8View as(
select nv.HoLot, nv.TenNV, nv.TongLuong from NhanVien as nv
where nv.TongLuong is not null
)
select *from Cau8View;
-- cau 9

update NhanVien
set PhuCap = PhuCap*1.5
where MaNV in 
		(select hd.MaNV 
			 from  HoaDon as hd inner join ChiTietHoaDon as ct 
			 on hd.MaHD=ct.MaHD
			 group by hd.MaNV 
			 having sum(ct.SoLuong) >=ALL(select sum(ct.SoLuong) from ChiTietHoaDon as ct
							inner join HoaDon as hd on hd.MaHD=ct.MaHD
							group by hd.MaNV)
		)
select *from NhanVien

--Cau 10
--nhan vien co ghi hoa don cho tat ca cac KH

--đếm KH của mỗi NV ghi hóa đơn
select nv.manv, nv.holot, nv.tennv
from NhanVien as nv inner join hoadon as hd on nv.manv=hd.manv
inner join khachhang as kh on hd.makh=kh.makh
group by nv.manv, nv.holot, nv.tennv -- so KH cua moi NV 
having count(kh.makh) >= ALL(select count(kh.makh) as soKH
							from NhanVien as nv inner join hoadon as hd on nv.manv=hd.manv
							inner join khachhang as kh on hd.makh=kh.makh
							group by nv.manv, nv.holot, nv.tennv --dem so KH da dat mua hang tren hoa don
						)
--kq NV MAI			

--cách khác
	select distinct nv.HoLot, nv.tennv, hd.makh into #b101
	from nhanvien as nv inner join hoadon as hd on nv.manv = hd.manv;
	
	select holot, tennv, count(makh) as slk into #b102
	from #b101
	group by holot, tennv;

	select count(makh) as sl into #b103 from khachhang group by makh;

	select holot, tennv
	from #b102
	where slk = (select sum(sl) as tongsokh from #b103)	
		
--cau 11 Liet ke cac hoa don co chi tiet hoa don chua tat ca cac nhom sach
select hd.MaHD,hd.MaKH,hd.MaNV,hd.NoiGiao,hd.NgayDH,hd.NgayGH, count(dm.MaNhom) as so_nhom_sach
from HoaDon as hd inner join ChiTietHoaDon as ct on hd.MaHD=ct.MaHD
inner join DanhMucSach as dm on ct.MaSach=dm.MaSach
group by hd.MaHD ,hd.MaKH,hd.MaNV,hd.NoiGiao,hd.NgayDH,hd.NgayGH-- so nhom sach cua moi hoa don
having count(dm.MaNhom) >= all (select count(*) from nhomsach)

select * from NhomSach -- co 6 nhom sach
--kq hd004

--cach khach
select count(manhom) as sl into #b111 from nhomsach group by manhom

select ct.mahd, ns.manhom , count(ct.masach) as soluongsach into #b112
from chitiethoadon as ct inner join danhmucsach as dm on ct.masach = dm.masach
inner join nhomsach as ns on dm.manhom = ns.manhom
group by ct.mahd, ns.manhom;

select mahd, sum(soluongsach) as sls into #b113
from #b112
group by mahd

select hd.mahd, hd.manv, hd.makh, hd.ngaydh, hd.ngaygh, hd.noigiao
from hoadon as hd inner join #b113 as b113 on hd.MaHD=b113.MaHD
where b113.sls  =(select  sum(sl) from #b111);

--cau 12 Liet ke KH co mua tat ca sach cua NMT
select kh.MaKH, kh.tenKH
from KhachHang as kh inner join HoaDon as hd on kh.maKH=hd.MaHD
inner join ChiTietHoaDon as ct on hd.MaHD=ct.MaHD
inner join DanhMucSach as dm on dm.MaSach = ct.MaSach
where dm.TacGia = N'Nguyễn Minh Trung'