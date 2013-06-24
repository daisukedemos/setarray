#ifndef SETARRAY_SETARRAY_BUILDER_HPP_
#define SETARRAY_SETARRAY_BUILDER_HPP_

#include <vector>
#include <stdint.h>


namespace setarray{

class SetArray;
struct Range;

class SetArrayBuilder{
public:
	SetArrayBuilder();
	void Add(const std::vector<uint64_t>& vals);
	void Build(SetArray& sa);
	void Clear();

private:
	std::pair<std::vector<uint64_t>, std::vector<bool> > vals_;
};

}

#endif // SETARRAY_SETARRAY_BUILDER_HPP_