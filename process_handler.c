#include "pipex.h"

static void	here_doc_get_lines( char *limiter, int *fd)
{
	char	*line;

	if (close(fd[0]) == -1)
		exit_with_error("Pipe close Error!\n");
	line = get_next_line(STDIN_FILENO);
	while (line)
	{
		if (strncmp(line, limiter, ft_strlen(limiter)) == 0)
		{
			free(line);
			exit(EXIT_SUCCESS);
		}
		write(fd[1], line, ft_strlen(line));
		free(line);
		line = get_next_line(STDERR_FILENO);
	}
}

void	here_doc(char *limiter, int argc)
{
	pid_t	pid;
	int		fd[2];

	if (argc < 6)
		exit_with_error("Wrong Argument Rules!\n");
	if (pipe(fd) == -1)
		exit_with_error("Pipe set Error!\n");
	pid = fork();
	if (pid == -1)
		exit_with_error("Process fork Error!\n");
	if (pid == 0)
		here_doc_get_lines(limiter, fd);
	else
	{
		if (close(fd[1]) == -1)
			exit_with_error("Pipe close Error!\n");
		if (dup2(fd[0], STDIN_FILENO) == -1)
			exit_with_error("Descriptor Duplication Error!\n");
		if (waitpid(pid, NULL, 0) == -1)
			exit_with_error("Waitpid Error!\n");
	}
}

static pid_t	in_process(char *cmd, char **envp)
{
    pid_t   pid;
    int     fd[2];

    if (pipe(fd) == -1)
        exit_with_error("Pipe set Error!\n");
    pid = fork();
    if (pid == -1)
        exit_with_error("Process fork Error!\n");
    if (pid == 0)
    {
        if (close(fd[0]) == -1)
            exit_with_error("Pipe close Error!\n");
        if (dup2(fd[1], STDOUT_FILENO) == -1)
            exit_with_error("Descriptor Duplication Error!\n");
        cmd_execute(cmd, envp);
    }
    else
    {
        if (close(fd[1]) == -1)
            exit_with_error("Pipe close Error!\n");
        if (dup2(fd[0], STDIN_FILENO) == -1)
            exit_with_error("Descriptor Duplication Error!\n");
    }
    return (pid);
}

static pid_t    out_process(char *cmd, char **envp, int file_out)
{
    pid_t   pid;
    int     fd[2];

    if (pipe(fd) == -1)
        exit_with_error("Pipe set Error!\n");
    pid = fork();
    if (pid == -1)
        exit_with_error("Process fork Error!\n");
    if (pid == 0)
    {
        if (close(fd[1]) == -1)
            exit_with_error("Pipe close Error!\n");
        if (dup2(file_out, STDOUT_FILENO) == -1)
            exit_with_error("Descriptor Duplication Error!\n");
        cmd_execute(cmd, envp);
    }
    else
    {
        if (close(fd[0]) == -1)
            exit_with_error("Pipe close Error!\n");
        if (dup2(fd[1], STDIN_FILENO) == -1)
            exit_with_error("Descriptor Duplication Error!\n");
    }
    return (pid);
}

void	child_processes(int argc, char **argv, char **envp, PipexVars *p_vars)
{
    pid_t   *all_pid;
    int     pid_i;
    int     status;
    int     i;

    // -3 to ignore allocating space for prog_name, file1, file2.
    // But we could not care about -3, as programm will work correctly even with more allocated space
    all_pid = (pid_t *)malloc(sizeof(pid_t) * (argc - 3));
    if (!all_pid)
        exit_with_error("Allocation Error!\n");
    pid_i = 0;
    i = p_vars->arg_idnex;
    while (i < argc - 2) // -2 to ignore last cmd
    {
        all_pid[pid_i] = in_process(argv[i], envp);
        ++pid_i;
        ++i;
    }
    all_pid[pid_i] = out_process(argv[i], envp, p_vars->file_out);
    ++pid_i;
    i = 0;
    status = 0;
    while (i < pid_i) // Wait for all child processes to finish
    {
        if (waitpid(all_pid[i], &status, 0) == -1)
            exit_with_error("Waitpid Error!\n");
        if (status != 0)
            exit(EXIT_FAILURE);
        ++i;
    }
    free(all_pid);
}