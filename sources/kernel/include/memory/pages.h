#pragma once

#include <hal/intdef.h>
#include "memmap.h"

class CPage_Manager
{
    private:
        uint8_t mPage_Bitmap[mem::PageCount / 8];

        void Mark(uint32_t page_idx, bool used);

    public:
        CPage_Manager();

        // alokuje novou stranku, vraci virtualni adresu pridelene stranky v adresnim prostoru kernelu
        uint32_t Alloc_Page();
        // TODO: alokuje count stranek, vraci zda uspelo. Pri neuspechu vraci false a nepridelene stranky jsou vraceny do bitmapy
        // bool Alloc_Pages(uint32_t count, uint32_t *pages);
        // dealokuje stranku s danou fyzickou adresou 
        void Free_Page(uint32_t fa);
};

extern CPage_Manager sPage_Manager;
