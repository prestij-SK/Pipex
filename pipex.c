#include "pipex.h"

// ./pipex f1 cmd1 =>cmd2<= =>f2<=
static void	parent_process(char **argv, char **envp, int *pipe, int child_status)
{
	int	descriptor;

	if (child_status != 0)
		exit(EXIT_FAILURE);
	// O_CREAT - create if doesn't exist
	// O_TRUNC - empty it if exists
	descriptor = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC);
	if (descriptor == -1)
	{
		perror("File opening Error!\n");
		exit(EXIT_FAILURE);
	}
	if (close(pipe[1]) == -1)
	{
		perror("Pipe close Error!\n");
		exit(EXIT_FAILURE);
	}
	if (dup2(pipe[0], STDIN_FILENO) == -1)
	{
		perror("Descriptor Duplication Error!\n");
		exit(EXIT_FAILURE);
	}
	if (dup2(descriptor, STDOUT_FILENO) == -1)
	{
		perror("Descriptor Duplication Error!\n");
		exit(EXIT_FAILURE);
	}
	if (close(pipe[0]) == -1)
	{
		perror("Pipe close Error!\n");
		exit(EXIT_FAILURE);
	}	
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
	if (close(pipe[0]) == -1)
	{
		perror("Pipe close Error!\n");
		exit(EXIT_FAILURE);
	}
	if (dup2(pipe[1], STDOUT_FILENO) == -1)
	{
		perror("Descriptor Duplication Error!\n");
		exit(EXIT_FAILURE);
	}
	if (dup2(descriptor, STDIN_FILENO) == -1)
	{
		perror("Descriptor Duplication Error!\n");
		exit(EXIT_FAILURE);
	}
	if (close(pipe[1]) == -1)
	{
		perror("Pipe close Error!\n");
		exit(EXIT_FAILURE);
	}
	cmd_execute(argv[2], envp);
}

static void	start_pipex(char **argv, char **envp)
{
	int		fd[2];
	pid_t	pid; // we could use simple 'int', but pid_t will handle for every type of OS
	int		status;

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
	if (pid == 0)
		child_process(argv, envp, fd);
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("Waitpid Error!\n");
		exit(EXIT_FAILURE);
	}
	parent_process(argv, envp, fd, status);
}

int main(int argc, char **argv, char **envp)
{
	if (argc == 5)
		start_pipex(argv, envp);
	else
		ft_putstr_fd("Wrong Argument Rules!\n", 2);
	return (0);
}
