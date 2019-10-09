'''import serial, time
import MySQLdb

ser = serial.Serial('/dev/ttyUSB0',115200)

db = MySQLdb.connect('localhost','root','Tel@123','mote1') or die('could not connect to database')

cursor = db.cursor()

while 1:
    serial_line = ser.readline()
    
    print(serial_line) 
    
    val = serial_line.split('\t')

    print(val[0])
    

    sql = "INSERT INTO HelloWorld(text) VALUES(%s)"

    try:
      #Execute the SQL command
      cursor.execute(sql,val[0])
      #Commit your changes in the database
      mydb.commit()
    except:
      print("Couldn't commit")
      db.rollback()

db.close()
ser.close()'''


#!/usr/bin/python
import serial
import MySQLdb

#establish connection to MySQL. You'll have to change this for your database.
dbConn = MySQLdb.connect("localhost","root","Tel@123","mote1") or die ("could not connect to database")
#open a cursor to the database
cursor = dbConn.cursor()


while True:
  #open a cursor to the database
  cursor = dbConn.cursor()
  pieces = ["Hello, World!"]
  #Here we are going to insert the data into the Database
  try:
    cursor.execute("INSERT INTO HelloWorld (text) VALUES (%s)", [pieces[0]])
    dbConn.commit() #commit the insert
    cursor.close()  #close the cursor
  except MySQLdb.IntegrityError:
    print "failed to insert data"
  finally:
    cursor.close()  #close just incase it failed
