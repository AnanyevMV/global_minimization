# Minimization by gradient descents.



# Build Project

### Docker
```sh
docker-compose up -d
docker exec -it cmake_project /bin/bash
```

To build project
```sh
./build.sh init
./build.sh build
```

For dev testing
```sh
./build.sh dev
```

To delete trash
```sh
./build.sh null
```

All logs will be in folder _build_.

# Project struct
* _doc_ - Folder for documents
* _include_ - *.h; header files
* _src_ - *.cpp; cpp files
* _test_ - files for test
* _tmp_ - temporary files
