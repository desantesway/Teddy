#include <Teddy.h>

class KeyStar : public Teddy::Application {
public:
	KeyStar() {
		// Initialization code can go here
	}
	~KeyStar() {
		// Cleanup code can go here
	}
};


Teddy::Application* Teddy::CreateApplication() {
	return new KeyStar();
} // CreateApplication