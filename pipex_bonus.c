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

static void	here_doc(char *limiter, int argc)
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

static void	child_process(char *cmd, char **envp)
{
	pid_t	pid;
	int		fd[2];

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
		if (waitpid(pid, NULL, 0) == -1)
			exit_with_error("Waitpid Error!\n");
	}
}

static void	child_processes(int argc, char **argv, char **envp, int i)
{
	pid_t	*all_pid;
	// pid_t	pid;
	// int		child_i;
	// int		status;

	all_pid = (pid_t *)malloc(sizeof(pid_t) * (argc - 3));
	if (!all_pid)
		exit_with_error("Allocation Error!\n");
	// child_i = 0;
	while (i < argc - 2)
	{
		// pid = fork();
		// if (pid == -1)
		// {
		// 	free(all_pid);
		// 	exit_with_error("Process fork Error!\n");
		// }
		// else if (pid == 0)
		// {
			child_process(argv[i], envp);
		// 	exit(EXIT_SUCCESS);
		// }
		// else
		// {
		// 	all_pid[child_i] = pid;
		// 	++child_i;
		// }
		++i;
	}
	// i = 0;
	// while (i < child_i)
	// {
	// 	if (waitpid(all_pid[i], &status, 0) == -1)
	// 		exit_with_error("Waitpid Error!\n");
	// 	if (status != 0)
	// 		exit(EXIT_FAILURE);
	// 	++i;
	// }
	free(all_pid);
}

static void	start_pipex_bonus(int argc, char **argv, char **envp)
{
	int	file_in;
	int	file_out;
	int	i;

	i = 2;
	if (strncmp(argv[1], "here_doc", 8) == 0)
	{
		i = 3; // starting iterator if HERE_DOC option
		file_out = get_descriptor(argv[argc - 1], 'h');
		here_doc(argv[2], argc);
	}
	else
	{
		file_out = get_descriptor(argv[argc - 1], 'o');
		file_in = get_descriptor(argv[1], 'i');
		if (dup2(file_in, STDIN_FILENO) == -1)
			exit_with_error("Descriptor Duplication Error!\n");
	}
	child_processes(argc, argv, envp, i);
	if (dup2(file_out, STDOUT_FILENO) == -1)
		exit_with_error("Descriptor Duplication Error!\n");
	cmd_execute(argv[argc - 2], envp);
}

int	main(int argc, char **argv, char **envp)
{
	if (argc >= 5 && argv && envp)
		start_pipex_bonus(argc, argv, envp);
	else
		exit_with_error("Wrong Argument Rules!\n");
	return (0);
}