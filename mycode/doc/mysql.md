# DataTypes
## varchar and char (store engine is InnoDB or MyISAM)
+ varchar
varchar类型用于存储可变长字符串，是最常见的字符串数据类型。它比定长类型更节省空间，它仅使用必要的空间（除非： MYSQL表使用ROW_FORMAT=FIXED创建的话，每一行都会使用定长存储，会浪费空间）
varchar需要使用1或2个额外字节记录字符串的长度，如果列最大长度<=255B，只使用1个字节，否则使用2个字节。
使用场景： 1.字符串列的最大长度比平均长度大很多；2. 列的更新很少，所以碎片不是问题；3.使用了像UTF-8这样复杂的字符集，每个字符都使用不同的字节数进行存储。
+ char
char类型是定长的：MYSQL总是根据定义的字符串长度分配足够的空间。当存储CHAR值时，MYSQL会删除所有的末尾空格（在MYSQL4.1和更老版本中VARCHAR也是这样实现的，即在这些版本中他们的区别只是在存储格式上）
使用场景：CHAR适合存储很短的字符串，或者所有值都接近同一个长度。不易产生碎片。例如用CHAR(1)来存储只有Y和N的值，如果采用单字节字符集只需要一个字节，但是VARCHAR(1)却需要两个字节，另一个字节记录长度。
    ```mysql
        > create table char_test(char_col CHAR(10));
        > insert into char_test(char_col) values ('str1'), (' str2'), ('str3 ');
    ```

    // CHAR会自动删除str3后的空格
    ```mysql
    > select CONCAT("'", char_col, "'") FROM char_test;

        results:
        'str1'
        ' str2'
        'str3'
    ```
# MYSQL Operation
+ 一般来说mysql的binlog在目录/var/lib/mysql下， mysql命令行进入:
  ```mysql
  ]# mysql -u root -p

  # remove logs before binlog.000911
  mysql> PURGE BINARY LOGS TO 'binlog.000911';

  # remove logs over 7 days
  mysql> SET GLOBAL expire_logs_days = 7;
  ```
  + 可以设置binlog生成的路径：
  ```shell
    ]$ sudo systemctl stop mysql
    # or
    ]$ sudo service mysql stop

    # move current binlog
    ]$ sudo mv /var/lib/mysql/binlog.*  /new/path/binglog

    # modify mysql config file
    # cd /etc/mysql/my.cnf or /etc/my.cnf
     vi /etc/mysql/my.cnf
    [mysqld]
    log-bin = /new/path/binglog

     # start mysql server
     ]$ sudo systemctl start mysql
     # verify the modification
     ]$ show VARIABLES LIKE 'log_bin_basename';
  ```
