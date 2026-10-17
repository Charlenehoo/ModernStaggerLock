#include "LoadGame.h"

SKSEPluginInfo(
		.Version = Plugin::Version,
	.Name = Plugin::NAME,
	.Author = Plugin::AUTHOR);

SKSE_PLUGIN_LOAD(const SKSE::LoadInterface* a_skse)
{
#ifndef NDEBUG
	while (!IsDebuggerPresent()) {
		Sleep(100);
	}
#endif

	DKUtil::Logger::Init(Plugin::NAME, REL::Module::get().version().string());

	SKSE::Init(a_skse);

	INFO("{} v{} loaded", Plugin::NAME, Plugin::Version);

	auto g_message = SKSE::GetMessagingInterface();
	if (!g_message) {
		ERROR("Messaging Interface Not Found!");
		return false;
	}

	g_message->RegisterListener(ModernStaggerLock::EventCallback);

	return true;
}