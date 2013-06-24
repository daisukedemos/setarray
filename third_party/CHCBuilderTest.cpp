#include <vector>
#include "CHCBuilder.hpp"
#include "CHC.hpp"
#include <gtest/gtest.h>

using namespace std;
using namespace chc;

TEST(CHCBuilder, trivial){
	CHCBuilder chcb;
	CHC chc;

	vector<uint64_t> freq;
	chcb.SetFreq(freq);
	chcb.Build(chc);
}

TEST(CHCBuilder, small){
	CHCBuilder chcb;
	CHC chc;

	vector<uint64_t> freq;
	freq.push_back(10);
	freq.push_back(1);
	freq.push_back(4);
	freq.push_back(2);
	freq.push_back(4);

	chcb.SetFreq(freq);
	chcb.Build(chc);
	vector<uint64_t> base;
	chc.GetBase(base);
	vector<uint64_t> offset;
	chc.GetOffset(offset);
	vector<uint64_t> charToLen;
	chc.GetCharToLen(charToLen);
	for (size_t  i = 0; i < base.size(); ++i){
		cout << i << " " << base[i] << " " << offset[i] << endl;
	}
	for (size_t i = 0; i < charToLen.size(); ++i){
		cout << "chartolen " << i << " " << charToLen[i] << endl;
	}
}

TEST(CHCBuilder, large){
	CHCBuilder chcb;
	CHC chc;

	const uint64_t N = 10000;
	vector<uint64_t> freq;
	for (uint64_t i = 0; i < N; ++i){
		freq.push_back(rand());
	}
	chcb.SetFreq(freq);
	chcb.Build(chc);
	vector<uint64_t> base;
	chc.GetBase(base);
	vector<uint64_t> offset;
	chc.GetOffset(offset);
	vector<uint64_t> charToLen;
	chc.GetCharToLen(charToLen);
	uint64_t prev = 0;
	for (size_t  i = 0; i < base.size(); ++i){
		cout << i << " " << base[i] << " " << offset[i] - prev << endl;
		prev = offset[i];
	}
}