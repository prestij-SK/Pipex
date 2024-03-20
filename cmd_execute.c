#include "pipex.h"

static char *path_result(char *cmd, char **paths)
{
	char	*cmd_path;
	char	*temp;
	int		i;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		cmd_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(cmd_path, F_OK) == 0) // check if file is accessible
			return (cmd_path);
		free(cmd_path);
		++i;
	}
	return (NULL);
}

static char	*check_path(char *cmd, char **envp)
{
	char	**paths;
	char	*cmd_path;
	int		i;

	i = 0;
	while (envp[i] && !ft_strnstr(envp[i], "PATH=", 5))
		++i;
	if (envp[i] == '\0')
	{
		perror("ENVP Error!\n");
		exit(EXIT_FAILURE);
	}
	// + 5 to avoid "PATH="
	paths = ft_split(envp[i] + 5, ':');
	cmd_path = path_result(cmd, paths);
	ft_free_split(paths);
	return (cmd_path);
}

void	cmd_execute(char *cmd, char **envp)
{
	char	**parsed_cmd;
	char	*cmd_path; // path where command + directory path are located

	// We seperate the real command name from it's arguments like 'wc' from 'wc -l'
	parsed_cmd = ft_split(cmd, ' ');
	cmd_path = check_path(parsed_cmd[0], envp);
	if (!cmd_path)
	{
		ft_free_split(parsed_cmd);
		perror("Command Error!\n");
		exit(EXIT_FAILURE);
	}
	if (execve(cmd_path, parsed_cmd, envp) == -1)
	{
		ft_free_split(parsed_cmd);
		free(cmd_path);
		perror("Execution Error!\n");
		exit(EXIT_FAILURE);
	}
}
