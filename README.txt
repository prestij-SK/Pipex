Pipex
42 School project (PIPEX)

---Mandatory---
Pipex is a project that re-creates in C the way two commands are piped together via | in the shell.
Example:
        < f1.txt cat | wc -l > f2.txt <

The result of given example will be number of lines from f1.txt file, which will be written in f2.txt.
The program works by calling it like:
        ./pipex f1.txt cat "wc -l" f2.txt

---Bonus part---
The project also has bonus part which is implementation of the same piping but with the support of multiple pipes.
Example:
        < f1.txt cat | wc -l | cat | wc -w > f2.txt

In this example Shell will run 4 commands and place the result in f2.txt file.
The program works by calling it like:
        ./pipex_bonus f1.txt cat "wc -l" cat "wc -w" f2.txt

---Here Doc---
The bonus part also includes Here documents (often abbreviated as "here docs"), a way to pass multiple lines of input to a command or program.
Example:
        cat << LIMITER | wc -l >> f1.txt
        this is
        some
        random text
        LIMITER

Running this in shell will take the given input from start till given delimiter (LIMITER in this case) and after calling cat | wc -l commands
will append the result in f1.txt.
The program works by calling it like:
        ./pipex_bonus here_doc ABUBA cat "wc -l" f1.txt

Where 'here_doc' must be written and ABUBA is the delimiter of the input.
The here_doc program also supports multiple lines too.
