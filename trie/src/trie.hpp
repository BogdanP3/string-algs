#include <iostream>
#include <string>
#include <algorithm>
#include <climits>
#include <iostream>
#include <string_view>
#include "node_types.hpp"

namespace trie_basic{

    template<typename NODE_TYPE>
    struct Trie{
        static const int _INCORRECT_RANGE    = -1;
        static const int _INCORRECT_SYMBOL   = -2;
        static const int _MISSING_WORD       = -3;

        NODE_TYPE* head_of_tree;

		Trie() : head_of_tree(new NODE_TYPE()) {}
        Trie(const Trie& ref)  : head_of_tree(ref.head_of_tree) {}
        Trie(const Trie&& ref) : head_of_tree(std::move(ref.head_of_tree)) {ref.head_of_tree = nullptr;}

        ~Trie(){
            if(head_of_tree){
                delete head_of_tree;
            }
        }

        static bool is_correct(std::string_view word){
			return !std::any_of(
				word.begin(),
				word.end(), 
				[](const char symbol) {return NODE_TYPE::get_index(symbol) == -1; }
			);
        }

        typename NODE_TYPE::COUNTER_TYPE count(std::string_view word){
            if(!Trie::is_correct(word)){
                return Trie::_INCORRECT_SYMBOL;
            }

            NODE_TYPE* node_iterator = head_of_tree;
            for(auto c : word){
                node_iterator = node_iterator->next(c);
                if(!node_iterator){
                    return 0;
                }
            }
            return node_iterator->word_end;
        }

        typename NODE_TYPE::COUNTER_TYPE prefix(std::string_view word){
            if(!Trie::is_correct(word)){
                return Trie::_INCORRECT_SYMBOL;
            }

            NODE_TYPE* node_iterator = head_of_tree;
			assert(head_of_tree);
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

        int insert(std::string_view word){
			if (!Trie::is_correct(word)) {
                return Trie::_INCORRECT_SYMBOL;
            }

            NODE_TYPE* node_iterator = head_of_tree;
            for(auto c : word){
                node_iterator = node_iterator->next(c, NODE_TYPE::_UPDATE | NODE_TYPE::_MARK);
            }
			if(!!node_iterator){
                node_iterator->word_end++;
            }
            return 0;
        }

        int remove(std::string_view word){
            if(is_correct(word) == false){
                return Trie::_INCORRECT_SYMBOL;
            }

            NODE_TYPE *node_iterator = head_of_tree;
            for(auto c : word){
                node_iterator = node_iterator->next(c);
                if(!node_iterator){
                    return _MISSING_WORD;
                }
            }

            node_iterator = head_of_tree;
            for(auto c : word){
                node_iterator = node_iterator->next(c, NODE_TYPE::_UPDATE | NODE_TYPE::_UNMARK);
                if(!node_iterator){
                    return 0;
                }
            }
            if(!!node_iterator){
                node_iterator->word_end--;
            }
            return 0;
        }


    };

}
