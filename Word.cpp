#include "Word.h"

void Word::newNoun(std::string word, std::vector<std::string> num)
{
	this->type = "noun";
	this->word = word;
	this->num = num;
}

void Word::newName(std::string word, std::vector<std::string> num)
{
	this->type = "name";
	this->word = word;
	this->num = num;
}

void Word::newVerb(std::string word, std::string verb, std::vector<std::string> num, std::vector<std::string> tense, std::vector<std::string> type_descriptor)
{
	this->type = "verb";
	this->verb = verb;
	this->word = word;
	this->num = num;
	this->tense = tense;
	this->type_descriptor = type_descriptor;
}

void Word::newArticle(std::string word, std::vector<std::string> num, std::string root)
{
	this->type = "article";
	this->word = word;
	this->num = num;
	this->root = root;
}

void Word::newPreposition(std::string word)
{
	this->type = "preposition";
	this->word = word;
}

void Word::newPronoun(std::string word, std::vector<std::string> num)
{
	this->type = "pronoun";
	this->word = word;
	this->num = num;
}

void Word::newAdjective(std::string word)
{
	this->type = "adjective";
	this->word = word;
}