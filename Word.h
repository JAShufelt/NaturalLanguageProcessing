#pragma once
#include <string>
#include <vector>

class Word
{
public:
	std::string verb,word,type,root;
	std::vector<std::string> type_descriptor,num,tense;

	Word()
	{
		verb = "NULL";
		word = "NULL";
		type = "NULL";
		root = "NULL";
	}

	void newNoun(std::string word, std::vector<std::string> num);

	void newName(std::string word, std::vector<std::string> num);

	void newVerb(std::string word, std::string verb, std::vector<std::string> num, std::vector<std::string> tense, std::vector<std::string> type_descriptor);

	void newArticle(std::string word, std::vector<std::string> num, std::string root);

	void newPreposition(std::string word);

	void newPronoun(std::string word, std::vector<std::string> num);

	void newAdjective(std::string word);

};

