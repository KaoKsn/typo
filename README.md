# typo - A terminal based simple typing software.

typo is a free software licensed under the [GPLv2](https://www.gnu.org/licenses/old-licenses/gpl-2.0.html) license.

----

## Project Structure

    .
    ├── include
    │   └── race.h
    ├── LICENSE
    ├── makefile
    ├── notes.txt
    ├── README.md
    ├── src
    │   └── race.c
    └── test

4 directories, 6 files

----

## Build and Run

```bash
git clone https://github.com/KaoKsn/typo.git
cd typo
make
./build/main

# make clean
```

-----

## Todo.
1. [ ] fix the color issue (red/green) after some backspaces.
2. [ ] use a db to store the information.
3. [ ] improvise the wpm formula to incorporate mistakes. Use a standardized formula
it most likely exists.
4. [ ] randomize test string selection. Maybe read it from a file/db.
