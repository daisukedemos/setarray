#include "SetArray.hpp"

using std::vector;
using rsdic::RSDic;


namespace {
uint64_t GetBit(uint64_t val, uint64_t pos){
	return (val >> pos) & 1LLU;
}

template <class Layers, class Ind, class Depth, class MaxDepth, class Code, class Ret>
void LookupHelper(const Layers& layers, Ind ind, Depth depth, MaxDepth max_depth, Code code, Ret& ret){
	if (depth == max_depth){
		ret.push_back(code);
		return;
	}
	const RSDic& left = layers[depth].first;
	const RSDic& right = layers[depth].second;
	if (left.GetBit(ind)){
		LookupHelper(layers, left.Rank(ind, 1), depth+1, max_depth, code, ret);
	}
	if (right.GetBit(ind)){
		uint64_t shift = 1LLU << (max_depth - depth - 1);
		LookupHelper(layers, left.one_num() + right.Rank(ind, 1), depth+1, max_depth, code | shift, ret);
	}
}
}

namespace setarray{

SetArray::SetArray() : length_(0), total_num_(0), max_num_(0), max_depth_(0){
}

SetArray::~SetArray(){
}

void SetArray::Clear(){
	layers_.clear();
	length_ = 0;
	total_num_ = 0;
	max_depth_ = 0;
}

bool SetArray::Find(uint64_t val, uint64_t pos) const{
	for (uint64_t depth = 0; depth < max_depth_; ++depth){
		uint64_t bit = GetBit(val, max_depth_ - depth - 1);
		const RSDic& rsdz = layers_[depth].first;
		const RSDic& rsdo = layers_[depth].second;
		if (!bit){
			if (rsdz.GetBit(pos) == 0) return false;
			pos = rsdz.Rank(pos, 1);
		} else {
			if (rsdo.GetBit(pos) == 0) return false;
			pos = rsdz.one_num() + rsdo.Rank(pos, 1);
		}
	}
	return true;
}

vector<uint64_t> SetArray::Lookup(uint64_t ind) const{
	vector<uint64_t> ret;
	if (max_depth_ == 0){
		return ret;
	}
	LookupHelper(layers_, ind, 0, max_depth_, 0, ret);
	return ret;
}

uint64_t SetArray::Rank(uint64_t val, uint64_t pos) const{
	uint64_t bpos = 0;
	uint64_t epos = pos;
	for (uint64_t depth = 0; depth < max_depth_; ++depth){
		uint64_t bit = GetBit(val, max_depth_ - depth - 1);
		const RSDic& rsdz = layers_[depth].first;
		const RSDic& rsdo = layers_[depth].second;
		if (!bit){
			bpos = rsdz.Rank(bpos, 1);
			epos = rsdz.Rank(epos, 1);
		} else {
			bpos = rsdz.one_num() + rsdo.Rank(bpos, 1);
			epos = rsdz.one_num() + rsdo.Rank(epos, 1);
		}
	}
	return epos - bpos;
}

uint64_t SetArray::SelectInside(uint64_t depth, uint64_t val, uint64_t ind, uint64_t pos) const{
	if (depth == max_depth_){
		return pos + ind;
	}
	uint64_t bit = GetBit(val, max_depth_ - depth - 1);
	const RSDic& rsdz = layers_[depth].first;
	const RSDic& rsdo = layers_[depth].second;
	if (!bit){
		pos = rsdz.Rank(pos, 1);
		ind = SelectInside(depth+1, val, ind, pos);
		return rsdz.Select(ind, 1);
	} else {
		pos = rsdz.one_num() + rsdo.Rank(pos, 1);
		ind = SelectInside(depth+1, val, ind, pos);
		return rsdo.Select(ind - rsdz.one_num(), 1);
	}
}

uint64_t SetArray::Select(uint64_t val, uint64_t ind) const{
 	return SelectInside(0, val, ind, 0);
}

Result SetArray::Max(uint64_t beg, uint64_t end) const{
	uint64_t bpos = 0;
	Result ret;
	ret.val = 0;
	for (uint64_t depth = 0; depth < max_depth_; ++depth){
		const RSDic& rsdz = layers_[depth].first;
		const RSDic& rsdo = layers_[depth].second;
		uint64_t beg_zero = rsdz.Rank(beg, 1);
		uint64_t end_zero = rsdz.Rank(end, 1);
		uint64_t beg_one  = beg - beg_zero;
		uint64_t end_one  = end - end_zero;

		if (end_one - beg_one > 0){
			beg = rsdz.one_num() + beg_one;
			end = rsdz.one_num() + end_one;
			bpos = rsdz.one_num() + rsdo.Rank(bpos, 1);
			ret.val = (ret.val << 1) + 1;
		} else {
			beg = beg_zero;
			end = end_zero;
			bpos = rsdz.Rank(bpos, 1);
			ret.val <<= 1;
		} 
	}
	uint64_t rank = beg - bpos;
	ret.pos = Select(ret.val, rank);
}

Result SetArray::Min(uint64_t beg, uint64_t end) const{
	uint64_t bpos = 0;
	Result ret;
	ret.val = 0;
	for (uint64_t depth = 0; depth < max_depth_; ++depth){
		const RSDic& rsdz = layers_[depth].first;
		const RSDic& rsdo = layers_[depth].second;
		uint64_t beg_zero = rsdz.Rank(beg, 1);
		uint64_t end_zero = rsdz.Rank(end, 1);
		uint64_t beg_one  = beg - beg_zero;
		uint64_t end_one  = end - end_zero;

		if (end_zero - beg_zero > 0){
			beg = beg_zero;
			end = end_zero;
			bpos = rsdz.Rank(bpos, 1);
			ret.val <<= 1;
		} else{
			beg = rsdz.one_num() + beg_one;
			end = rsdz.one_num() + end_one;
			bpos = rsdz.one_num() + rsdo.Rank(bpos, 1);
			ret.val = (ret.val << 1) + 1;
		} 
	}
	uint64_t rank = beg - bpos;
	ret.pos = Select(ret.val, rank);
}

uint64_t SetArray::GetMemorySize() const{
	uint64_t bytes = 0;
	for (size_t i = 0; i < layers_.size(); ++i){
		bytes += layers_[i].first.GetUsageBytes();
		bytes += layers_[i].second.GetUsageBytes();
	}
	return bytes;
}
}