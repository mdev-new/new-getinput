class Misc {
private:
public:
	Misc() = delete; // fully static class
	static void init();
	static void run();
	static void unload();
};
