
<!-- vim-markdown-toc GFM -->

* [关键字](#关键字)
* [增删改查](#增删改查)
	* [select](#select)
* [高级](#高级)
	* [高级查询](#高级查询)
	* [聚合](#聚合)
	* [事务](#事务)
	* [　隔离级别](#隔离级别)
* [运维](#运维)
	* [高可用](#高可用)
	* [主备](#主备)
	* [分库分表](#分库分表)
	* [集群](#集群)
	* [故障恢复](#故障恢复)

<!-- vim-markdown-toc -->
## 关键字
1. select/ insert/ delete/ update
2. create table/ drop table/ alter table/ 
3. create view/ drop view/
4. create index/ drop index
5. create procedure/ drop procedure
6. create tirgger/ drop trigger
7. create schema/ drop schema
8. create domain/ drop domain/ alter domain

9. grant: 授权/  deny: 拒绝用户访问/ revoke:接触用户访问权限

10. commit/ rollback/ set transaction: 定义当前事务数据访问特征.

11. declare: 为查询设定游标/ explan/ open: 检索查询结果打开一个游标

12. fetch: 检索一行查询结果/ close/ prepare: 为动态执行准备SQL语句
13. execute:动态的执行SQL语句/ describe: 描述准备好的查询

## 增删改查
### select
1. select * from table_name where column_name operator value

2. select * from stock where stock_id = str(id)
	- stockname = 'str_name'
	- stockname like '% find this%'
	- stockname like '[a-zA-Z]%' ([]指定值的范围)
	- stockname like '[^F-M]%' (^排除指定范围)
		1. 只能在使用like关键字的where子句中使用通配符
	- or stockpath='stock_path'
	- or stocknumber < 1000
	- and stockindex = 23
	- not sock='man'
	- stocknum between 20 and 100
	- stocknum in (10, 20, 30)

	- order by stockid desc(asc)
	- order by 1, 2  (by 列号)

	- stockname = (select stockname from stock_in where stockid=4)
		1. 除非能确保内层select只返回一个行的值,否则应在外层where子句中用一个in限定符.

	- select distinct column_name from table_name;

## 高级
### 高级查询
### 聚合
1. having 只用在group后的聚合的条件,那having是否只能跟count(*),avg等聚合函数?
### 事务

###　隔离级别



## 运维
### 高可用
### 主备
### 分库分表
### 集群
### 故障恢复

