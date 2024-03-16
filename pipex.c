#include "pipex.h"

// ./pipex f1 cmd1 =>cmd2<= =>f2<=
static void	parent_process(char **argv, char **envp, int *pipe)
{
	int	descriptor;

	// O_CREAT - create if doesn't exist
	// O_TRUNC - empty it if exists
	descriptor = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC);
	if (descriptor == -1)
	{
		perror("File opening Error!\n");
		exit(EXIT_FAILURE);
	}
	close(pipe[1]);
	dup2(pipe[0], STDIN_FILENO);
	dup2(descriptor, STDOUT_FILENO);
	close(pipe[0]);
	cmd_execute(argv[3], envp);
}

// ./pipex =>f1<= =>cmd1<= cmd2 f2
static void	child_process(char **argv, char **envp, int *pipe)
{
	int	descriptor;

	descriptor = open(argv[1], O_RDONLY);
	if (descriptor == -1)
	{
		perror("File opening Error!\n");
		exit(EXIT_FAILURE);
	}
	close(pipe[0]);
	dup2(pipe[1], STDOUT_FILENO);
	dup2(descriptor, STDIN_FILENO);
	close(pipe[1]);
	cmd_execute(argv[2], envp);
}

static void	start_pipex(char **argv, char **envp)
{
	int		fd[2];
	pid_t	pid; // we could use simple 'int', but pid_t will handle for every type of OS

	if (pipe(fd) == -1)
	{
		perror("Pipe set Error!\n");
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("Process fork Error!\n");
		exit(EXIT_FAILURE);
	}
	// We can avoid placing else here, because when child process is called,
	// it will call other program, which will replace the current (before calling the other program).
	if (pid == 0)
		child_process(argv, envp, fd);
		// pid_t waitpid(pid_t pid, int *status, int options);
	waitpid(pid, NULL, 0);
	parent_process(argv, envp, fd);
}

int main(int argc, char **argv, char **envp)
{
	if (argc == 5)
		start_pipex(argv, envp);
	else
		ft_putstr_fd("Wrong Argument Rules!\n", 2);
	return (0);
}