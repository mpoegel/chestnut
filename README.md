# Chestnut

## Building
Requires `fwoop`. Requires `gtest` to build unit tests.

```sh
mkdir build
cd build
cmake ../
cmake --build .
```

### Testing
```sh
cd build
ctest
```

## Release
```sh
dch -v <version> -M
debuilder -us -uc
gh release create
```
