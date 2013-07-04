#include <algorithm>
#include "SetArray.hpp"
#include "SetArrayBuilder.hpp"
#include <rsdic/RSDicBuilder.hpp>

using std::vector;
using std::pair;
using std::sort;
using rsdic::RSDicBuilder;
using rsdic::RSDic;

namespace {
uint64_t GetMax(vector<uint64_t>& v){
	uint64_t maxval = 0;
	for (size_t i = 0; i < v.size(); ++i){
		if (v[i] > maxval){
			maxval = v[i];
		}
	}
	return maxval;
}


uint64_t GetLen(uint64_t val){
	uint64_t len = 0;
	while (val > 0){
		val >>= 1;
		++len;
	}
	return len;
}


template <class V, class Depth, class NextLeft, class NextRight, class RSDBLeft, class RSDBRight>
void Filter(const V& vals, Depth depth, NextLeft& next_left, NextRight& next_right, RSDBLeft& rsdb_left, RSDBRight& rsdb_right){
	const vector<uint64_t>& v = vals.first;
	const vector<bool>& begs = vals.second;
	size_t begs_pos = 0;

for (uint64_t i = 0; i < v.size();){
		uint64_t retl = 0;
		uint64_t retr = 0;
		for ( ; begs[begs_pos] == false; ++begs_pos, ++i){
			uint64_t bit = (v[i] >> depth) & 1LLU;
			if (!bit){
				next_left.first.push_back(v[i]);
				next_left.second.push_back(false);
				retl++;
			} else {
				next_right.first.push_back(v[i]);
				next_right.second.push_back(false);
				retr++;
			}
		}
		++begs_pos;

		if (retl > 0){
			next_left.second.push_back(true);
			rsdb_left.PushBack(1);
		} else {
			rsdb_left.PushBack(0);
		}
		if (retr > 0){
			next_right.second.push_back(true);
			rsdb_right.PushBack(1);
		} else {
			rsdb_right.PushBack(0);
		}
	}
}


} // namespace


namespace setarray{

SetArrayBuilder::SetArrayBuilder() {
}

void SetArrayBuilder::Add(const vector<uint64_t>& vals){
	for (size_t i = 0; i < vals.size(); ++i){
		vals_.first.push_back(vals[i]);
		vals_.second.push_back(false);
	}
	vals_.second.push_back(true);
}

void SetArrayBuilder::Clear(){
	vals_.first.clear();
	vals_.second.clear();
}

void SetArrayBuilder::Build(SetArray& sa){
	uint64_t maxval = GetMax(vals_.first);
	uint64_t maxlen = GetLen(maxval);
	sa.length_ = vals_.second.size() - vals_.first.size();
	sa.total_num_ = vals_.first.size();
	sa.max_num_ = maxval;
	sa.max_depth_ = maxlen;
	sa.layers_.resize(maxlen);

	pair<vector<uint64_t>, vector<bool> > left;
	pair<vector<uint64_t>, vector<bool> > right;

	left.first.swap(vals_.first);
	left.second.swap(vals_.second);
	for (uint64_t depth = 0; depth < maxlen; ++depth){
		pair<vector<uint64_t>, vector<bool> > next_left;
		pair<vector<uint64_t>, vector<bool> > next_right;

		RSDicBuilder rsdb_left;
		RSDicBuilder rsdb_right;
		Filter(left,  maxlen - depth - 1, next_left, next_right, rsdb_left, rsdb_right);
		Filter(right, maxlen - depth - 1, next_left, next_right, rsdb_left, rsdb_right);
		left.first.swap(next_left.first);
		left.second.swap(next_left.second);
		right.first.swap(next_right.first);
		right.second.swap(next_right.second);

		RSDic& rsd_left = sa.layers_[depth].first;
		RSDic& rsd_right = sa.layers_[depth].second;
		rsdb_left.Build(rsd_left);
		rsdb_right.Build(rsd_right);
	}
}

}