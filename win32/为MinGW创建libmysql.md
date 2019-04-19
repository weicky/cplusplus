# 为MinGW创建libmysql

## 方法1：

* (1) reimp.exe <MySQL\lib\opt>\libmysql.lib，生成liblibmysql.a和LIBMYSQL.def两个文件
* (2) 重命名liblibmysql.a为libmysql.a
* (3) 复制libmysql.a 到LIB目录
* (4) 加入编译选项-lmysql


## 方法2：

* (1) reimp.exe -d <MySQL\lib\opt>\libmysql.lib，生成LIBMYSQL.def
* (2) dlltool.exe -k -d LIBMYSQL.def -l libmysql.a，生成libmysql.a
* (3) 复制libmysql.a 到LIB目录
* (4) 加入编译选项-lmysql