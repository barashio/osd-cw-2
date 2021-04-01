import mysql.connector
numbersString = "11,22,33,"
numbersArray = numbersString.split(",") # seperate string into array
del numbersArray[-1] # delete last empty value in array from comma seperation
try:
    connection = mysql.connector.connect(user='root', password='w1646608', host='192.168.0.2', port='49162', database='w1646608') # connect to my docker MySQL DB
    cursor = connection.cursor() # initialise cursor
    cursor.execute("DELETE FROM numbers;") # empty DB
    for number in (numbersArray): cursor.execute("INSERT INTO numbers(number) VALUES ('%s')" % number) # insert into DB
    connection.commit() # commit changes to DB
    cursor.execute("SHOW TABLES") # get table nabe
    for (table_name,) in cursor: print(table_name) # print table name
    cursor.execute("SELECT number FROM numbers") # get table data 
    for (columnt_value,) in cursor: print(columnt_value) # show table data
    connection.close() # end connection
except mysql.connector.Error as err: print("Something went wrong: {}".format(err)) # catch any error

