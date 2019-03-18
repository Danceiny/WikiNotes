# Tips to write SQL using index

- 不要使用not操作符
- 不要在sql中使用函数 `Select * from emp where substr(ename, 1, 1) = ‘K’;`
- 不要比较不同类型的数据
- 使用EXIST函数，而不是IN函数 `Select product_id, qty from product where product_id = 167 and
item_no in (select item_no from items); =====?
Select product_id, qty from product a where product_id = 167 and
exists (select ‘x’ from items b where b.item_no = a.item_no);`

- 