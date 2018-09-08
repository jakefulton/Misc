## life.c

Implementation of John Conway's Game of Life using C and ncurses library.

```
gcc life.c -o life -lncurses
./life
```

## rename.py

Rename .mp3 files under a root directory.
Name will contain: artist, album, track#, and title.

```
import rename
root_dir = '/home/fultonj/Music'
rename.rename_mp3_files(root_dir, 1, 2, 3, 4)
```

## blackjack.c

Play blackjack against the computer.
Start out with $20 and keep betting until you run out.

## lss.c

Implement "ls -l" sorted by decreasing byte count in C.

## lss.sh

Similar to lss.c but implemented in bash.
Also accepts other options that are available to ls (like -r, or --all).
Additional arguments are interpreted as files

```
chmod 755 lss.sh
bash lss.sh [option]... [file]...
```

## every.c

C filter that prints M lines out of every N for a file, or list of files
Default for both N and M is 1.

```
./every [-N,M] [list-of-files]
```
