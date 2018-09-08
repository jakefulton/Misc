## life.c

Implementation of John Conway's Game of Life using C and ncurses library.

```
gcc life.c -o life -lncurses
./life
```

## blackjack.c

Play blackjack against the computer.
Start out with $20 and keep betting until you run out.

```
gcc blackjack.c -o blackjack
./blackjack
```

## lss.c

Implement "ls -l" sorted by decreasing byte count in C.

```
gcc lss.c -o lss
./lss
```

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
gcc every.c -o every
./every [-N,M] [list-of-files]
```
