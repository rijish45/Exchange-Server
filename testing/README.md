Functionality Testing
=====================

To test the server program, run the following commands from a virtual machine:

```bash
bash netcat.sh
```

The *netcat.sh* file has the following content:
```bash
#!/bin/bash

netcat vcm-8762.vm.duke.edu 12345 < test5.xml
```
Replace vcm-8762.vm.duke.edu with the name of your virtual machine and redirect the appropriate
xml testing file.



Test 1:
------

```xml
144
<?xml version="1.0" encoding="UTF-8"?>
<create>
  <account id="12345" balance="123"/> 
  <symbol sym="SYM"> 
  <account id="12345">456</account> 
  </symbol>
</create>
```


Response:
---------

```xml
<results>
<created id = "12345"/>
<created sym = "SYM" id = "12345"/>
</results>
```




Test 2:
------

```xml
144
<?xml version="1.0" encoding="UTF-8"?>
<create>
  <account id="1000" balance="1000"/>
  <account id="2000" balance="1000"/>
  <account id="3000" balance="1000"/>
  <account id="4000" balance="1000"/>
  <symbol sym="BTC"> 
  <account id="1000">500</account>
  <account id="2000">2000</account>
  </symbol>  
</create>
```

Response:
---------

```xml
<results>
<created id = "1000"/>
<created id = "2000"/>
<created id = "3000"/>
<created id = "4000"/>
<created sym = "BTC" id = "1000"/>
<created sym = "BTC" id = "2000"/>
</results>
```


Test 3:
------

```xml
<?xml version="1.0" encoding="UTF-8"?>
<create>
  <account id="1000" balance="123"/> 
  <symbol sym="SYM"> 
  <account id="2345">456</account> 
  </symbol>
</create>
```

Response:
---------

```xml
<results>
<error id = "1000"> Account already exists </error>
<created sym = "SYM" id = "2345"/>
</results>
```


Test 4:
------

```xml
144
<?xml version="1.0" encoding="UTF-8"?>
<create>
  <account id="-1300" balance="100"/>
  <account id="1245" balance="-2334"/>
  <symbol sym="H%YM"> 
  <account id="345">456</account>
  <account id="1000">-200</account>
  </symbol>
</create>
```

Response:
---------

```xml
<results>
<error id = "-1300"> Account ID cannot be negative. </error> 
<error id = "1245"> Balance cannot be negative. </error> 
<error sym = "H%YM id = "345"> The symbol of the share should be alphanumeric. </error>
<error sym = "H%YM id = "1000"> The symbol of the share should be alphanumeric. </error>
</results>
```

Test 5:
-------

```xml
144
<?xml version="1.0" encoding="UTF-8"?>
<create>
  <account id="1300" balance="100"/>
  <account id="1245" balance="2334"/>
  <symbol sym="BTC"> 
  <account id="345">456</account>
  <account id="1000">200</account>
  </symbol>
  <symbol sym="HTTC"> 
  <account id="345">456</account>
  <account id="1000">200</account>
  </symbol> 
</create>
```
Response:
---------

```xml
<results>
<created id = "1300"/>
<created id = "1245"/>
<created sym = "BTC" id = "345"/>
<created sym = "BTC" id = "1000"/>
<created sym = "HTTC" id = "345"/>
<created sym = "HTTC" id = "1000"/>
</results>
```





Now we run netcat with specific directions. We quit the program after running a particular test-case, so that the database tables are dropped:

Test:
------

Content of netcat.sh

```bash
#!/bin/bash                                                                            

netcat vcm-8762.vm.duke.edu 12345 < test6.xml
netcat vcm-8762.vm.duke.edu 12345 < test7.xml
netcat vcm-8762.vm.duke.edu 12345 < test8.xml
```


test6.xml
---------
```xml
144
<?xml version="1.0" encoding="UTF-8"?>
<create>
  <account id="100" balance="10000"/>
  <account id="200" balance="12000"/>
  <account id="300" balance="10000"/>
  <account id="400" balance="10000"/>
  <account id="500" balance="10000"/>
  <account id="600" balance="10000"/>
  <symbol sym="BTC"> 
  <account id="100">500</account>
  <account id="200">100</account>
  <account id="300">100</account>
  <account id="400">100</account>
  <account id="500">100</account>
  <account id="600">100</account>
  </symbol>
</create>
```

test7.xml
---------
```xml
123
<?xml version="1.0" encoding="UTF-8"?>
<transactions id="100">
<order sym="BTC" amount="-500" limit="20"/>
</transactions>
```

test8.xml
---------
```xml
122
<?xml version="1.0" encoding="UTF-8"?>
<transactions id="200">
<order sym="BTC" amount="600" limit="20"/>
</transactions>
```


Response:

```xml
<results>
<created id = "100"/>
<created id = "200"/>
<created id = "300"/>
<created id = "400"/>
<created id = "500"/>
<created id = "600"/>
<created sym = "BTC" id = "100"/>
<created sym = "BTC" id = "200"/>
<created sym = "BTC" id = "300"/>
<created sym = "BTC" id = "400"/>
<created sym = "BTC" id = "500"/>
<created sym = "BTC" id = "600"/>
</results>
<results>
<opened sym="BTC" amount="500.000000" limit="20" id ="0"/>
</results>
<results>
<opened sym="BTC" amount="600.000000" limit="20" id ="1"/>
</results>
```




Test:
----

Content of netcat.sh:
```bash
#!/bin/bash                                                                            

netcat vcm-8762.vm.duke.edu 12345 < test6.xml
netcat vcm-8762.vm.duke.edu 12345 < test7.xml
netcat vcm-8762.vm.duke.edu 12345 < test9.xml
netcat vcm-8762.vm.duke.edu 12345 < test10.xml
netcat vcm-8762.vm.duke.edu 12345 < test11.xml
netcat vcm-8762.vm.duke.edu 12345 < test12.xml
netcat vcm-8762.vm.duke.edu 12345 < test13.xml
```

test6.xml
---------

```xml
144
<?xml version="1.0" encoding="UTF-8"?>
<create>
  <account id="100" balance="10000"/>
  <account id="200" balance="12000"/>
  <account id="300" balance="10000"/>
  <account id="400" balance="10000"/>
  <account id="500" balance="10000"/>
  <account id="600" balance="10000"/>
  <symbol sym="BTC"> 
  <account id="100">500</account>
  <account id="200">100</account>
  <account id="300">100</account>
  <account id="400">100</account>
  <account id="500">100</account>
  <account id="600">100</account>
  </symbol>
</create>
```

test7.xml
---------

```xml
123
<?xml version="1.0" encoding="UTF-8"?>
<transactions id="100">
<order sym="BTC" amount="-500" limit="20"/>
</transactions>
```

test9.xml
---------

```xml
122
<?xml version="1.0" encoding="UTF-8"?>
<transactions id="200">
<order sym="BTC" amount="100" limit="20"/>
</transactions>
```

test10.xml
----------
```xml
122
<?xml version="1.0" encoding="UTF-8"?>
<transactions id="300">
<order sym="BTC" amount="100" limit="20"/>
</transactions
```


test11.xml
----------
```xml
122
<?xml version="1.0" encoding="UTF-8"?>
<transactions id="400">
<order sym="BTC" amount="100" limit="20"/>
</transactions>
```

test12.xml
----------
```xml
122
<?xml version="1.0" encoding="UTF-8"?>
<transactions id="500">
<order sym="BTC" amount="100" limit="20"/>
</transactions>
```


test13.xml
----------
```xml
122
<?xml version="1.0" encoding="UTF-8"?>
<transactions id="600">
<order sym="BTC" amount="100" limit="20"/>
</transactions>
```

Response:
--------


```xml

<results>
<created id = "100"/>
<created id = "200"/>
<created id = "300"/>
<created id = "400"/>
<created id = "500"/>
<created id = "600"/>
<created sym = "BTC" id = "100"/>
<created sym = "BTC" id = "200"/>
<created sym = "BTC" id = "300"/>
<created sym = "BTC" id = "400"/>
<created sym = "BTC" id = "500"/>
<created sym = "BTC" id = "600"/>
</results>
<results>
<opened sym="BTC" amount="500.000000" limit="20" id ="0"/>
</results>
<results>
<opened sym="BTC" amount="100.000000" limit="20" id ="1"/>
</results>
<results>
<opened sym="BTC" amount="100.000000" limit="20" id ="2"/>
</results>
<results>
<opened sym="BTC" amount="100.000000" limit="20" id ="3"/>
</results>
<results>
<opened sym="BTC" amount="100.000000" limit="20" id ="4"/>
</results>
<results>
<opened sym="BTC" amount="100.000000" limit="20" id ="5"/>
</results>

```




Test:
-----

content of netcat.sh

```bash
#!/bin/bash                                                                            

netcat vcm-8762.vm.duke.edu 12345 < test6.xml
netcat vcm-8762.vm.duke.edu 12345 < test15.xml

```

test15.xml
----------
```xml
94
<?xml version="1.0" encoding="UTF-8"?>
<transactions id="200">
<order sym="BTC" amount="-100" limit="20"/>
<query id="1"/>
</transactions>
```

Response:
---------

```xml
<results>
<created id = "100"/>
<created id = "200"/>
<created id = "300"/>
<created id = "400"/>
<created id = "500"/>
<created id = "600"/>
<created sym = "BTC" id = "100"/>
<created sym = "BTC" id = "200"/>
<created sym = "BTC" id = "300"/>
<created sym = "BTC" id = "400"/>
<created sym = "BTC" id = "500"/>
<created sym = "BTC" id = "600"/>
</results>
<results>
<opened sym="BTC" amount="100.000000" limit="20" id ="0"/>
<status id="1">
</status>
</results>
```






