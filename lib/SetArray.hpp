#ifndef SETARRAY_SETARRAY_HPP_
#define SETARRAY_SETARRAY_HPP_

#include <vector>
#include <rsdic/RSDic.hpp>


namespace setarray{

struct Result{
	uint64_t pos;
	uint64_t val;
};

class SetArray{
public:
	SetArray();
	~SetArray();

	void Clear();

	uint64_t Length() const{
		return length_;
	}

	uint64_t TotalNum() const{
		return total_num_;
	}

	uint64_t MaxNum() const {
		return max_num_;
	}

	bool Find(uint64_t val, uint64_t pos) const;
	std::vector<uint64_t> Lookup(uint64_t pos) const;
	uint64_t Rank(uint64_t val, uint64_t pos) const;
	uint64_t Select(uint64_t val, uint64_t ind) const;
	uint64_t GetMemorySize() const;

private:
	friend class SetArrayBuilder;
	std::vector<std::pair<rsdic::RSDic, rsdic::RSDic> > layers_;

	uint64_t SelectInside(uint64_t depth, uint64_t val, uint64_t ind, uint64_t pos) const;

	uint64_t length_;
	uint64_t total_num_;
	uint64_t max_num_;
	uint64_t max_depth_;
};

}

#endif // SETARRAY_SETARRAY_HPP_