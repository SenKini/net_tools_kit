#include <exception>
#include <string>

class KitException {
private:
	std::string _pos;
	std::string _mess;

public:
	KitException(std::string p, std::string m);
	std::string what() const noexcept;
};
