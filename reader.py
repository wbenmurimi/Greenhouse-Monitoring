import serial
import sqlite3
import MySQLdb

db = MySQLdb.connect("localhost", "root", "pi", "greenhouse")

ser = serial.Serial('/dev/ttyUSB0', 9600)

while True:
  val=ser.readline()
  print val

  # Split the values: [temp, humidity, moisture, ph]
  vals = val.split(",")
  print vals
  
  # Length must be 4
  if ( len(vals) == 4 ):
     # save to db
     c = db.cursor()
     c.execute( """INSERT INTO greenhouse(temperature,humidity,moisture,ph,greenhouse)
	     VALUES(%s,%s,%s,%s,%s)""", (vals[0],vals[1],vals[2],vals[3],1,) )
     db.commit()

