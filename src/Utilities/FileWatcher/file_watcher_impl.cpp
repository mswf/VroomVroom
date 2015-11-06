#include "file_watcher_impl.h"
#include <iostream>
#include "../../console.h"

UpdateListener::UpdateListener()
{

}

UpdateListener::~UpdateListener()
{

}

const std::vector< std::string >* UpdateListener::GetEvents() const
{
	return &events;
}

void UpdateListener::ClearEvents()
{
	events.clear();
}

void UpdateListener::handleFileAction(FW::WatchID watchid, const FW::String& dir, const FW::String& filename,
									  FW::Action action)
{
	if (action == FW::Action::Modified)
	{
#ifdef _WIN32
		static int FilterOutEverySecondSignal = 0;
		FilterOutEverySecondSignal++;
		if (FilterOutEverySecondSignal % 2 == 0)
		{
#endif

			//std::cout << ": DIR (" << dir + ") FILE (" + filename + ") has event " << action << std::endl;
			events.push_back(filename);
#ifdef _WIN32
		}
#endif
	}
}