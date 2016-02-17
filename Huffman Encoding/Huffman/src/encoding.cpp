

#include "encoding.h"
#include "pqueue.h"
#include "filelib.h"
#include "strlib.h"


/**
 * @brief buildFrequencyTable
 * Builds a map based on the frequencies of each character in the stream
 * @param input
 * @return
 */
Map<int, int> buildFrequencyTable(istream& input) {

    Map<int, int> freqTable;

    char letter;
    while(input.get(letter)){
        if(freqTable.containsKey(letter)){
            int freq = freqTable.get(letter);
            freqTable.add(letter,++freq);
        }else{
            freqTable.add(letter,1);
        }
    }
    freqTable.add(PSEUDO_EOF,1);

    return freqTable;
}

/**
 * @brief buildEncodingTree
 * Takes a frequency table and builds a binary tree based on the frequency of characters
 * @param freqTable
 * @return
 */
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {

    PriorityQueue<HuffmanNode*> queue;

    //create priority queue
    for(int ch : freqTable){
        HuffmanNode* node = new HuffmanNode;
        node->character = ch;
        node->count = freqTable.get(ch);
        node->zero = NULL;
        node->one = NULL;
        queue.enqueue(node,node->count);
    }

    //create tree
    HuffmanNode* root;
    while(queue.size() != 1){
        HuffmanNode* left = queue.dequeue();
        HuffmanNode* right = queue.dequeue();
        root = new HuffmanNode(NOT_A_CHAR,(left->count+right->count),left,right); //create root node with left and right nodes as children
        queue.enqueue(root,root->character);
    }

    return root;
}

/**
 * @brief buildEncodingMap
 * Returns a map of the codes for each character in the tree
 * @param encodingTree
 * @return
 */
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    if(encodingTree == NULL) return encodingMap;

    encodingMap = addToMap(encodingMap,"",encodingTree);

    return encodingMap;
}

/**
 * @brief addToMap
 * Recursively generates a map of codes for each character
 * @param encodingMap
 * @param code
 * @param encodingTree
 * @return
 */
Map<int,string> addToMap(Map<int,string> &encodingMap, string code, HuffmanNode* encodingTree){
    if(encodingTree == NULL) return encodingMap;
    if(encodingTree->character != NOT_A_CHAR){
        int character = encodingTree->character;
        encodingMap.add(character,code);
        return encodingMap;
    }
    string leftCode = code+"0";
    string rightCode = code+"1";
    Map<int,string> leftMap = addToMap(encodingMap,leftCode,encodingTree->zero);
    Map<int,string> rightMap = addToMap(encodingMap,rightCode,encodingTree->one);

    leftMap.putAll(rightMap);
    return leftMap;
}

/**
 * @brief encodeData
 * Writes the characters into their binary representations
 * @param input
 * @param encodingMap
 * @param output
 */
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    rewindStream(input);
    char letter;
    while(input.get(letter)){
        if(letter != PSEUDO_EOF){
            string code = encodingMap.get(letter);
            writeToBitStream(output,code);
        }
    }

    string eof = encodingMap.get(PSEUDO_EOF);
    writeToBitStream(output,eof);
}

void writeToBitStream(obitstream& output, string stringBits){
    for(int i = 0; i < stringBits.length(); i++){
        if(stringBits[i] == '0'){
            output.writeBit(0);
        }else if(stringBits[i] == '1'){
            output.writeBit(1);
        }
    }
}

/**
 * @brief recDecode
 * Recursively traverses through a tree and prints characters to a stream
 * @param input
 * @param encodingTree
 * @param output
 * @param currentBit
 */
void recDecode(ibitstream& input, HuffmanNode* encodingTree, ostream& output, int& currentBit){
    if(encodingTree->character != NOT_A_CHAR){
        if(encodingTree->character == PSEUDO_EOF){
            currentBit = -1;
            return;
        }
        output.put(encodingTree->character);
        return;
    }else{
        if(currentBit == 0){
            currentBit = input.readBit();
            return recDecode(input,encodingTree->zero,output,currentBit);
        }else if(currentBit == 1){
            currentBit = input.readBit();
            return recDecode(input,encodingTree->one,output,currentBit);
        }
    }
    return;
}

/**
 * @brief decodeData
 * Decodes a binary stream and writes it to another stream
 * @param input
 * @param encodingTree
 * @param output
 */
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    rewindStream(input);
    if(input.get() == '{')              //ignore the frequency map
        input.ignore(input.size(),'}');
    int curr = input.readBit();
    while(curr != -1){
        recDecode(input,encodingTree,output,curr);
    }
    return;
}

void compress(istream& input, obitstream& output) {
    Map<int,int> freqTable = buildFrequencyTable(input);
    output << freqTable.toString();
    HuffmanNode* tree = buildEncodingTree(freqTable);
    Map<int,string> encodingMap = buildEncodingMap(tree);
    encodeData(input,encodingMap,output);
    freeTree(tree);
}

void decompress(ibitstream& input, ostream& output) {
    Map<int,int> freqTable = getFreqMapFromCompressedInput(input);
    HuffmanNode* tree = buildEncodingTree(freqTable);
    decodeData(input,tree,output);
    freeTree(tree);
}

Map<int,int> getFreqMapFromCompressedInput(ibitstream& input){
    Map<int,int> freqTable;
    int currChar = input.get();
    string letters;

    while(currChar != '}'){
        if(currChar == '}') break;
        if(currChar == '{' || currChar == ' '){ //go to next letter
            currChar = input.get();
            continue;
        }
        if(currChar != ','){                    //add letter
            letters += currChar;
        }
        if(currChar == ','){
            Vector<string> strings = stringSplit(letters,":");  //split strings and add to frequency map
            letters = "";
            freqTable.add(stringToInteger(strings.get(0)),stringToInteger(strings.get(1)));
        }
        currChar = input.get();
    }

    freqTable.add(PSEUDO_EOF,1);
    return freqTable;
}

void printInput(ibitstream& input){
    int curr = input.get();
    while(curr){
        cout << char(curr);
        curr = input.get();
    }
    cout << endl;
}

void freeTree(HuffmanNode* node) {
    if(node != NULL){
        freeTree(node->one);
        freeTree(node->zero);
        delete node;
    }
}
