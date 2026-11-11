#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <list>
#include <map>
#include <vector>
using namespace std;

string toLower(string w)
{
    for (int i = 0; i < w.length(); i++)
    {
        unsigned char c = w[i];

        if (c >= 'A' && c <= 'Z')
        {
            w[i] = c + 32;
        }
        else if (c == 0xD0 && i + 1 < w.length())
        {
            unsigned char d = w[i + 1];

            if (d == 0x81)
            {
                w[i] = (char)0xD1;
                w[i + 1] = (char)0x91;
            }
            else if (d >= 0x90 && d <= 0x9F)
            {
                w[i + 1] = d + 0x20;
            }
            else if (d >= 0xA0 && d <= 0xAF)
            {
                w[i] = (char)0xD1;
                w[i + 1] = d - 0x20;
            }
            i++;
        }
        else if (c == 0xD1 && i + 1 < w.length())
        {
            i++;
        }
    }
    return w;
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        cout << "Usage: word_count.exe input.txt output.csv" << endl;
        return 1;
    }

    ifstream fin(argv[1]);
    if (!fin.is_open())
    {
        cout << "Cannot open input file" << endl;
        return 1;
    }

    list<string> lines;
    string line;
    while (getline(fin, line))
    {
        lines.push_back(line);
    }
    fin.close();

    map<string, int> words;
    int total = 0;

    list<string>::iterator it;
    for (it = lines.begin(); it != lines.end(); it++)
    {
        string s = *it;
        string word = "";

        for (int i = 0; i < s.length(); i++)
        {
            char c = s[i];

            bool isLetter = (c >= 'a' && c <= 'z')
                         || (c >= 'A' && c <= 'Z')
                         || (c >= '0' && c <= '9')
                         || (c < 0);

            if (isLetter)
            {
                word = word + c;
            }
            else
            {
                if (word != "")
                {
                    words[toLower(word)]++;
                    total++;
                    word = "";
                }
            }
        }

        if (word != "")
        {
            words[toLower(word)]++;
            total++;
        }
    }

    vector<string> names;
    vector<int> counts;
    map<string, int>::iterator mit;
    for (mit = words.begin(); mit != words.end(); mit++)
    {
        names.push_back(mit->first);
        counts.push_back(mit->second);
    }

    int n = names.size();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n - 1 - i; j++)
        {
            if (counts[j] < counts[j + 1])
            {
                int tmpCount = counts[j];
                counts[j] = counts[j + 1];
                counts[j + 1] = tmpCount;

                string tmpName = names[j];
                names[j] = names[j + 1];
                names[j + 1] = tmpName;
            }
        }
    }

    ofstream fout(argv[2]);
    if (!fout.is_open())
    {
        cout << "Cannot open output file" << endl;
        return 1;
    }

    fout << "Word, Count, Frequency (%)" << endl;
    fout << fixed << setprecision(2);

    for (int i = 0; i < n; i++)
    {
        double percent = counts[i] * 100.0 / total;
        fout << names[i] << ", " << counts[i] << ", " << percent << endl;
    }

    fout.close();
    return 0;
}
