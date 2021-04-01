# osd-cw-2
Operating Systems &amp; Drivers Coursework 2

## MySQL Database

In docker environment, use the ```mysql -u root -p``` command to launch the MySQL database and type the password provided.

Create the database:

```mysql> CREATE DATABASE w1646608;```

To show databases:

```mysql> SHOW DATABASES;```

Use the database:

```mysql> USE w1646608;```

Show tables: 

```mysql> SHOW TABLES;```

Create table with numbers field:

```mysql> CREATE TABLE numbers (number_id int auto_increment primary key, number varchar(2) NOT null);```

Show all data in a table:

```mysql> SELECT * FROM numbers;```

Show table structure:

```mysql> DESCRIBE numbers;```

Show numbers table content:

```mysql> SELECT * FROM numbers;```

Insert data:

```mysql> INSERT INTO numbers(number) values ('22')```


## Launch the Python Virtual Environment

All python modules and pakcages are already installed in the virtual enviroment. Launch the virtual environment with:

```source /python/bin/activate```

This will cause the enviroment to be shown in brackets indicating the virtual environment is active.

Example: ```(python) [null@arch osd-cw-2]$```

From this point on, run any of the files. The virtual environment containts the needed library to run.


