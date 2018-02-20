sandbox3d-editor
=========

### Build

Compile application :
```
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
```

### Dependencies

This project is based on Qt5 (>= 5.4).

By design, this project can't render anything by itself. Instead it loads an external plugin that will provide render passes that will do the rendering. An example of such a plugin can be found here : [sandbox3d](https://github.com/magestik/sandbox3d).

