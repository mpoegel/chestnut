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
dch -v <version> -M -r
debuilder -us -uc
gh release create
gh release upload v0.1.0 ../chestnut_<version>_amd64.deb ../chestnut_<version>.tar.gz ../chestnut_<version>.dsc
```
