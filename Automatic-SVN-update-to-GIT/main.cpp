#include <iostream>


std::string repositoryPath = " D:/MOMAKaKao/trunk";
const int MAX_LENGTH = 300;

std::string getCommandResult(std::string &command)
{
	FILE* file;
	char output[MAX_LENGTH];

	file = _popen(command.c_str(), "r");
	if (file == NULL)
	{
		perror("erro : ");
		exit(0);
	}

	while (fgets(output, MAX_LENGTH, file) != NULL)
	{
		printf("%s", output);
	}

	return output;
}

std::string getCurrentRevision()
{
	std::string command = "svn info --show-item last-changed-revision";
	command.append(repositoryPath);

	return getCommandResult(command);
}

void getLogList(std::string currentRevision)
{
	std::string command = "svn log --xml -r ";
	command.append(currentRevision);
 	command.append(":HEAD ");
 	command.append(repositoryPath);

	getCommandResult(command);
}


int main(void)
{
	std::string option = "chcp 65001";
	getCommandResult(option);

	std::string currentRevision;
	currentRevision = getCurrentRevision();
	currentRevision.pop_back();

	getLogList(currentRevision);

	return 0; 
}