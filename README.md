# Operating Systems Assignment Three


## Notes

### parse.h
I decided to implement this a bit differently. After some thought, one of my goals is to 
do away with pointers as much possible. Now, we could still implement the program options
with the struct. However, this entirely does away with data encapsulation. So I'm trying
using a singleton design with methods that return this static data. I'll decide how much
I like this approach by the end of the project. If nothing else, it's easier to read than
pointers, and only marginally harder than returning struct copies. 


### Outstanding Questions
- Will I need to keep some reference to all child pid_ts? as of right now, I don't *think*
so. This could change.