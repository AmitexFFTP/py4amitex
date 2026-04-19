
```
pip install .
```

## Developping and testing

```
cmake -S. -Bbuild -DCMAKE_INSTALL_PREFIX=YOUR_INSTALL_DIR
cmake --build build -j4
cmake --install build
```

python modules are in `YOUR_INSTALL_DIR/lib`.

Tests:
```
PYTHONPATH=<YOUR_INSTALL_DIR> pytest
```
