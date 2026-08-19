# postgres_1

 Version: 0.9.1

 date    : 2026/08/19
 
 update :

***

C++ Drogon , API Server Postgres

***
### related

https://github.com/drogonframework/drogon

https://drogonframework.github.io/drogon-docs/#/ENG/ENG-02-Installation

***
* LIB add
```
sudo apt-get update
sudo apt-get install libpq-dev
sudo apt install nlohmann-json3-dev
```

***
* setup
* .env

```
DATABASE_URL="host=localhost port=5432 dbname=mydb user=root password=admin"
```
***

* build

```
cd build
cmake ..
make
```

* start
* localhost:5555

```
./build/postgres_1
```
***
* Test-code

* add
```
curl -X POST -H "Content-Type: application/json" \
 -d '{"title": "test-12"}' \
 http://localhost:5555/api/todo/create

```

* delete
```
curl -X POST -H "Content-Type: application/json" \
 -d '{"id": 4}' \
 http://localhost:5555/api/todo/delete
```

* list
```
curl http://localhost:5555/api/todo/list
```

***