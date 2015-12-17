#ifndef file_watcher_impl_h
#define file_watcher_impl_h

#include "FileWatcher.h"
#include <vector>

class UpdateListener : public FW::FileWatchListener
{
	public:
		UpdateListener();
		~UpdateListener();
		void handleFileAction(FW::WatchID watchid, const FW::String& dir, const FW::String& filename, FW::Action action) override;
		const std::vector< std::string >* GetEvents() const;
		void ClearEvents();
		std::vector< std::string > events;
	
};

#endif /* file_watcher_impl_h */

