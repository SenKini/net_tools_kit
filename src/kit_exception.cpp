#include "kit_exception.hpp"

KitException::KitException(std::string p, std::string m)
	: _pos(p), _mess(m) {}

std::string KitException::what() const noexcept {
	return _pos + " : " + _mess;
}