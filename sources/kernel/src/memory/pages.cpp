#include <memory/pages.h>

CPage_Manager sPage_Manager;

CPage_Manager::CPage_Manager()
{
    // zadna stranka neni alokovana
    for (int i = 0; i < sizeof(mPage_Bitmap); i++)
        mPage_Bitmap[i] = 0;

    // nutno dodat, ze strankovatelna pamet implicitne nezahrnuje pamet, kam se nahralo jadro
}

void CPage_Manager::Mark(uint32_t page_idx, bool used)
{
    if (used)
        mPage_Bitmap[page_idx / 8] |= 1 << (page_idx % 8);
    else
        mPage_Bitmap[page_idx / 8] &= ~(1 << (page_idx % 8));
}

uint32_t CPage_Manager::Alloc_Page()
{
    // VELMI jednoduchy alokator stranek, prochazi bitmapu a hleda prvni volne misto
    // to je samozrejme O(n) a pro prakticke pouziti ne uplne dobre, ale k tomuto problemu az jindy

    uint32_t i, j;

    // projdeme vsechny stranky
    for (i = 0; i < mem::PageCount; i++)
    {
        // je v dane osmici volna nejaka stranka? (0xFF = vse obsazeno)
        if (mPage_Bitmap[i] != 0xFF)
        {
            // projdeme vsechny bity a najdeme ten co je volny
            for (j = 0; j < 8; j++)
            {
                if ((mPage_Bitmap[i] & (1 << j)) == 0)
                {
                    // oznacime 
                    const uint32_t page_idx = i*8 + j;
                    Mark(page_idx, true);
                    return mem::LowMemory + page_idx * mem::PageSize;
                }
            }
        }
    }

    return 0;
}

bool CPage_Manager::Alloc_Pages(uint32_t count, uint32_t *pages)
{
    int32_t i;
    for(i = 0; i < count; i++)
    {
        pages[i] = Alloc_Page();
        if (pages[i] == 0)
            goto fail;
    }

    return true;

fail:
    for(; i>=0; --i)
    {
        Free_Page(pages[i] - mem::MemoryVirtualBase);
    }

    return false;
}

constexpr uint32_t LowMemoryPhys = mem::LowMemory - mem::MemoryVirtualBase;
void CPage_Manager::Free_Page(uint32_t fa)
{
    // pro vyssi bezpecnost v nejakych safe systemech lze tady data stranky premazavat napr. nulami po dealokaci

    Mark( (fa - LowMemoryPhys) / mem::PageSize, false);
}
