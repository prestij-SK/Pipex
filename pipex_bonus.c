#include "pipex.h"

static void    start_pipex_bonus(int argc, char **argv, char **envp)
{
    PipexVars   p_vars;

    p_vars.arg_idnex = 2;
    if (strncmp(argv[1], "here_doc", 8) == 0)
    {
        p_vars.arg_idnex = 3; // starting iterator if HERE_DOC option
        p_vars.file_out = get_descriptor(argv[argc - 1], 'h');
        here_doc(argv[2], argc);
    }
    else
    {
        p_vars.file_out = get_descriptor(argv[argc - 1], 'o');
        p_vars.file_in = get_descriptor(argv[1], 'i');
        if (dup2(p_vars.file_in, STDIN_FILENO) == -1)
            exit_with_error("Descriptor Duplication Error!\n");
    }
    child_processes(argc, argv, envp, &p_vars);
}

int	main(int argc, char **argv, char **envp)
{
	if (argc >= 5 && argv && envp)
		start_pipex_bonus(argc, argv, envp);
	else
		exit_with_error("Wrong Argument Rules!\n");
	return (0);
}