###Add_money tool instruction
+ First:
    - git clone http://git-config.wx.bc/DB_tools/Insert_client.git 
    - ]# cd config
    - ]# vi clientinfo.txt(add users who you want to recharge)
    (example: 0000000023 autoMZ123232@qq.com)
    - ]# vi instruinfo.txt(add coins)
    (example:BTC)
    - ]# vi ods.cnf(to edit the address & path)

+ Second:
    - check the version of python(at least 3.0)
+ Third:
    - ]# cd ../bin
    - ]# check data of these tables(t_member_client_apply,t_client_info,t_contact_info,t_trade_user,t_user_trader,t_traderstatus),then don't remove data of these tables;
    ==[don't remove the record whose client_id = 00000000]==
    - [passwd of users you charge money for:1qaz@WSX]
    - ]# python client_info.py
    - ]# python DBConnect_server.py
+ Forth:
    - if the two python files above execute unsuccessfully,then:
    - ]# cd ../tmp
    - ]# check the to_tradingaccount table,if there are data left,clean these data,then run the next step
    - ]# mysql -uroot -p123456 -h172.16.68.73 -P3306 < account.sql.20180930

+ attention:
    - MYSQL_DB: desc [table_name](denote every field_name and property)
    