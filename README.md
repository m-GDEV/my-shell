# my-shell

Very simple shell implementation.

I wrote this project quickly to just learn how shells work at a basic level.

Once I learned what exec and fork were it was pretty obvious what the basic model of
shells were.

## I might add more features in the future but currently in **my-shell** you can:

-   run any command in your `$PATH`
-   use '.' as directories
-   exit with: exit, Exit, bye, or e
-   change directories with `cd` (including home using `~`)
-   enter empty command
-   report return code of commad

## Planned features:

-   `time` (maybe)
-   aliases
    -   format (for now?): 'alias>gstat>git diff'
    -   parsing steps:
        -   check if first part of string is 'alias'
        -   add gstat to global list (in memory) of aliases
    -   when the command is entered, first check if it matches any of the aliases, if not the run it as normal
    -   if it is in the list of aliases, convert that string the same we do of buf
-   sigint hanlder to exit gracefully
