class GUI
{
	bool m_initialized = false;
public:
	GUI();
	~GUI();

	void MouseMove(int x, int y);
	void MouseLeftDown();
	void MouseLeftUp();
	void MouseMiddleDown();
	void MouseMiddleUp();
	void MouseRightDown();
	void MouseRightUp();
	void MouseWheel(int d);

	void SetScreenSize(int w, int h);

	void Begin(float deltaSeconds);
	void End();
};
