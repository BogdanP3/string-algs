#include <vector>
#include <iostream>
#include <assert.h>


namespace __TRIE__{

    template<typename SizeType = char, unsigned DictionarySize = 26>
    struct LowerAlphaNode{

        static const int _UPDATE = 1;
        static const int _MARK = 2;
        static const int _UNMARK = 4;

        SizeType wordEnd;
        SizeType prefixEnd;
        std::vector<LowerAlphaNode*> leaf;

        typedef SizeType CounterType;

        LowerAlphaNode()
		:   wordEnd(0),
            prefixEnd(0),
            leaf(DictionarySize, nullptr)
        {}

        LowerAlphaNode( const LowerAlphaNode& objReference )
        :   wordEnd(objReference.wordEnd), 
            prefixEnd(objReference.prefixEnd), 
            leaf(DictionarySize, nullptr) 
        {
            for( size_t index = 0; index < DictionarySize; ++index ){
                if(objReference.leaf[index] != nullptr){
                    leaf[index] = new LowerAlphaNode( objReference.leaf[index] );
                }
            }
        }

        LowerAlphaNode( const LowerAlphaNode&& objReference )
        :   wordEnd(objReference.wordEnd),
            prefixEnd(objReference.prefixEnd), 
            leaf(std::move(objReference.leaf))
        {}

		static char getSymbolIndex( const char symbol ){
			if ('a' <= symbol && symbol <= 'z') {
				return symbol - 'a';
			}
			return -1;
		}

        LowerAlphaNode* next( char symbol, int _FLAGS = 0 ){
			symbol = getSymbolIndex(symbol);

			assert( symbol != -1 && "Invalid input symbol" );
			assert( !(_FLAGS & _MARK) || !(_FLAGS & _UNMARK) );

            switch( _FLAGS & _UNMARK ){
                case 0:
                    if( !leaf[symbol]  && (_FLAGS & _UPDATE) ){
                        leaf[symbol] = new LowerAlphaNode();
                    }
                    if( leaf[symbol] && (_FLAGS & _MARK) ){
                        leaf[symbol]->prefixEnd++;
                    }
                    break;
                default:
                    if( !leaf[symbol] ){
                        return nullptr;
                    }
                    if( leaf[symbol]->prefixEnd > 0){
                        leaf[symbol]->prefixEnd--;
                    }
                    if( leaf[symbol]->prefixEnd == 0){
                        if( _FLAGS & _UPDATE ){
                            delete leaf[symbol];
                            leaf[symbol] = nullptr;
                        }
                    }
                    break;
            }
            return leaf[symbol];
        }


        ~LowerAlphaNode(){
            wordEnd = 0;
            prefixEnd = 0;
            for(size_t index = 0; index < DictionarySize; ++index){
                if(!leaf[index]){
                    continue;
                }
                delete leaf[index];
                leaf[index] = nullptr;
            }
        }
    };



}
