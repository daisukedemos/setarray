#include <gtest/gtest.h>
#include <sstream>
#include "SetArray.hpp"
#include "SetArrayBuilder.hpp"

using namespace std;
using namespace setarray;

TEST(SetArray, trivial){
	SetArrayBuilder sab;
	SetArray sa;
	sab.Build(sa);
	sab.Clear();
	sab.Build(sa);

	EXPECT_EQ(0, sa.Length());
	EXPECT_EQ(0, sa.TotalNum());
	EXPECT_EQ(0, sa.MaxNum());
	vector<uint64_t> v;
	EXPECT_EQ(v, sa.Lookup(0));
	EXPECT_EQ(0, sa.Rank(0, 0));
	EXPECT_EQ(0, sa.Rank(1, 0));
}

vector<uint64_t> SetVector(const string& s){
	vector<uint64_t> ret;
	istringstream is(s);
	for (uint64_t v; is >> v; ){
		ret.push_back(v);
	}
	return ret;
}

TEST(SetArray, small){
	SetArrayBuilder sab;
	SetArray sa;
	sab.Add(SetVector("1 3"));
	sab.Add(SetVector("2"));
	sab.Add(SetVector("1"));
	sab.Add(SetVector("2"));
	sab.Build(sa);

	EXPECT_EQ(4, sa.Length());
	EXPECT_EQ(5, sa.TotalNum());
	EXPECT_EQ(3, sa.MaxNum());
	EXPECT_EQ(SetVector("1 3"), sa.Lookup(0));
	EXPECT_EQ(SetVector("2"), sa.Lookup(1));
	EXPECT_EQ(SetVector("1"), sa.Lookup(2));
	EXPECT_EQ(SetVector("2"), sa.Lookup(3));
	EXPECT_EQ(0, sa.Rank(0, 0));
	EXPECT_EQ(0, sa.Rank(1, 0));
	EXPECT_EQ(0, sa.Rank(2, 0));
	EXPECT_EQ(0, sa.Rank(3, 0));
	EXPECT_EQ(0, sa.Rank(0, 1));
	EXPECT_EQ(1, sa.Rank(1, 1));
	EXPECT_EQ(0, sa.Rank(2, 1));
	EXPECT_EQ(1, sa.Rank(3, 1));

	EXPECT_EQ(0, sa.Select(1, 0));
	EXPECT_EQ(2, sa.Select(1, 1));
	EXPECT_EQ(1, sa.Select(2, 0));
	EXPECT_EQ(3, sa.Select(2, 1));
	EXPECT_EQ(0, sa.Select(3, 0));
	EXPECT_EQ(true, sa.Find(3, 0));
	EXPECT_EQ(false, sa.Find(2, 0));
}

TEST(SetArray, set){
	SetArrayBuilder sab;
	SetArray sa;
	vector<vector<uint64_t> > orig;
	const int N = 10;
	const int ALPHA = 4;
	const int AVGNUM = 128;
	uint64_t total_num = 0;
	for (uint64_t i = 0; i < N; ++i){
		uint64_t num = rand() % AVGNUM;
		vector<uint64_t> v;
		for (uint64_t j = 0; j < num; ++j){
			v.push_back(rand() % ALPHA);
		}
		sort(v.begin(), v.end());
		v.erase(unique(v.begin(), v.end()), v.end());
		sab.Add(v);
		orig.push_back(v);
		total_num += v.size();
	}
	sab.Build(sa);

	EXPECT_EQ(N, sa.Length());
	EXPECT_EQ(total_num, sa.TotalNum());
	vector<uint64_t> cums(ALPHA);
	for (uint64_t i = 0; i < N; ++i){
		EXPECT_EQ(orig[i], sa.Lookup(i));
		for (uint j = 0; j < ALPHA; ++j){
			EXPECT_EQ(cums[j], sa.Rank(j, i));
		}
		for (size_t j = 0; j < orig[i].size(); ++j){
			uint64_t val = orig[i][j];
			EXPECT_EQ(i, sa.Select(val, cums[val]));
			EXPECT_EQ(true, sa.Find(val, i));
		}
		for (size_t j = 0; j < orig[i].size(); ++j){
			uint64_t v = orig[i][j];
			++cums[v];
		}
	}

}