#include <vector>
#include <iostream>
#include <assert.h>


namespace trie_basic{

    template<typename _data_counter_type = char, unsigned leaf_count = 26>
    struct node{

        static const int _UPDATE = 1;
        static const int _MARK = 2;
        static const int _UNMARK = 4;

        _data_counter_type word_end;
        _data_counter_type word_middle;
        std::vector<node*> leaf;

        typedef _data_counter_type COUNTER_TYPE;

        node()
			: word_end(0), word_middle(0), leaf(leaf_count, nullptr) {}

        node(const node& ref)
        : word_end(ref.word_end), word_middle(ref.word_middle), leaf(leaf_count, nullptr) {
            for(size_t i = 0; i < leaf_count; ++i){
                if(ref.leaf[i] != nullptr){
                    leaf[i] = new node( ref.leaf[i] );
                }
            }
        }

        node(const node&& ref)
        : word_end(ref.word_end), word_middle(ref.word_middle), leaf(std::move(ref.leaf))
        {}

		static
		char get_index(const char symbol) {
			if ('a' <= symbol && symbol <= 'z') {
				return symbol - 'a';
			}
			return -1;
		}

        node* next(char symbol, int _FLAGS = 0){
			symbol = get_index(symbol);

			assert(symbol != -1 && "Invalid input symbol");
			assert(!(_FLAGS & _MARK) || !(_FLAGS & _UNMARK));

            switch(_FLAGS & _UNMARK){
                case 0:
                    if( !leaf[symbol]  && (_FLAGS & _UPDATE)){
                        leaf[symbol] = new node();
                    }
                    if( leaf[symbol] && (_FLAGS & _MARK)){
                        leaf[symbol]->word_middle++;
                    }
                    break;
                default:
                    if(!leaf[symbol]){
                        return nullptr;
                    }
                    if(leaf[symbol]->word_middle > 0){
                        leaf[symbol]->word_middle--;
                    }
                    if(leaf[symbol]->word_middle == 0){
                        if(_FLAGS & _UPDATE){
                            delete leaf[symbol];
                            leaf[symbol] = nullptr;
                        }
                    }
                    break;
            }
            return leaf[symbol];
        }


        ~node(){
            word_end = 0;
            word_middle = 0;
            for(size_t i = 0; i < leaf_count; ++i){
                if(!leaf[i]){
                    continue;
                }
                delete leaf[i];
                leaf[i] = nullptr;
            }
        }
    };



}
