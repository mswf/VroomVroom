class ComponentSystem
{
	public:
		ComponentSystem() {};
		virtual ~ComponentSystem() {};
        virtual bool Initialize() { return false; };
		virtual void Update(void* data) {};
		virtual void SendMessage(void* data) {};
};

