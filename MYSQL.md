# 上传MYSQL数据库

## 1.创建表

两个字段bookname，bookid。使用查找bookid以找到bookid

结果：![72009635800](C:\Users\CUISHU~1\AppData\Local\Temp\1720096358008.png)

![72009637175](C:\Users\CUISHU~1\AppData\Local\Temp\1720096371751.png)

## 2.QT连接MYSQL

```
 QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
```

```
        db.setHostName("8.147.233.239");
```

```
        db.setPort(3306);
```

```
        db.setDatabaseName("cst"); //不是Database,记得别填错
```

```
        db.setUserName("root");
```

```
        db.setPassword("team2111");
```

```
        bool ok = db.open();
```

```
        if (ok)
```

```
        {
```

```
            QMessageBox::information(this, "infor", "link success");
```

```
        }
```

```
        else
```

```
        {
            QMessageBox::information(this, "infor", "link failed");
        }
```

使用ODBC连接mysql

![72009663180](C:\Users\CUISHU~1\AppData\Local\Temp\1720096631807.png)

结果：![72009676779](C:\Users\CUISHU~1\AppData\Local\Temp\1720096767799.png)