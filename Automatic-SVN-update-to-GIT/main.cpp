#include <iostream>
#include <vector>
#include "tinyxml2.h"
#include <windows.h>
#include <stringapiset.h>

const int MAX_LENGTH = 300;

struct logInfo
{
	std::string revision;
	std::wstring msg;
};

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

std::string getCommandResult(std::wstring& command)
{
	FILE* file;
	char output[MAX_LENGTH];
	std::string xml;

	file = _wpopen(command.c_str(), L"r");
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
	return getCommandResult(command);
}

void getLogList(std::string currentRevision, std::string& xml)
{
	std::string command = "svn log --xml -r ";
	command.append(currentRevision);
	command.append(":HEAD ");

	xml = getCommandResult(command);
}

void getRevisionList(std::string& xml, std::vector<logInfo>& revisionList)
{
	tinyxml2::XMLDocument doc;
	doc.Parse(xml.c_str());

	doc.Print();

	tinyxml2::XMLElement *levelElement = doc.FirstChildElement("log")->FirstChildElement("logentry")->NextSiblingElement();
	for (tinyxml2::XMLElement* child = levelElement; child != NULL; child = child->NextSiblingElement())
	{
		logInfo log;

		std::string revision = child->Attribute("revision");
		log.revision = revision;

		tinyxml2::XMLElement* child2 = child->FirstChildElement("msg");
		if (nullptr != child2)
		{
			if (child2->FirstChild() == nullptr)
				continue;

			std::string str = child2->FirstChild()->Value();

			int size_needed = MultiByteToWideChar(CP_UTF8, 0, &child2->FirstChild()->Value()[0], (int)str.size(), NULL, 0);
			std::wstring msg(size_needed, 0);
			MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &msg[0], size_needed);
			log.msg = msg;
		}

		revisionList.push_back(log);
	}
}

void svnUpdateAndGitCommit(std::vector<logInfo>& revisionList)
{
	for (auto revision : revisionList)
	{
		// svn update
		std::string command = "svn update -r";
		command.append(revision.revision);
		getCommandResult(command);

		// checkout branch
		command = "git checkout develop";
		getCommandResult(command);

		// git add
		command = "git add -A";
		getCommandResult(command);

		// git commit
		std::wstring Wcommand = L"git commit -am \"";
		Wcommand.append(revision.msg);
		Wcommand.append(L"\"");
		getCommandResult(Wcommand);
	}
}

int main(void)
{
	// 현재 리비전 갖고오기
	std::string currentRevision;
	currentRevision = getCurrentRevision();

	// 로그 리스트 갖고오기
	std::string xml;
	getLogList(currentRevision, xml);

	// 리비전 리스트 갖고 오기
	std::vector<logInfo> revisionList;
	getRevisionList(xml, revisionList);

	//업데이트 & 커밋
	svnUpdateAndGitCommit(revisionList);

	return 0;
}