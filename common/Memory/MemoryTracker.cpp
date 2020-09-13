#include "MemoryTracker.h"
#include "npas4/Npas4.h"

namespace Memory
{
    namespace MemoryTracker
    {
        void InitMemoryTracker()
        {

        }

        void SetCurrentFrameIndex(u32 frameIndex)
        {
            // TODO: Add storage of previous frames of memory usage so we can do graphs
        }

        size_t GetMemoryUsage()
        {
            return npas4::GetRAMPhysicalUsedByCurrentProcess();
        }

        size_t GetMemoryUsagePeak()
        {
            return npas4::GetRAMPhysicalUsedByCurrentProcessPeak();
        }

        size_t GetMemoryAvailable()
        {
            return npas4::GetRAMPhysicalAvailable();
        }

        size_t GetMemoryBudget()
        {
            return npas4::GetRAMPhysicalTotal();
        }
    }
};

