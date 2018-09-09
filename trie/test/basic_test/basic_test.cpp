#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "../../src/trie.hpp"

using namespace std;
using namespace __TRIE__;

void test(const int index, const bool log_to_console = false){
    ifstream input ( "input/grader_test"  + std::to_string(index) + ".in" );
    ifstream model( "output/grader_test" + std::to_string(index) + ".ok" );
	__TRIE__::TrieBase<LowerAlphaNode<int>> test_trie;
    while(input.good()){
        char symbol = '\0';
        string word;
        input >> symbol >> word;
        if(!symbol || word.empty()){
            break;
        }
		int expected_result;
        switch(symbol){
        case '0':
            test_trie.insert(word);
            break;
        case '1':
            test_trie.remove(word);
            break;
        case '2':
			model >> expected_result;
            assert(test_trie.count(word) == expected_result);
            break;
        case '3':
			model >> expected_result;
			assert(test_trie.prefix(word) == expected_result);
            break;
        default:
			assert(false && "Invalid input");
        }
    }
	cout << "[Succeded] with file #" << index << '\n';

}

int main(int argc, char **argv)
{
    if(argc > 1){
        for(int index=1; index < argc; ++index){
            test(atoi(argv[index]), true);
        }
    }

    const int count = 20;
    for(int index = 1; index <= count; ++index){
        test(index, true);
    }



    return 0;
}
