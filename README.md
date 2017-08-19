# Brainbash
It's brainf***, with two tapes. Each tape has its own pointer, and each cell has its own maximum value. As of yet, one cannot change the cell's maximum value, but that is planned for the future.

## Commands

In addition to the original 8 commands, these are currently implemented:

    `       debug the tapes
    ~       swap tapes
    *       swap tapes and copy pointer position to other tape
    ;       output as a number
    #       input as a number
    {...}   if condition

These commands might also be implemented from the original JS implementation, but they also might not. They are ordered from most to least likely to be implemented.
    @       set cell pointer to value in cell
    x       zero the current cell
    :...:Q  define a macro into symbol Q

->>[<<<.>>>->+<]>[-<+>]<
<{>+<-}<-~.+~