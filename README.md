# Operating Systems Assignment Three
Program to spawn children. The children will select a
random integer and use that to decide when to terminate.
Usage: `./oss [-h] [-t -i -o] [dirname]`

For: 

    -h: Display help and exit.
    -t: Amount of time to allow the program to run for.
    -l: Log file path.
    -s: Maximum child processes.

## Build
This requires building two files. Clean with `make clean`. Then, you can build the
master program with `make oss`, and the child with `make user`.

## Notes

### parse.h
I decided to implement this a bit differently. After some thought, one of my goals is to 
do away with pointers as much possible. Now, we could still implement the program options
with the struct. However, this entirely does away with data encapsulation. So I'm trying
using a singleton design with methods that return this static data. I'll decide how much
I like this approach by the end of the project. If nothing else, it's easier to read than
pointers, and only marginally harder than returning struct copies. 

I've decided I mostly like this approach, but will start making some tweaks with project
four.

### Random
It's possible there are bugs I don't know about, or
things out of spec, but as of right now, everything
appears to be running properly. 

## Lessons Learned
- I hate the design of a partitioned system clock. It's far easier to just work in a 
mock nano-second design, then have methods to get the seconds and nano appropriately.
- The modular design is great, but we have to recall that we don't have namespaces.
Worth nothing - namespaces are one honking great idea -- let's do more of those!