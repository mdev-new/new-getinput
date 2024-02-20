class Controller {
private:
public:
	Controller() = delete; // fully static class
	static void init();
	static void run();

	static int deadzone = 0;
};
