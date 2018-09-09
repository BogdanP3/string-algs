#include <iostream>
#include <string>
#include <algorithm>
#include <climits>
#include <iostream>
#include <string>

#include "node_types.hpp"

/*
    TO DO:
    * use string_view once fully supported
*/

namespace __TRIE__{

    typedef const std::string& StringReference;

    template<typename nodeType>
    struct TrieBase{

        nodeType* root;

		TrieBase() : root(new nodeType()) {}
        TrieBase(const TrieBase &  objReference) : root(objReference.root) {}
        TrieBase(const TrieBase && objReference) : root(std::move(objReference.root)) {objReference.root = nullptr;}

        ~TrieBase(){
            if(root){
                delete root;
            }
        }

        static bool is_correct(StringReference word){
			return !std::any_of(
				word.begin(),
				word.end(), 
				[](const char symbol) {return nodeType::getSymbolIndex(symbol) == -1; }
			);
        }

        typename nodeType::CounterType count(StringReference word){
            if(!TrieBase::is_correct(word)){
                return -1;
            }

            nodeType* node_iterator = root;
            for(auto c : word){
                node_iterator = node_iterator->next(c);
                if(!node_iterator){
                    return 0;
                }
            }
            return node_iterator->wordEnd;
        }

        typename nodeType::CounterType prefix(StringReference word){
            if(!TrieBase::is_correct(word)){
                return -1;
            }

            nodeType* node_iterator = root;
			assert(root);
            size_t index = 0;
            for(auto c : word){
                node_iterator = node_iterator->next(c);
				if (!node_iterator){
                    break;
                }
                ++index;
            }
            return index;
        }

        int insert(StringReference word){
			if (!TrieBase::is_correct(word)) {
                return -1;
            }

            nodeType* node_iterator = root;
            for(auto c : word){
                node_iterator = node_iterator->next(c, nodeType::_UPDATE | nodeType::_MARK);
            }
			if(!!node_iterator){
                node_iterator->wordEnd++;
            }
            return 0;
        }

        int remove(StringReference word){
            if(is_correct(word) == false){
                return -1;
            }

            nodeType *node_iterator = root;
            for(auto c : word){
                node_iterator = node_iterator->next(c);
                if(!node_iterator){
                    return -1;
                }
            }

            node_iterator = root;
            for(auto c : word){
                node_iterator = node_iterator->next(c, nodeType::_UPDATE | nodeType::_UNMARK);
                if(!node_iterator){
                    return 0;
                }
            }
            if(!!node_iterator){
                node_iterator->wordEnd--;
            }
            return 0;
        }


    };

}
