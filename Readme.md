## A Tool for Decoding Wallpaper Engine's mpkg Files

### Overview
`unmpkg` is a program written in C language for decoding `.mpkg` files of Wallpaper Engine. This program adheres to the principles of free software and is released under the GNU General Public License (GPL).

### Build it
```shell
gcc unmpkg.c -o unmpkg
```

### Functional Description
- The program provides a command-line interface, accepting at least one argument: the name of the `.mpkg` file.
- It supports reading the version information, number of files, and the name and size of each file within the `.mpkg` file.
- It is capable of recursively creating directories and extracting resources from the `.mpkg` file into the corresponding directory structure.

### Usage
```text
Usage: unmpkg <filename>
A tool for decoding the wallpaper engine's mpkg file.
Example: unmpkg test.mpkg
```
### Others
If you want to decode '.tex' file,you can see [repkg](https://github.com/notscuffed/repkg).
