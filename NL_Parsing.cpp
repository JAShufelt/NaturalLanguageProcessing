#include <iostream>
#include "Word.h"

std::string sentenceATN(std::vector<Word> words);
std::vector<Word> loadStandardLexicon();
std::vector<Word> tokenize(std::string sentence, std::vector<Word> lexicon);
std::string nounPhrase(std::vector<Word> words, int& currentWord, std::string& state, bool& legal, std::vector<std::string> &num);
std::string verbPhrase(std::vector<Word> words, int& currentWord, std::string& state, bool& legal, std::vector<std::string> &num);
std::string prepositionalPhrase(std::vector<Word> words, int& currentWord, std::string& state, bool& legal);
std::string numVectorToString(std::vector<std::string> num);
std::vector<std::string> numIntersection(std::vector<std::string> num1, std::vector<std::string> num2);
std::string tenseVectorToString(std::vector<std::string> tense);

int main()
{
    std::string parse = "";
    std::string sentence;
    std::vector<Word> words;
    std::vector<Word> lexicon = loadStandardLexicon();
    
    std::cout << "Enter a sentence for parsing...\n";
    std::getline(std::cin, sentence);
    words = tokenize(sentence, lexicon);
    parse = sentenceATN(words);
    std::cout << parse;
}

std::string sentenceATN(std::vector<Word> words)
{
    std::string state = "S";
    std::string parse = "(S (MOOD declarative)\n\t";
    std::string subject = "(SUBJ ";
    std::string i_obj = "(IND-OBJ ";
    std::string d_obj = "(DIR-OBJ ";
    std::string np_one = "";
    std::string np_two = "";
    
    std::vector<std::string> d_obj_num;
    std::vector<std::string> i_obj_num;
    std::vector<std::string> subj_num;
    std::vector<std::string> verb_num;

    bool obj = false;
    bool done = false;
    bool legal = true;
    int currentWord = 0;
    
    //Current State: S
    subject += nounPhrase(words, currentWord, state, legal, subj_num);
    
    if(legal)
    {
        state = "S1";
        parse += subject;
        parse += "\n\t";
        parse += verbPhrase(words, currentWord, state, legal, subj_num);
        parse += "\n\t";

        if (currentWord > words.size() - 1)
        {
            done = true;
        }
    }
    else
    {
        std::cout << "Illegal sentence structure";
    }

    if (legal && (!done))
    {
        state = "S2";
        np_one += nounPhrase(words, currentWord, state, legal, i_obj_num);
        obj = true;
        
        if(currentWord > words.size() - 1)
        {
            done = true;
        }
    }

    if(legal && (!done))
    {
        state = "S3";
        np_two += nounPhrase(words, currentWord, state, legal,d_obj_num);
        d_obj += np_two;
        i_obj += np_one;
        
        if (currentWord > words.size() - 1)
        {
            done = true;
        }
        parse += (i_obj + "\n\t" + d_obj + "\n\t");
    }
    else
    {
        if(obj)
        {
            d_obj += np_one;
            parse += (d_obj + "\n\t");
        }
        
    }

    if (legal && (!done))
    {
        state = "S4";
        parse += prepositionalPhrase(words, currentWord, state, legal);
        parse += "\n\t";

        if (currentWord > words.size() - 1)
        {
            done = true;
        }
    }

    return parse;
    
}

std::string nounPhrase(std::vector<Word> words, int &currentWord,std::string &state, bool &legal, std::vector<std::string> &num)
{
    int saveWord = currentWord;
    
    std::string parse = "(NP "; 
    std::string localState = "NP";
  
    if(words[currentWord].type == "article")
    {
        parse += "(ART \"" + words[currentWord].word + "\")";
        currentWord++;
        localState = "NP1";
    }
    else if(words[currentWord].type == "pronoun")
    {
        parse += "(PRONOUN \"" + words[currentWord].word + "\")" +numVectorToString(words[currentWord].num);
        num = words[currentWord].num;
        currentWord++;
        localState = "NP2";
    }
    else if(words[currentWord].type == "noun")
    {
        parse += "(NOUN \"" + words[currentWord].word + "\")" + numVectorToString(words[currentWord].num);
        num = words[currentWord].num;
        currentWord++;
        localState = "NP2";
    }
    else if(words[currentWord].type == "name")
    {
        parse += "(NAME \"" + words[currentWord].word + "\")" + numVectorToString(words[currentWord].num);
        num = words[currentWord].num;
        currentWord++;
        localState = "NP2";
    }
    else
    {
        std::cout << "Syntax error at word, \"" << words[currentWord].word << "\"" << std::endl;
        legal = false;
    }

    while(localState == "NP1")  //If NP1 is entered, 
    {
        if (words[currentWord].type == "adjective")
        {
            parse += "(ADJ \"" + words[currentWord].word + "\")";
            currentWord++;
            localState = "NP1";
        }
        else if (words[currentWord].type == "noun")
        {
            parse += "(NOUN \"" + words[currentWord].word + "\")" + numVectorToString(words[currentWord].num);
            num = words[currentWord].num;
            currentWord++;
            localState = "NP2";
        }
        else
        {
            std::cout << "Syntax error at word, \"" << words[currentWord].word << "\"" << std::endl;
            legal = false;
        }
    }

    if(legal && (currentWord <= (words.size()-1)))
    {
        parse += prepositionalPhrase(words, currentWord, localState, legal);
    }
  
    if(legal == false)
    {
        currentWord = saveWord;
        return "";
    }
    else
    {
        return (parse + ")");
    }

}

std::string prepositionalPhrase(std::vector<Word> words, int& currentWord, std::string& state, bool& legal)
{
    std::vector<std::string> obj_num;
    std::string parse = "(PP ";
    std::string localState;
    int saveWord = currentWord;

    
    if(state == "NP2")
    {
        localState = "PP";
    }
    if(state == "S4")
    {
        localState = "PP";
    }

    if(localState == "PP")
    {
        if(words[currentWord].type == "preposition")
        {
            parse += "(PREP \"" + words[currentWord].word + "\")";
            currentWord++;
            localState = "PP1";
        }
        else if(words[currentWord].type == "pp noun")
        {
            parse += "(PP NOUN  \"" + words[currentWord].word + "\")";
            currentWord++;
            localState = "PP1";
        }
        else
        {
            legal = true;
            currentWord = saveWord;
            return "";
        }
    }

    if(localState == "PP1")
    {
        parse += nounPhrase(words, currentWord, localState, legal, obj_num);
    
    }

    return parse;
}

std::string verbPhrase(std::vector<Word> words, int& currentWord, std::string& state, bool& legal, std::vector<std::string> &num)
{
    std::string parse = "(VP ";
    std::string localState;
    int saveWord = currentWord;


    if(state == "S1")
    {
        localState = "VERB";
    }

    if(words[currentWord].verb == "do")
    {
        std::vector<std::string> intersection = numIntersection(words[currentWord].num, num);
        parse += "(AUX VERB \"" + words[currentWord].word + "\")" + numVectorToString(intersection);
        currentWord++;

        localState = "V1";
    }
    else
    {
        localState = "V1";
    }

    //Current State: V1
    if((words[currentWord].type == "verb") && (words[currentWord].verb != "do"))
    {
        std::vector<std::string> intersection = numIntersection(words[currentWord].num, num);
        parse += "(VERB \"" + words[currentWord].word + "\")" + numVectorToString(intersection) + " " + tenseVectorToString(words[currentWord].tense);
        currentWord++;
        localState = "V2";
    }
    else
    {
        currentWord = saveWord;
        return "";
        legal = false;
    }

    if(localState == "V2")
    {
        return (parse += ")");
    }
    
}

std::vector<Word> loadStandardLexicon()
{
    std::vector<Word> lexicon;
    std::vector<std::string> type_descriptor;
    std::vector<std::string> num;
    std::vector<std::string> tense;

    Word mary;
        num = { "3s" };
        mary.newName("mary", num);
        lexicon.push_back(mary);

    Word did;
        tense = { "past" };
        num = { "1s","1p","2s","2p","3s","3p" };
        type_descriptor = {"transitive", "bitransitive"};
        did.newVerb("did","do", num, tense, type_descriptor);
        
        lexicon.push_back(did);
        tense.clear();
        num.clear();
        type_descriptor.clear();

    Word walks;
        tense = { "present" };
        num = { "3s" };
        walks.newVerb("walks", "walk", num, tense, type_descriptor);
        
        lexicon.push_back(walks);
        tense.clear();
        num.clear();
        type_descriptor.clear();

    Word walked;
        tense = { "past" };
        num = { "1s","1p","2s","2p","3s","3p" };
        walked.newVerb("walked", "walk", num, tense, type_descriptor);

        lexicon.push_back(walked);
        tense.clear();
        num.clear();
        type_descriptor.clear();

    Word give;
        type_descriptor = { "transitive", "bitransitive" };
        tense = { "ininfitive" , "present" };
        num = { "1s","1p","2s","2p","3p" };
        give.newVerb("give", "give", num, tense, type_descriptor);

        lexicon.push_back(give);
        tense.clear();
        num.clear();
        type_descriptor.clear();

    Word gave;
        type_descriptor = { "transitive", "bitransitive" };
        tense = { "past" };
        num = { "1s","1p","2s","2p","3s","3p" };
        gave.newVerb("gave", "give", num, tense, type_descriptor);

        lexicon.push_back(gave);
        tense.clear();
        num.clear();
        type_descriptor.clear();

    Word me;
        num = { "1s" };
        me.newPronoun("me", num);

        lexicon.push_back(me);
        tense.clear();
        num.clear();
        type_descriptor.clear();
    
    Word a;
        num = { "3s" };
        a.newArticle("a", num, "");

        lexicon.push_back(a);
        tense.clear();
        num.clear();
        type_descriptor.clear();

    Word an;
        num = { "3s" };
        an.newArticle("an", num, "");

        lexicon.push_back(an);
        tense.clear();
        num.clear();
        type_descriptor.clear();

    Word the;
        num = { "3s", "3p" };
        the.newArticle("the", num, "");

        lexicon.push_back(the);
        tense.clear();
        num.clear();
        type_descriptor.clear();

    Word her;
        num = { "3s", "3p" };
        her.newArticle("her", num, "she");

        lexicon.push_back(her);
        tense.clear();
        num.clear();
        type_descriptor.clear();

    Word picture;
        num = { "3s" };
        picture.newNoun("picture", num);

        lexicon.push_back(picture);
        tense.clear();
        num.clear();
        type_descriptor.clear();

    Word man;
        num = { "3s" };
        man.newNoun("man", num);

        lexicon.push_back(man);
        tense.clear();
        num.clear();
        type_descriptor.clear();

    Word dreams;
        num = { "3p" };
        dreams.newNoun("dreams", num);

        lexicon.push_back(dreams);
        tense.clear();
        num.clear();
        type_descriptor.clear();

    Word boat;
        num = { "3s" };
        boat.newNoun("boat", num);

        lexicon.push_back(boat);
        tense.clear();
        num.clear();
        type_descriptor.clear();

    Word of;
        of.newPreposition("of");

        lexicon.push_back(of);
        tense.clear();
        num.clear();
        type_descriptor.clear();

    Word in;
        in.newPreposition("in");

        lexicon.push_back(in);
        tense.clear();
        num.clear();
        type_descriptor.clear();

    Word on;
        on.newPreposition("on");

        lexicon.push_back(on);
        tense.clear();
        num.clear();
        type_descriptor.clear();

    Word old;
        old.newAdjective("old");

        lexicon.push_back(old);
        tense.clear();
        num.clear();
        type_descriptor.clear();
    
    Word nice;
        nice.newAdjective("nice");

        lexicon.push_back(nice);
        tense.clear();
        num.clear();
        type_descriptor.clear();

    Word dogs;
        num = { "3p" };
        dogs.newNoun("dogs", num);

        lexicon.push_back(dogs);
        tense.clear();
        num.clear();
        type_descriptor.clear();

    Word ball;
        num = { "3s" };
        ball.newNoun("ball", num);

        lexicon.push_back(ball);
        tense.clear();
        num.clear();
        type_descriptor.clear();

    return lexicon;
}

std::vector<Word> tokenize(std::string sentence, std::vector<Word> lexicon)
{
    std::vector<std::string> wordStrings;
    std::vector<Word> words;
    std::string temp = "";
    char SPACE = ' ';
    char PERIOD = '.';

    for(int i = 0; i < sentence.size(); i++)    //Extract all of the words from a sentence into individual strings.
    {
        sentence[i] = tolower(sentence[i]);     //Make character at i lowercase.
        
        if((sentence[i] != SPACE) && (sentence[i] != PERIOD))
        {
            temp += sentence[i];
        }
        else
        {
            wordStrings.push_back(temp);
            temp = "";
        }
    }


    for(int i = 0; i < wordStrings.size(); i++) //Look for the word in the lexicon and push it into the words vector.
    {
        bool flag = false;
        for(int j = 0; j < lexicon.size() && !flag; j++)
        {
            if(wordStrings[i] == lexicon[j].word)
            {
                words.push_back(lexicon[j]);
                flag = true;
            }
        }
        if(!flag)   //Word was not found in the lexicon
        {
            std::cout << "The word, \"" << wordStrings[i] << "\" was not found in the lexicon.\nThis word was not included in the parse.\n";
        }

    }

    return words;
}

std::string numVectorToString(std::vector<std::string> num)
{
    std::string temp = "(NUM ";

    for(int i = 0; i < num.size(); i++)
    {
        temp += num[i];
        if(i < num.size() - 1)
        {
            temp += ",";
        }
    }
    temp += ")";
    return temp;
}

std::string tenseVectorToString(std::vector<std::string> tense)
{
    std::string temp = "(TENSE ";

    for (int i = 0; i < tense.size(); i++)
    {
        temp += tense[i];
        if (i < tense.size() - 1)
        {
            temp += ",";
        }
    }
    temp += ")";
    return temp;
}



std::vector<std::string> numIntersection(std::vector<std::string> num1, std::vector<std::string> num2)
{
    std::vector<std::string> intersection;

    for(int i = 0; i < num1.size(); i++)
    {
        for(int j = 0; j < num2.size(); j++)
        {
            if(num1[i] == num2[j])
            {
                intersection.push_back(num1[i]);
            }
        }
    }

    return intersection;
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
