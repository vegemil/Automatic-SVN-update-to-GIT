#include <iostream>
#include <vector>
#include "tinyxml2.h"

std::string repositoryPath = " D:/MOMAKaKao/trunk";
const int MAX_LENGTH = 300;

std::string getCommandResult(std::string &command)
{
	FILE* file;
	char output[MAX_LENGTH];
	std::string xml;

	file = _popen(command.c_str(), "r");
	if (file == NULL)
	{
		perror("erro : ");
		exit(0);
	}

	while (fgets(output, MAX_LENGTH, file) != NULL)
	{
		printf("%s", output);
		xml.append(output);
		xml.pop_back();
	}

	return xml;
}

std::string getCurrentRevision()
{
	std::string command = "svn info --show-item last-changed-revision";
	command.append(repositoryPath);

	return getCommandResult(command);
}

void getLogList(std::string currentRevision, std::string& xml)
{
	std::string command = "svn log --xml -r ";
	command.append(currentRevision);
 	command.append(":HEAD ");
 	command.append(repositoryPath);

	xml = getCommandResult(command);
}

void getRevisionList(std::string& xml, std::vector<std::string>& revisionList)
{
	tinyxml2::XMLDocument doc;
	doc.Parse(xml.c_str());

	doc.Print();
}

int main(void)
{
	// 한글 셋팅
	std::string option = "chcp 65001";
	getCommandResult(option);

	// 현재 리비전 갖고오기
	std::string currentRevision;
	currentRevision = getCurrentRevision();

	// 로그 리스트 갖고오기
	std::string xml;
	getLogList(currentRevision, xml);

	// 리비전 리스트 갖고 오기
	std::vector<std::string> revisionList;
	getRevisionList(xml, revisionList);

	return 0; 
}