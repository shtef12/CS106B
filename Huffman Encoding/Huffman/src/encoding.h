/*
 * CS 106X Huffman Encoding
 * This file declares the functions that you will need to write in this
 * assignment for your Huffman Encoder in huffmanencoding.cpp.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * Author : Marty Stepp
 * Edit for 106X: Cynthia Lee
 * Version: 2015/11/04
 * - updated for X
 */

#ifndef _encoding_h
#define _encoding_h

#include <iostream>
#include <string>
#include "bitstream.h"
#include "HuffmanNode.h"
#include "map.h"
using namespace std;

/*
 * See huffmanencoding.cpp for documentation of these functions
 * (which you are supposed to write, based on the spec).
 */
void printInput(ibitstream &input);
Map<int, int> buildFrequencyTable(istream& input);
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable);
//recursively builds encoding map
Map<int,string> addToMap(Map<int,string> &encodingMap,string code,HuffmanNode* encodingTree);
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree);
void writeToBitStream(obitstream& output,string stringBits);
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output);
char recDecode(istream& input, HuffmanNode* encodingTree, ostream& output, int& currentBit);
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output);
void compress(istream& input, obitstream& output);
Map<int,int> getFreqMapFromCompressedInput(ibitstream& input);
void decompress(ibitstream& input, ostream& output);
void freeTree(HuffmanNode* node);

#endif
