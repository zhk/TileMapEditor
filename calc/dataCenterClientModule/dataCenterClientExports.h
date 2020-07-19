#pragma once

#if DATACENTERCLIENTMODULE_EXPORTS
#	define DCClientAPI	__declspec(dllexport)
#else
#	define DCClientAPI	__declspec(dllimport)
#endif