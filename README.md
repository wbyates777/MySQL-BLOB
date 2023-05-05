# MySQL-BLOB


An example of a simple object database using binary large objects (BLOBs) built on MySQL.
The demo shows how to stream an arbitrary c++ class to and from a MySQL database.
The code has been compiled and tested under the c++11, c++14, c++17 and c++20 standards, and MySQL 8.0.29. 

The demo employs a Trade object which serves as a non-trivial example class.
The Trade class inherits from a Persistent class which, assuming certain read/write functions are defined,
allows a Trade instance to be efficiently streamed as a binary or text string. 
This string can then be read from, or written to, a BLOB field in the database
using a single SQL statement.

Trades are typically identified using unique numeric or text codes that are assigned when they are created.
For the case where our persistent objects do not have a natural unique identifier we can
construct one from the object's contents using the MD5 hash function.

The code can be easily extended to save and load multiple objects (of various types) as a single (albeit large) SQL statement.
In practice, for very large numbers of objects, it is preferable to batch them into a number of smaller separate SQL statements.

Another simple extension would be to maintain an additional Trade table in the database.
The Trade table would be updated whenever a Trade instance was saved (or deleted) as an object.
The table would contain select query criteria, such as the trade's unique object identifier, and fields such as 
the trade type, maturity date, or city. 
These auxiliary tables facilitate the selection and loading of (sets of) objects using SQL statements.
 

For further reading see 

https://en.wikipedia.org/wiki/Object_database

https://en.wikipedia.org/wiki/Binary_large_object

https://en.wikipedia.org/wiki/MD5

 
  


