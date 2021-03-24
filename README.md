# osd-cw-2
Operating Systems &amp; Drivers Coursework 2

## MySQL Database

In docker environment, use the ```mysql -u root -p``` command to launch the MySQL database and type the password provided.

Create the database:

```mysql> CREATE DATABASE w1646608;```

Use the database:

```
mysql> USE w1646608;
Database changed
```

## Launch the Python Virtual Environment

All python modules and pakcages are already installed in the virtual enviroment. Launch the virtual environment with:

```source /python/bin/activate```

This will cause the enviroment to be shown in brackets indicating the virtual environment is active.

Example: ```(python) [null@arch osd-cw-2]$```


## Test Python Connectivity with MySQL Database

Inside ```/osd-cw-2/python/code``` , run the ```main.py``` file with ```python3 main.py```. 

If no error is shown, python is able to connect to the database. Else, something went wrong. 



