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
debuild -us -uc
gh release create
gh release upload <version> ../chestnut_<version>_amd64.deb ../chestnut_<version>.tar.gz ../chestnut_<version>.dsc
dch -r
```
