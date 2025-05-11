# Minesweeper with Dynamic Generator

## How to Build the Project

### Step 0: Prerequisities

It is assumed `cmake` is working on your machine, you have this `repository` successfully cloned and your current working directory is `farkasm2/project`. On Windows, make sure you have `vcpkg` all set and running too.

### Step 1: Install SFML & it's dependencies

If you have `SFML` library along with it's `dependencies` installed on your machine, skip to the step 2. If it is not the case, install them using the following commands:

On Windows:
```
vcpkg install sfml
```
```
vcpkg integrate install
```

On Fedora:
```
sudo dnf install -y SFML-devel
```

On Debian:
```
sudo apt install libsfml-dev
```

### Step 2: Build the executable

After you've successfully done the previous steps all you have to do now is to change your working directory to `build` and finish the building process. Final executable is called `Minesweeper`.

All commands are shared across both Windows and Linux:
```
mkdir build
```
```
cd build
```
```
cmake -DCMAKE_BUILD_TYPE=Release ..
```
```
cmake --build .
```

The executable `Minesweeper` should now be fully built and completely ready to start.
