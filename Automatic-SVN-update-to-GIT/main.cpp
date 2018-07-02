#include <iostream>


std::string repositoryPath = "D:/MOMAKaKao/trunk";
const int MAX_LENGTH = 300;

std::string getCurrentRevision()
{
	FILE* file;
	char output[MAX_LENGTH];
	std::string command = "svn info --show-item last-changed-revision ";
	command.append(repositoryPath);

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

int main(void)
{
	std::string currentRevision;
	currentRevision = getCurrentRevision();

	return 0; 
}