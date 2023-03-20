# simple_archiver
A simple .tar archiver for my OS class

To compile the code:
```
make -f MakeFile
```

### Supported commands:
ℹ️ Any command with an incorrect amount of arguments is interpreted as a help command 

#### Archiving:
```
./ttar -a <dirname> <newtarname>
```

#### Unpacking:
```
./ttar -r <tarname> <newdirname>
```
