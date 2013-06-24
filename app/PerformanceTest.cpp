#include <iostream>
#include <string>
#include <sstream>
#include "../lib/SetArray.hpp"
#include "../lib/SetArrayBuilder.hpp"

using namespace std;
using namespace setarray;

#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <sys/time.h>

double gettimeofday_sec() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + static_cast<double>(tv.tv_usec*1e-6);
}


void GenerateSample(vector<uint64_t>& v, uint64_t alpha_num){
	for (uint64_t i = 0; i < 128; ++i){
		v.push_back(rand() % alpha_num);
	}
	sort(v.begin(), v.end());
	v.erase(unique(v.begin(), v.end()), v.end());
}

void Test(const size_t size, uint64_t alpha_num){
	SetArrayBuilder sab;
	double begin_time;
	begin_time = gettimeofday_sec();
	vector<uint64_t> freqs(alpha_num);
	for (size_t i = 0; i < size; ++i){
		vector<uint64_t> v;
		GenerateSample(v, alpha_num);
		sab.Add(v);
		for (size_t j = 0; j < v.size(); ++j){
			++freqs[v[j]];
		}
	}
	double add_time = gettimeofday_sec() - begin_time;

	vector<uint64_t> nonzeros;
	for (uint64_t i = 0; i < freqs.size(); ++i){
		if (freqs[i] > 0) nonzeros.push_back(i);
	}

	begin_time = gettimeofday_sec();
	SetArray sa;
	sab.Build(sa);
	double build_time = gettimeofday_sec() - begin_time;

	begin_time = gettimeofday_sec();
	uint64_t dummy = 0;
	for (size_t i = 0; i < 1000; ++i){
		uint64_t pos = rand() % size;
		vector<uint64_t> ret = sa.Lookup(pos);
		dummy += ret.size();
	}
	double lookup_time = gettimeofday_sec() - begin_time;

	begin_time = gettimeofday_sec();
	for (size_t i = 0; i < 1000; ++i){
		uint64_t val = rand() % alpha_num;
		uint64_t pos = rand() % size;
		dummy += sa.Rank(val, pos);
	}
	double rank_time = gettimeofday_sec() - begin_time;

	begin_time = gettimeofday_sec();
	for (size_t i = 0; i < 1000; ++i){
		uint64_t vind = rand() % nonzeros.size();
		uint64_t val = nonzeros[vind];
		uint64_t ind = rand() % freqs[val];
		dummy += sa.Select(val, ind);
	}
	double select_time = gettimeofday_sec() - begin_time;

	uint64_t memory_size = sa.GetMemorySize();
	cout
		<< size << "\t"
		<< add_time << "\t"
		<< build_time << "\t"
		<< rank_time << "\t"
		<< lookup_time << "\t"
		<< select_time << "\t"
		<< memory_size << "\t"
		<< (double)memory_size / sa.TotalNum() << 
		endl;
}

int main(){
	cout << "size\tadd\tbuild\trank\tlookup\tselect\tmemory\tratio" << endl;
	uint64_t size = 256;
	for (uint64_t i = 8; i <= 30; size *= 2, ++i){
		Test(size, 65536);
	}
	return 0;
}
