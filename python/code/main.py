import mysql.connector

try:
   cnx = mysql.connector.connect(user='root', password='w1646608', host='192.168.0.2', port='49162', database='w1646608')
   cnx.close()
except mysql.connector.Error as err:
   print("Something went wrong: {}".format(err))

